/*-----------------------------------------*/
/* File : parse.c, utilities for jfif view */
/* Author : Pierre Guerrier, march 1998	   */
/*-----------------------------------------*/


//#include <string.h>

#include "jpeg.h"

/*---------------------------------------------------------------------*/

/* utility and counter to return the number of bits from file */
/* right aligned, masked, first bit towards MSB's		*/

static unsigned char bit_count;	/* available bits in the window */
static unsigned char window;
static int parts[4] = {0,0,0,0};

unsigned long get_bits(int number)
{
	int i, newbit;
	unsigned long result = 0;
	unsigned char aux, wwindow;

	if (!number)
		return 0;

	for (i = 0; i < number; i++) {
		if (bit_count == 0) {
			wwindow = FGETC();

			if (wwindow == 0xFF)
				switch (aux = FGETC()) {	/* skip stuffer 0 byte */
				case 0xFF:
					break;

				case 0x00:
					stuffers++;
					break;

				default:
					break;
				}

			bit_count = 8;
		} else
			wwindow = window;
		newbit = (wwindow >> 7) & 1;
		window = wwindow << 1;
		bit_count--;
		result = (result << 1) | newbit;
	}
	return result;
}

void clear_bits(void)
{
	bit_count = 0;
}

unsigned char get_one_bit()
{
	int newbit;
	unsigned char aux, wwindow;

	if (bit_count == 0) {
		wwindow = FGETC();

		if (wwindow == 0xFF)
			switch (aux = FGETC()) {	/* skip stuffer 0 byte */
			case 0xFF:
				break;

			case 0x00:
				stuffers++;
				break;

			default:
				break;
			}

		bit_count = 8;
	} else
		wwindow = window;

	newbit = (wwindow >> 7) & 1;
	window = wwindow << 1;
	bit_count--;
	return newbit;
}

/*----------------------------------------------------------*/

unsigned int get_size()
{
	unsigned char aux;

	//aux = fgetc(fi);
	aux = FGETC();
	//return (aux << 8) | fgetc(fi);	/* big endian */
	return (aux << 8) | FGETC();
}

/*----------------------------------------------------------*/

void skip_segment()
{				/* skip a segment we don't want */
	unsigned int size;
	char tag[5];
	int i;

	size = get_size();//0x0010, 16 in decimal , 0x0011 17 second time 
	if (size > 5) {
		for (i = 0; i < 4; i++)
			tag[i] = FGETC();//4A 46 49 46  00 01 02 00    00 64 00 64 
							 //4475636B 79000100 04000000 3C0000
		tag[4] = '\0';
		size -= 4;// get four chars and then substract 4 from size, then size-2 + 2(at the beginning) =size remaining
	}
	FSEEK(size-2,SEEK_CUR);//0x FFEC0011
}

/*----------------------------------------------------------------*/
/* find next marker of any type, returns it, positions just after */
/* EOF instead of marker if end of file met while searching ...	  */
/*----------------------------------------------------------------*/

unsigned int get_next_MK()
{
	unsigned int c;
	int ffmet = 0;
	int locpassed = -1;

	passed--;		/* as we fetch one anyway */

	while ((c = FGETC()) != (unsigned int)EOF) {
		switch (c) {
		case 0xFF:
			ffmet = 1;
			break;
		case 0x00:
			ffmet = 0;
			break;
		default:
			if (ffmet)
				return (0xFF00 | c);
			ffmet = 0;
			break;
		}
		locpassed++;
		passed++;
	}

	return (unsigned int)EOF;
}

/*----------------------------------------------------------*/
/* loading and allocating of quantization table             */
/* table elements are in ZZ order (same as unpack output)   */
/*----------------------------------------------------------*/

int load_quant_tables()
{
	char aux;
	unsigned int size, n, i, id, x;

	size = get_size();	/* this is the tables' size */
	n = (size - 2) / 65;

	for (i = 0; i < n; i++) {
		aux = FGETC();
		if (first_quad(aux) > 0) {
			return -1;
		}
		id = second_quad(aux);
		QTable[id] = (PBlock *) mk_malloc(sizeof(PBlock));
		QTvalid[id] = 1;
		for (x = 0; x < 64; x++)
			QTable[id]->linear[x] = FGETC();
	}
	return 0;
}

