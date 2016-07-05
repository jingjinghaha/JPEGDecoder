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
	
	unsigned int *d = (unsigned int *)(producer34->dma_data_address+sizeof(FBlock));
	unsigned int c;
	unsigned int tmp;
	tmp = vld_count % DMA_BUF_BYTESIZE;
	if ( tmp == 0)
	{
		//mk_mon_debug_info(12344321);
		while(hw_dma_status_addr((void *)producer34->dma_address));
		DMA_usage((void *)d, (void *)(jpg_from_host+vld_count / 4), DMA_BUF_WORDSIZE, (void *)producer34->dma_address);
		hw_dma_receive_addr((void *)d, (void *)(jpg_from_host+vld_count / 4), DMA_BUF_WORDSIZE, (void *)producer34->dma_address);

	}
	c = ((*(d+tmp/4))>>(8 * (tmp % 4)))& 0x00ff;
	vld_count++;
	return c;
	
	// unsigned int *d = (unsigned int *)(producer32->dma_data_address);
	// unsigned int c;
	// while(hw_dma_status_addr((void *)producer32->dma_address));
	// hw_dma_receive_addr((void *)d, (void *)(jpg_from_host+vld_count / 4), 1, (void *)producer32->dma_address);
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
}

int FTELL()
{
	return vld_count;
}
