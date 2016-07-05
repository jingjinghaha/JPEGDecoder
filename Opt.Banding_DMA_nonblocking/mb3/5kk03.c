
#include "5kk03.h"
#include "jpeg.h"
#include <hw_dma.h>
#include <5kk03-utils.h>
#include <global_memmap.h>
#include <comik.h>

// int InWordSize =  mb3_cmemout0_SIZE >> 3; // shift 2 for 32bit size, shift 1 for half the cmemout buffer
// int InByteSize =  mb3_cmemout0_SIZE >> 1;
int InWordSize =  mb3_cmemout0_SIZE >> 2; // shift 2 for 32bit size, shift 1 for half the cmemout buffer
int InByteSize =  mb3_cmemout0_SIZE;
extern unsigned int *FrameBuffer;
extern unsigned int *cmem_in_buffer0;
extern unsigned int *cmem_in_buffer1;
extern int vld_count;
extern int dma_count;


// unsigned int FGETC()
// {
	////Changed Big Endian to Little Endian!
	////unsigned int c = ((input_buffer[vld_count / 4] << (8 * (vld_count % 4))) >> 24) & 0x00ff;
	// unsigned int c = (input_buffer[vld_count / 4] >> ((vld_count & 0x3)<<3)) & 0x00ff;
	// vld_count++;
	// return c;
// }

unsigned int FGETC()//DMA_GET()
{
	unsigned int c = 0;
	//int vld_limit = vld_count &  0x7fff;
	//int vld = vld_count / 0x8000
	if(dma_count == 0)	{
		hw_dma_receive_non_block_addr	(	cmem_in_buffer0, 																	// Destination
											(unsigned int*) (shared_pt_REMOTEADDR + 4194304 + ( dma_count*InByteSize )),	// Source
											InWordSize, 																				// Block Size
											(unsigned int*) (mb3_dma0_BASEADDR) 												// DMA Address
										);
		
		/*
		dma_count++;
		hw_dma_receive_non_block_addr	(	cmem_in_buffer1, 																	// Destination
											(unsigned int*) (shared_pt_REMOTEADDR + 4194304 + ( dma_count*InByteSize )),	// Source
											InWordSize, 																				// Block Size
											(unsigned int*) (mb3_dma1_BASEADDR) 												// DMA Address
										);
		*/
		vld_count = 0;
		dma_count++;
		while(hw_dma_status_addr((void *)mb3_dma0_BASEADDR));
		//while(hw_dma_status_addr((void *)mb3_dma1_BASEADDR));
	}		
	else if (vld_count >= InByteSize)	{
		//if((dma_count%2) == 0) {
			hw_dma_receive_non_block_addr	(	cmem_in_buffer0, 																	// Destination
												(unsigned int*) (shared_pt_REMOTEADDR + 4194304 + ( dma_count*InByteSize )),	// Source
												InWordSize, 																				// Block Size
												(unsigned int*) (mb3_dma0_BASEADDR) 												// DMA Address
											);
			while(hw_dma_status_addr((void *)mb3_dma0_BASEADDR));
		/*
		}
		else if((dma_count%2) == 1)	{
			hw_dma_receive_non_block_addr	(	cmem_in_buffer1, 																	// Destination
												(unsigned int*) (shared_pt_REMOTEADDR + 4194304 + ( dma_count*InByteSize )),	// Source
												InWordSize, 																				// Block Size
												(unsigned int*) (mb3_dma1_BASEADDR) 												// DMA Address
											);
			while(hw_dma_status_addr((void *)mb3_dma1_BASEADDR));								
		}*/
		vld_count = 0;
		dma_count++;
	}
	
	//if((dma_count%2) == 0) {
		c = (cmem_in_buffer0[vld_count >> 2] >> ((vld_count & 0x3)<<3)) & 0x00ff;
		vld_count++;
	/*
	}
	else if((dma_count%2) == 1)	{
		c = (cmem_in_buffer1[vld_count >> 2] >> ((vld_count & 0x3)<<3)) & 0x00ff;
		vld_count++;	
	}	*/	
		
	return c;
}

// int FSEEK(int offset, int start)
// {

	// vld_count += offset + (start - start);	/* Just to use start... */
	
	// return 0;
// }

int FSEEK(int offset, int start)//DMA_SEEK(int offset, int start)
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
