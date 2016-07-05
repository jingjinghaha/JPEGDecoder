#include "jpeg.h"
/* real declaration of global variables here */
/* see jpeg.h for more info			*/
// global variables related to the C-HEAP FIFO
volatile fifo_t* producer34 = (fifo_t*) mb3_cmemin1_BASEADDR;
volatile fifo_t* consumer34 = (fifo_t*) mb4_cmemin1_pt_REMOTEADDR;
volatile int* producer_data34 = (int*) (mb4_cmemin1_pt_REMOTEADDR + 1 * sizeof(fifo_t));
volatile int* consumer_data34 = (int*) (mb4_cmemin1_BASEADDR + 1 * sizeof(fifo_t));
// Note that producer_data and consumer_data point to the same place, although the producer and consumer
// may have different addresses to address it, since it is remote for the producer, but local for the consumer.
// synchronization variable that indicates that C-HEAP FIFO is initialized.
volatile int* fifo_sync34 = (int*) (mb4_cmemin1_pt_REMOTEADDR + 1 * sizeof(fifo_t) + FIFO_SIZE3_4 * sizeof(FBlock));

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

/* MCU after color conversion */
unsigned char *ColorBuffer;
/* complete final RGB image */
unsigned char *FrameBuffer;
/* scratch frequency buffer */
FBlock *FBuff;
/* scratch pixel buffer */
PBlock *PBuff;
/* frame started ? current component ? */
int in_frame, curcomp;
/* current position in MCU unit */
int MCU_row, MCU_column;

/* input  File stream pointer   */
//FILE *fi;

/* stuff bytes in entropy coded segments */
int stuffers = 0;
/* bytes passed when searching markers */
int passed = 0;

int verbose = 0;

/* Extra global variables for 5kk03 */

int vld_count = 0;		/* Counter used by FGET and FSEEK in 5kk03.c */
//unsigned int input_buffer[JPGBUFFER_SIZE / sizeof(int)];
//int DMA_count = 0;
/* End extra global variables for 5kk03 */

int JpegToBmp();
//volatile FrameBuffer_t *bmp_buffer = (FrameBuffer_t*)shared_pt_REMOTEADDR;
volatile unsigned char *bmp_buffer = (unsigned char*)shared_pt_REMOTEADDR;
int main (void)
{
	// Sync with the monitor.
	mk_mon_sync();
	// Enable stack checking.
	start_stack_check();
	begin_use();
	// initialize FIFO
	cheap_init_fifo(
			producer34,
			consumer34,
			sizeof(FBlock), // Token size
			FIFO_SIZE3_4,  // fifo size
			(unsigned int)producer_data34,
			(unsigned int)consumer_data34,
#ifdef USE_DMA
			mb3_dma1_BASEADDR,
			mb3_cmemout1_BASEADDR+sizeof(int),
			mb4_dma1_BASEADDR,
			mb4_cmemout1_BASEADDR);
#else
			0,
			0,
			0,
			0);
#endif
#ifdef USE_DMA
	unsigned int *f = (unsigned int *)mb3_cmemout1_BASEADDR;	
	*f =1 ;
	DMA_usage((void*)fifo_sync34, (void*)f, 1, (void*)mb3_dma1_BASEADDR);
	hw_dma_send_addr((void*)fifo_sync34, (void*)f, 1, (void*)mb3_dma1_BASEADDR);
	
#else
	// signal to core 2 that the FIFO is initialized and can be read from.
	*fifo_sync32 = 1;
#endif

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
	// Signal the monitor we are done.
	mk_mon_debug_tile_finished();
	return 0;
}
/*-----------------------------------------------------------------*/
/*		MAIN		MAIN		MAIN		   */
/*-----------------------------------------------------------------*/

