//#include <stdio.h>


#include "5kk03.h"
#include "jpeg.h"
#include "surfer.jpg.h"

#define From_host
//extern unsigned int input_buffer[JPGBUFFER_SIZE / sizeof(int)];
extern int vld_count;
volatile unsigned int *jpg_from_host = (unsigned int*)(shared_pt_REMOTEADDR+4*1024*1024);
#ifndef From_host
unsigned int FGETC()
{
	
	unsigned int c = ((surfer_jpg[vld_count / 4] << (8 * (vld_count % 4))) >> 24) & 0x00ff;
	vld_count++;
	return c;
}
#else
#ifndef USE_DMA
unsigned int FGETC()
{
	
	unsigned int c = ((jpg_from_host[vld_count / 4] >> (8 * (vld_count % 4))) ) & 0x00ff;
	vld_count++;
	return c;
}
#else
unsigned int FGETC()
{
	//here we use offset as sizeof(int) does not matter, that shows when dealing with communication and dealing with FGETC do not overlap
	unsigned int *d = (unsigned int *)(consumer34->dma_data_address+sizeof(int) );
	unsigned int c;
	unsigned int tmp;
	tmp = vld_count% DMA_BUF_BYTESIZE;
	if ( tmp == 0)
	{
		//mk_mon_debug_info(12344321);
		while(hw_dma_status_addr((void *)consumer34->dma_address));
		DMA_usage((void *)d, (void *)(jpg_from_host+vld_count / 4), DMA_BUF_WORDSIZE, (void *)consumer34->dma_address);
		hw_dma_receive_addr((void *)d, (void *)(jpg_from_host+vld_count / 4), DMA_BUF_WORDSIZE, (void *)consumer34->dma_address);

	}
	
	c = ((*(d+tmp/4))>>(8 * (tmp % 4)))& 0x00ff;
	vld_count++;
	return c;
	
	// unsigned int *d = (unsigned int *)(consumer24->dma_data_address);
	// unsigned int c;
	// while(hw_dma_status_addr((void *)consumer24->dma_address));
	// hw_dma_receive_addr((void *)d, (void *)(jpg_from_host+vld_count / 4), 1, (void *)consumer24->dma_address);
	// c = ((*d)>>(8 * (vld_count % 4)))& 0x00ff;
	// vld_count++;
	// return c;
}
#endif
#endif
int FSEEK(int offset, int start)
{
	unsigned i = 0;
	while ( i< offset)
	{
		FGETC();
		i++;
	
	}
	return 0;
	//vld_count += offset + (start - start);	/* Just to use start... */
	
}

int FTELL()
{
	return vld_count;
}
