#include "jpeg.h"
/* real declaration of global variables here */
/* see jpeg.h for more info			*/
// global variables related to the C-HEAP FIFO
volatile fifo_t* producer42 = (fifo_t*) mb4_cmemin0_BASEADDR;
volatile fifo_t* consumer42 = (fifo_t*) mb2_cmemin0_pt_REMOTEADDR;
volatile int* producer_data42 = (int*) (mb2_cmemin0_pt_REMOTEADDR + 1 * sizeof(fifo_t));
volatile int* consumer_data42 = (int*) (mb2_cmemin0_BASEADDR + 1 * sizeof(fifo_t));

// synchronization variable that indicates that C-HEAP FIFO is initialized.
volatile int* fifo_sync42 = (int*) (mb2_cmemin0_pt_REMOTEADDR + 1 * sizeof(fifo_t) + FIFO_SIZE4_2 * sizeof(PBlock));

/* descriptors for 3 components */
cd_t comp[3];
/* decoded DCT blocks buffer */
PBlock *MCU_buff[10];
/* components of above MCU blocks */
int MCU_valid[10];

/* quantization tables */
PBlock *QTable[4];
int QTvalid[4];

/* Video frame size     */
int x_size, y_size;
/* down-rounded Video frame size (integer MCU) */
int rx_size, ry_size;
/* MCU size in pixels   */
int MCU_sx, MCU_sy;
/* picture size in units of MCUs */
int mx_size, my_size;
/* number of components 1,3 */
int n_comp;

/* scratch frequency buffer */
FBlock *FBuff;

/* frame started ? current component ? */
int in_frame, curcomp;
/* current position in MCU unit */
int MCU_row, MCU_column;

/* stuff bytes in entropy coded segments */
int stuffers = 0;
/* bytes passed when searching markers */
int passed = 0;

/* Extra global variables for 5kk03 */
int vld_count = 0;		/* Counter used by FGET and FSEEK in 5kk03.c */
/* End extra global variables for 5kk03 */

int JpegToBmp();

/*-----------------------------------------------------------------*/
/*		MAIN		MAIN		MAIN		   */
/*-----------------------------------------------------------------*/
int main (void)
{
	// Sync with the monitor.
	mk_mon_sync();
	// Enable stack checking.
	start_stack_check();
	begin_use();
	// initialize FIFO
	cheap_init_fifo(
			producer42,
			consumer42,
			sizeof(PBlock), // Token size
			FIFO_SIZE4_2,  // fifo size
			(unsigned int)producer_data42,
			(unsigned int)consumer_data42,
#ifdef USE_DMA
			mb4_dma0_BASEADDR,
			mb4_cmemout0_BASEADDR+sizeof(int),
			mb2_dma0_BASEADDR,
			mb2_cmemout0_BASEADDR);
#else
			0,
			0,
			0,
			0);
#endif

	unsigned int *f = (unsigned int *)mb4_cmemout0_BASEADDR;	
	*f =1 ;
	DMA_usage((void*)fifo_sync42, (void*)f, 1, (void*)mb4_dma0_BASEADDR);
	hw_dma_send_addr((void*)fifo_sync42, (void*)f, 1, (void*)mb4_dma0_BASEADDR);
	

    JpegToBmp();
	end_use();
   mk_mon_debug_info(0x00444D41);//start of DMA_counters
   //print the dma_counters according to the following order here
     mk_mon_debug_info(to_core1);
     mk_mon_debug_info(to_core2);
     mk_mon_debug_info(to_core3);
     mk_mon_debug_info(to_core4);
     mk_mon_debug_info(to_DDR);
     mk_mon_debug_info(from_core1);
     mk_mon_debug_info(from_core2);
     mk_mon_debug_info(from_core3);
     mk_mon_debug_info(from_core4);
     mk_mon_debug_info(from_DDR);
   mk_mon_debug_info(0xFF444D41);//end of DMA_counters

	mk_mon_debug_tile_finished();
	return 0;
}

int JpegToBmp()
{
	unsigned int aux, mark;
	int n_restarts, restart_interval, leftover;	/* RST check */
	int i, j;
	
	/* First find the SOI marker: */
	aux = get_next_MK();
	if (aux != SOI_MK)
		aborted_stream(0);
		
	in_frame = 0;
	restart_interval = 0;
	for (i = 0; i < 4; i++)
		QTvalid[i] = 0;

	/* Now process segments as they appear: */
	do {
		mark = get_next_MK();

		switch (mark) {
		case SOF_MK:
		
			in_frame = 1;
			/* header size, don't care */
			get_size();

			/* load basic image parameters */
			FGETC();
			
			y_size = get_size();//FGETC() twice
			x_size = get_size();

			n_comp = FGETC();	

			for (i = 0; i < n_comp; i++) {
				comp[i].CID = FGETC();
				aux = FGETC();
				comp[i].HS = first_quad(aux);
				comp[i].VS = second_quad(aux);
				comp[i].QT = FGETC();
			}
		

			if (init_MCU() == -1)
				aborted_stream(1);
				

			/* dimension scan buffer for YUV->RGB conversion */
			FBuff = (FBlock *) mk_malloc(sizeof(FBlock));

			if ( (FBuff == NULL) ) {
				aborted_stream(2);
			}
			break;

		case DHT_MK:				
			if (load_huff_tables() == -1)
				aborted_stream(2);
				
			break;

		case DQT_MK:

				
			if (load_quant_tables() == -1)
				aborted_stream(3);
				
			break;

		case DRI_MK:
			get_size();	/* skip size */
			restart_interval = get_size();

			break;

		case SOS_MK:	/* lots of things to do here */

			get_size();	/* don't care */

			aux = FGETC();
			if (aux != (unsigned int)n_comp) {
			
				aborted_stream(4);
			
			}

			for (i = 0; i < n_comp; i++) {
				aux = FGETC();
				if (aux != comp[i].CID) {
				
					aborted_stream(5);
					
				}
				aux = FGETC();
				comp[i].DC_HT = first_quad(aux);
				comp[i].AC_HT = second_quad(aux);
			}
			get_size();
			FGETC();
			MCU_column = 0;
			MCU_row = 0;
			clear_bits();
			reset_prediction();

			/* main MCU processing loop here */
			if (restart_interval) {
				n_restarts = ceil_div(mx_size * my_size, restart_interval) - 1;
				leftover = mx_size * my_size - n_restarts * restart_interval;
				/* final interval may be incomplete */
				
				for (i = 0; i < n_restarts; i++) {
					for (j = 0; j < restart_interval; j++)
						process_MCU();
					/* proc till all EOB met */

					aux = get_next_MK();
					if (!RST_MK(aux))
						aborted_stream(6);

					reset_prediction();
					clear_bits();
				}	/* intra-interval loop */
			} else
				leftover = mx_size * my_size;

			/* process till end of row without restarts */
			for (i = 0; i < leftover; i++)			
				process_MCU();

			in_frame = 0;
			break;

		case EOI_MK:
			if (in_frame)
				aborted_stream(7);
			
			return 0;
			break;

		case COM_MK:
			skip_segment();
			break;

		case EOF:
			aborted_stream(8);

		default:
			if ((mark & MK_MSK) == APP_MK) {//when read from FFEC, this will hold again
				skip_segment();
				break;
			}
			if (RST_MK(mark)) {
				reset_prediction();
				break;
			}
			/* if all else has failed ... */
			aborted_stream(9);
			break;
		}		/* end switch */
	} while (1);

	return 0;
}