int JpegToBmp()
{
	unsigned int aux, mark;
	int n_restarts, restart_interval, leftover;	/* RST check */
	int i, j;
	//int temp = 0;
	

	/* First find the SOI marker: */
	//mk_mon_debug_info(9999);
	aux = get_next_MK();
	if (aux != SOI_MK)
		aborted_stream(0);
		

	//if (verbose)
		//fprintf(stderr, "%ld:\tINFO:\tFound the SOI marker!\n", ftell(fi));
		//;
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
			//fgetc(fi);	/* precision, 8bit, don't care */
			FGETC();
			
			y_size = get_size();//FGETC() twice
			x_size = get_size();
			mk_mon_debug_info(x_size);
			mk_mon_debug_info(y_size);
	
			n_comp = FGETC();
			mk_mon_debug_info(123456);
			mk_mon_debug_info(n_comp);
			mk_mon_debug_info(654321);
	

			for (i = 0; i < n_comp; i++) {
				/* component specifiers */
				//comp[i].CID = fgetc(fi);
				comp[i].CID = FGETC();
				//aux = fgetc(fi);
				aux = FGETC();
				comp[i].HS = first_quad(aux);
				comp[i].VS = second_quad(aux);
				//comp[i].QT = fgetc(fi);
				comp[i].QT = FGETC();
			}
		

			if (init_MCU() == -1)
				aborted_stream(1);
				

			/* dimension scan buffer for YUV->RGB conversion */
			
			ColorBuffer = (unsigned char *)mk_malloc((size_t) MCU_sx * MCU_sy * n_comp);
			FBuff = (FBlock *) mk_malloc(sizeof(FBlock));
			PBuff = (PBlock *) mk_malloc(sizeof(PBlock));

			if (  (ColorBuffer == NULL) || (FBuff == NULL) || (PBuff == NULL)) {
				//fprintf(stderr, "\tERROR:\tCould not allocate pixel storage!\n");
				aborted_stream(2);
			}
			break;

		case DHT_MK:
			//if (verbose)
				//fprintf(stderr, "%ld:\tINFO:\tDefining Huffman Tables\n", ftell(fi));
				
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
			mk_mon_debug_info(00000000);
			mk_mon_debug_info(restart_interval);
			mk_mon_debug_info(00000000);

			break;

		case SOS_MK:	/* lots of things to do here */

			get_size();	/* don't care */

			aux = FGETC();
			if (aux != (unsigned int)n_comp) {
			
				aborted_stream(4);
			
			}

			for (i = 0; i < n_comp; i++) {
				//aux = fgetc(fi);
				aux = FGETC();
				if (aux != comp[i].CID) {
				
					aborted_stream(5);
					
				}
				//aux = fgetc(fi);
				aux = FGETC();
				comp[i].DC_HT = first_quad(aux);
				comp[i].AC_HT = second_quad(aux);
			}
			get_size();
			//fgetc(fi);	/* skip things */
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
					//temp = i * restart_interval;
					for (j = 0; j < restart_interval; j++)
						process_MCU();
					/* proc till all EOB met */

					aux = get_next_MK();
					if (!RST_MK(aux)) {
						//fprintf(stderr, "%ld:\tERROR:\tLost Sync after interval!\n", ftell(fi));
						aborted_stream(6);
						
					} else if (verbose);
						//fprintf(stderr, "%ld:\tINFO:\tFound Restart Marker\n", ftell(fi));

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
			//if (verbose);
				//fprintf(stderr, "%ld:\tINFO:\tFound the EOI marker!\n", ftell(fi));
			if (in_frame)
				aborted_stream(7);

			//if (verbose);
				/*fprintf(stderr, "\tINFO:\tTotal skipped bytes %d, total stuffers %d\n", passed,
					stuffers);*/
			//fclose(fi);
			
	//mk_mon_debug_info(8888);
			//write_bmp();
		//	mk_mon_debug_info(6666);
			//printf("%ld,%ld", x_size * y_size * n_comp,MCU_sx * MCU_sy * n_comp);
			
			//free_structures();
			return 0;
			break;

		case COM_MK:
			//if (verbose);
				//fprintf(stderr, "%ld:\tINFO:\tSkipping comments\n", ftell(fi));
			skip_segment();
			break;

		case EOF:
			//if (verbose);
				//fprintf(stderr, "%ld:\tERROR:\tRan out of input data!\n", ftell(fi));
			aborted_stream(8);

		default:
			if ((mark & MK_MSK) == APP_MK) {//when read from FFEC, this will hold again
				//if (verbose);
					//fprintf(stderr, "%ld:\tINFO:\tSkipping application data\n", ftell(fi));
				skip_segment();
				break;
			}
			if (RST_MK(mark)) {
				reset_prediction();
				break;
			}
			/* if all else has failed ... */
			//fprintf(stderr, "%ld:\tWARNING:\tLost Sync outside scan, %d!\n", ftell(fi), mark);
			aborted_stream(9);
			break;
		}		/* end switch */
	} while (1);

	return 0;
}

/* void write_bmp()
{
	//FILE *fpBMP;

	int i, j;
	int kk;
//mk_mon_debug_info(7777);


	for (i = y_size - 1; i >= 0; i--) {
		//in bitmaps the bottom line of the image is at the
		  // beginning of the file 
		for (j = 0; j < x_size; j++) {
			bmp_buffer[i*1024 + j].B = FrameBuffer[3 * (i * x_size + j) + 0];
			bmp_buffer[i*1024 + j].G = FrameBuffer[3 * (i * x_size + j) + 1];
			bmp_buffer[i*1024 + j].R = FrameBuffer[3 * (i * x_size + j) + 2];
			//putc(FrameBuffer[3 * (i * x_size + j) + 0], fpBMP);
			//putc(FrameBuffer[3 * (i * x_size + j) + 1], fpBMP);
			//putc(FrameBuffer[3 * (i * x_size + j) + 2], fpBMP);
		}
		//for (j = 0; j < x_size % 4; j++)
			//putc(0, fpBMP);
	}

	//fclose(fpBMP);
} */
