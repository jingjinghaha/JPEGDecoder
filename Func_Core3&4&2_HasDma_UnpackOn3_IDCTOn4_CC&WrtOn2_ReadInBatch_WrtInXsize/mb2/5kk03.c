//#include <stdio.h>


#include "5kk03.h"
#include "jpeg.h"

//extern unsigned int input_buffer[JPGBUFFER_SIZE / sizeof(int)];
extern int vld_count;
volatile unsigned int *jpg_from_host = (unsigned int*)(shared_pt_REMOTEADDR+4*1024*1024);

unsigned int FGETC()
{
	unsigned int *d = (unsigned int *)(consumer42->dma_data_address+sizeof(int));
	unsigned int c;
	unsigned int tmp;
	tmp = vld_count% DMA_BUF_BYTESIZE;
	if ( tmp == 0)
	{
		//mk_mon_debug_info(12344321);
		while(hw_dma_status_addr((void *)consumer42->dma_address));
		hw_dma_receive_addr((void *)d, (void *)(jpg_from_host+vld_count / 4), DMA_BUF_WORDSIZE, (void *)consumer42->dma_address);
	}
	
	c = ((*(d+tmp/4))>>(8 * (tmp % 4)))& 0x00ff;
	vld_count++;
	return c;
}

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
	return 0;
}

int FTELL()
{
	return vld_count;
}
