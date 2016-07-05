/* File : jpeg.h, header for all jpeg code */
/* Author: Pierre Guerrier, march 1998     */
/*                                         */
/* 19/01/99  Edited by Koen van Eijk       */

/* Leave structures in memory,output something and dump core in the event
   of a failure: */
#ifndef JPEG_H
#define JPEG_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <comik.h>
#include <global_memmap.h>
#include <5kk03-utils.h>
#include <memmap.h>
#include <hw_tifu.h>
#include "c-heap.h"
#include <core_number_measure.h>

#define DEBUG 0

/*data size in DMA for reading and writing*/
 #define DMA_BUF_WORDSIZE 256
 #define DMA_BUF_BYTESIZE (DMA_BUF_WORDSIZE*4)

/*Define fifo_size for communication*/
 #define FIFO_SIZE3_4  10
 #define FIFO_SIZE4_2  5


/*----------------------------------*/
/* JPEG format parsing markers here */
/*----------------------------------*/
//#define MYCORE  0
#define SOI_MK	0xFFD8		/* start of image       */
#define APP_MK	0xFFE0		/* custom, up to FFEF */
#define COM_MK	0xFFFE		/* commment segment     */
#define SOF_MK	0xFFC0		/* start of frame       */
#define SOS_MK	0xFFDA		/* start of scan        */
#define DHT_MK	0xFFC4		/* Huffman table        */
#define DQT_MK	0xFFDB		/* Quant. table         */
#define DRI_MK	0xFFDD		/* restart interval     */
#define EOI_MK	0xFFD9		/* end of image         */
#define MK_MSK	0xFFF0

/* is x a restart interval ? */
#define RST_MK(x)	( (0xFFF8&(x)) == 0xFFD0 )

/*-------------------------------------------------------- */
/* all kinds of macros here				*/
/*-------------------------------------------------------- */

#define first_quad(c)   ((c) >> 4)	/* first 4 bits in file order */
#define second_quad(c)  ((c) & 15)

#define HUFF_ID(hclass, id)       (2 * (hclass) + (id))

#define DC_CLASS        0
#define AC_CLASS        1

/*-------------------------------------------------------*/
/* JPEG data types here					*/
/*-------------------------------------------------------*/

typedef union {			/* block of pixel-space values */
	unsigned char block[8][8];
	unsigned char linear[64];
} PBlock;

typedef union {			/* block of frequency-space values */
	int block[8][8];
	int linear[64];
} FBlock;

/* component descriptor structure */

typedef struct {
	unsigned char CID;	/* component ID */
	unsigned char IDX;	/* index of first block in MCU */

	unsigned char HS;	/* sampling factors */
	unsigned char VS;
	unsigned char HDIV;	/* sample width ratios */
	unsigned char VDIV;

	char QT;		/* QTable index, 2bits  */
	char DC_HT;		/* DC table index, 1bit */
	char AC_HT;		/* AC table index, 1bit */
	int PRED;		/* DC predictor value */
} cd_t;

/*--------------------------------------------*/
/* global variables here		      */
/*--------------------------------------------*/

/* global variables for c-heap*/
extern volatile fifo_t* producer34 ;
extern volatile fifo_t* consumer34 ;
extern volatile int* producer_data34;
extern volatile int* consumer_data34 ;
extern volatile int* fifo_sync34 ;
/* for every component, useful stuff */
extern cd_t comp[3];
/* decoded component buffer */
extern PBlock *MCU_buff[10];

/* between IDCT and color convert */
/* for every DCT block, component id then -1 */
extern int MCU_valid[10];
/* three quantization tables */
extern PBlock *QTable[4];
/* at most, but seen as four ... */
extern int QTvalid[4];

/* picture attributes */
/* Video frame size     */
extern int x_size, y_size;

/* down-rounded Video frame size */
/* in pixel units, multiple of MCU */
extern int rx_size, ry_size;

/* MCU size in pixels   */
extern int MCU_sx, MCU_sy;
/* picture size in units of MCUs */
extern int mx_size, my_size;

/* number of components 1,3 */
extern int n_comp;

/* processing cursor variables */
extern int in_frame, curcomp, MCU_row, MCU_column;
/* current position in MCU unit */

/* RGB buffer storage */
extern FBlock *FBuff;

/* process statistics */
/* number of stuff bytes in file */
extern int stuffers;
/* number of bytes skipped looking for markers */
extern int passed;

/* Counter used by FGET and FSEEK in 5kk03.c */
extern int vld_count;

/*-----------------------------------------*/
/* prototypes from utils.c		   */
/*-----------------------------------------*/

int ceil_div(int N, int D);
int floor_div(int N, int D);
void reset_prediction();
int reformat(unsigned long S, int good);
void aborted_stream(int debug);

/*-----------------------------------------*/
/* prototypes from parse.c		   */
/*-----------------------------------------*/

void clear_bits();
unsigned long get_bits(int number);
unsigned char get_one_bit();
unsigned int get_size();
unsigned int get_next_MK();
int load_quant_tables();
int init_MCU();
void skip_segment();
int process_MCU();

/*-------------------------------------------*/
/* prototypes from fast_idct.c               */
/*-------------------------------------------*/

// void IDCT(const FBlock * S, PBlock * T);

/*-----------------------------------------*/
/* prototypes from color.c		   */
/*-----------------------------------------*/

// void color_conversion();

/*-------------------------------------------*/
/* prototypes from table_vld.c or tree_vld.c */
/*-------------------------------------------*/

int load_huff_tables();
unsigned char get_symbol( int select);

/*-----------------------------------------*/
/* prototypes from huffman.c 		   */
/*-----------------------------------------*/

void unpack_block(FBlock * T, int comp);
void unpack_block_skip(FBlock * T, int comp);
/* unpack, predict, dequantize, reorder on store */

/*-----------------------------------------*/
/* prototypes from jpegtobmp.c		       */
/*-----------------------------------------*/
void write_bmp();

/*-----------------------------------------*/
/* prototypes from 5kk03.c		   */
/*-----------------------------------------*/
unsigned int FGETC();
int FSEEK(int offset, int start);

#endif
