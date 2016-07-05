#include "jpeg.h"
/* real declaration of global variables here */
/* see jpeg.h for more info			*/


volatile fifo_t* producer32 = (fifo_t*) mb3_cmemin0_pt_REMOTEADDR;
volatile fifo_t* consumer32 = (fifo_t*) mb2_cmemin0_BASEADDR;
volatile fifo_t* producer24 = (fifo_t*) (mb2_cmemin0_BASEADDR + 1 * sizeof(fifo_t) + FIFO_SIZE3_2 * sizeof(FBlock) + 4);
volatile fifo_t* consumer24 = (fifo_t*) mb4_cmemin0_pt_REMOTEADDR;
volatile int* producer_data24 = (int*) (mb4_cmemin0_pt_REMOTEADDR + 1 * sizeof(fifo_t));
volatile int* consumer_data24 = (int*) (mb4_cmemin0_BASEADDR + 1 * sizeof(fifo_t));
volatile int* fifo_sync32 = (int*) (mb2_cmemin0_BASEADDR + 1 * sizeof(fifo_t) + FIFO_SIZE3_2 * sizeof(FBlock));
volatile int* fifo_sync24 = (int*) (mb4_cmemin0_pt_REMOTEADDR + 1 * sizeof(fifo_t) + FIFO_SIZE2_4 * sizeof(PBlock));
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

int vld_count = 0;	
//int DMA_count = 0;	/* Counter used by FGET and FSEEK in 5kk03.c */
//unsigned int input_buffer[JPGBUFFER_SIZE / sizeof(int)];

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
	// initialize FIFO
	cheap_init_fifo(
			producer24,
			consumer24,
			sizeof(PBlock), // Token size
			5,  // fifo size
			(unsigned int)producer_data24,
			(unsigned int)consumer_data24,
#ifdef USE_DMA
			mb2_dma0_BASEADDR,
			mb2_cmemout0_BASEADDR+sizeof(int),
			mb4_dma0_BASEADDR,
			mb4_cmemout0_BASEADDR);
#else
			0,
			0,
			0,
			0);
#endif
	while (*fifo_sync32 == 0) {}
#ifdef USE_DMA
	unsigned int *f = (unsigned int *)mb2_cmemout0_BASEADDR+sizeof(int);	
	*f =1 ;
	hw_dma_send_addr((void*)fifo_sync24, (void*)f, 1, (void*)mb2_dma0_BASEADDR);
	
#else
	// signal to core 2 that the FIFO is initialized and can be read from.
	*fifo_sync24 = 1;
#endif

	begin_use();
    JpegToBmp();
	end_use();

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
		case SOF_MK:  //ffc0 start of the frame
			//if (verbose);
				//fprintf(stderr, "%ld:\tINFO:\tFound the SOF marker!\n", ftell(fi));
			in_frame = 1;
			//get_size(fi);	/* header size, don't care */
			get_size();   //0011 17

			/* load basic image parameters */
			//fgetc(fi);	/* precision, 8bit, don't care */
			FGETC();   //8
			
			y_size = get_size();//FGETC() twice
			x_size = get_size();

			//if (verbose);
				//fprintf(stderr, "\tINFO:\tImage size is %d by %d\n", x_size, y_size);

			//n_comp = fgetc(fi);	/* # of components */
			n_comp = FGETC();

			//if (1) {
				//fprintf(stderr, "\tINFO:\t");
				////switch (n_comp) {
				//case 1:
					//printf( "Monochrome\n");
					//break;
				//case 3:
					//printf( "Color\n");
				//	break;
				////default:
				//	printf( "Not a picture!\n");
					//break;
				//}
				//fprintf(stderr, " JPEG image!\n");
			//}

			for (i = 0; i < n_comp; i++) {
				/* component specifiers */
				//comp[i].CID = fgetc(fi);
				comp[i].CID = FGETC();
				//aux = fgetc(fi);
				aux = FGETC();
				comp[i].HS = first_quad(aux); //0x11 >> 4   1
				comp[i].VS = second_quad(aux);  //&15       1
				//comp[i].QT = fgetc(fi);
				comp[i].QT = FGETC();
			}
			//if ((n_comp > 1) && verbose);
				/*fprintf(stderr,
					"\tINFO:\tColor format is %d:%d:%d, H=%d\n",
					comp[0].HS * comp[0].VS, comp[1].HS * comp[1].VS, comp[2].HS * comp[2].VS,
					comp[1].HS);*/
					

			if (init_MCU() == -1)
				aborted_stream(1);
				

			/* dimension scan buffer for YUV->RGB conversion */
			//FrameBuffer = (unsigned char *)mk_malloc((size_t) x_size * y_size * n_comp);
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

		case DQT_MK:  //FFDB, the following 0084 shows the table length 132 
			//if (verbose)
				//fprintf(stderr, "%ld:\tINFO:\tDefining Quantization Tables\n", ftell(fi));
				
			if (load_quant_tables() == -1)
				aborted_stream(3);
				
			break;

		case DRI_MK:
			get_size();	/* skip size */
			restart_interval = get_size();

			//if (verbose)
				//fprintf(stderr, "%ld:\tINFO:\tDefining Restart Interval %d\n", ftell(fi),
					//restart_interval);
					//;
			break;

		case SOS_MK:	/* lots of things to do here */ //ffda
			//if (verbose);
				//fprintf(stderr, "%ld:\tINFO:\tFound the SOS marker!\n", ftell(fi));
			get_size();	/* don't care */
			//aux = fgetc(fi);
			aux = FGETC();  //03
			if (aux != (unsigned int)n_comp) {
				//fprintf(stderr, "\tERROR:\tBad component interleaving!\n");
				aborted_stream(4);
			
			}

			for (i = 0; i < n_comp; i++) {
				//aux = fgetc(fi);
				aux = FGETC();
				if (aux != comp[i].CID) {
					//fprintf(stderr, "\tERROR:\tBad Component Order!\n");
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
					if (!RST_MK(aux)) { // restart marker (FFD0-FFD7) #define RST_MK(x)	( (0xFFF8&(x)) == 0xFFD0 )
						//fprintf(stderr, "%ld:\tERROR:\tLost Sync after interval!\n", ftell(fi));
						aborted_stream(6);
						
					} else if (verbose);
						//fprintf(stderr, "%ld:\tINFO:\tFound Restart Marker\n", ftell(fi));

					reset_prediction();
					clear_bits();
				}	/* intra-interval loop */
			} else
			leftover = mx_size * my_size; //picture size in units of MCUs 
			//MCU_column = leftover/2 % mx_size;
			//MCU_row = leftover / 2 /mx_size;
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

		case COM_MK: //ffee
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