/*----------------------------------------------------------*/
/* initialise MCU block descriptors	                    */
/*----------------------------------------------------------*/

int init_MCU(void)
{
	int i, j, k, n, hmax = 0, vmax = 0;

	for (i = 0; i < 10; i++)
		MCU_valid[i] = -1;

	k = 0;

	for (i = 0; i < n_comp; i++) {
		if (comp[i].HS > hmax)
			hmax = comp[i].HS;
		if (comp[i].VS > vmax)
			vmax = comp[i].VS;
		n = comp[i].HS * comp[i].VS;

		comp[i].IDX = k;
		for (j = 0; j < n; j++) {
			MCU_valid[k] = i;
			MCU_buff[k] = (PBlock *) mk_malloc(sizeof(PBlock));
			k++;
			if (k == 10) {
				return -1;
			}
		}
	}

	MCU_sx = hmax<<3;
	MCU_sy = vmax<<3;
	for (i = 0; i < n_comp; i++) {
		comp[i].HDIV = (hmax / comp[i].HS > 1);	/* if 1 shift by 0 */
		comp[i].VDIV = (vmax / comp[i].VS > 1);	/* if 2 shift by one */
	}

	mx_size = ceil_div(x_size, MCU_sx);
	my_size = ceil_div(y_size, MCU_sy);
	rx_size = MCU_sx * floor_div(x_size, MCU_sx);
	ry_size = MCU_sy * floor_div(y_size, MCU_sy);

	if((y_size <= 1)) {							
		parts[0] = 0; 	parts[1] = 0; 			parts[2] = 0; 					parts[3] = 0;				
	} else if( (x_size <= 1) && (y_size > 1)) {	
		parts[0] = 0; 		parts[1] = (3*my_size/16); 			parts[2] = (6*my_size/16); 		parts[3] = (11*my_size/16); 	
	} else if( (x_size > 1) && (y_size > 1)) {	
		parts[0] = 0; 			parts[1] = ((9*my_size)/32); 	parts[2] = ((16*my_size)/32); 	parts[3] = ((25*my_size)/32);	
	}
	
	return 0;
}

/*----------------------------------------------------------*/
/* this takes care for processing all the blocks in one MCU */
/*----------------------------------------------------------*/

int process_MCU(int turn)
{
	int i,j;
	int goodrows, goodcolumns;
	int row_count = 0;
	int col_count = 0;

	if (MCU_column == mx_size) {
		MCU_column = 0;
		MCU_row++;
		if (MCU_row == my_size) {
			in_frame = 0;
			return 0;
		}
	}
	
	if (MCU_row >= parts[3])
	{	
		for (curcomp = 0; MCU_valid[curcomp] != -1; curcomp++) {
			unpack_block( FBuff, MCU_valid[curcomp]);	/* pass index to HT,QT,pred */
			IDCT(FBuff, MCU_buff[curcomp]);
		}

		/* cut last row/column as needed */
		if ((y_size != ry_size) && (MCU_row == (my_size - 1)))
			goodrows = y_size - ry_size;
		else
			goodrows = MCU_sy;

		if ((x_size != rx_size) && (MCU_column == (mx_size - 1)))
			goodcolumns = x_size - rx_size;
		else
			goodcolumns = MCU_sx;

 		/* YCrCb to RGB color space transform here */
		color_conversion( goodrows, goodcolumns );

		col_count = MCU_column*MCU_sx;
		row_count = MCU_row*MCU_sy*1024;

			for (i = 0; i < goodrows; i++) 
			{
				for (j = 0; j < goodcolumns; j++) {
					 cmem_out_buffer[j] = (ColorBuffer[(i * MCU_sx) + j]);
				}
				hw_dma_send_non_block_addr( 	(void*) (shared_pt_REMOTEADDR+((col_count*4)+(i*1024*4)+(row_count*4))),
									(void*) cmem_out_buffer,
									goodcolumns,
									(void*) (mb4_dma1_BASEADDR)
								);
			}
	}
	else
	for (curcomp = 0; MCU_valid[curcomp] != -1; curcomp++) {
			unpack_block_skip( FBuff, MCU_valid[curcomp]);
	}

	MCU_column++;
	return 1;


}
