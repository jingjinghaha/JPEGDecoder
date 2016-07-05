#include <comik.h>
   #include "memory.h"
   #include <global_memmap.h> /*Memory Address*/
   #include <string.h> /* memset */
   #include <5kk03-utils.h>
   
   #define DDR_SIZE_PER_CORE		0x600000
   #define MASK_BASE_DDR 				(shared_pt_REMOTEADDR + 0)
   #define INPUT_OFFSET				(4*1024*1024)
   
   /* Defines for Core 1*/
   #ifdef MB1
       #define DMA_CMEM_OUT 		mb1_cmemout0_BASEADDR
       #define DMA_BASE_ADDR		mb1_dma0_BASEADDR
       #define DMA_OUT_SIZE			mb1_cmemout0_SIZE
   /* Defines for Core 2 */
   #elif MB2
       #define DMA_CMEM_OUT 		mb2_cmemout0_BASEADDR
       #define DMA_BASE_ADDR		mb2_dma0_BASEADDR
       #define DMA_OUT_SIZE			mb2_cmemout0_SIZE
   /* Defines for Core 3 */
   #elif MB3
       #define DMA_CMEM_OUT 		mb3_cmemout0_BASEADDR
       #define DMA_BASE_ADDR		mb3_dma0_BASEADDR
       #define DMA_OUT_SIZE			mb3_cmemout0_SIZE
   /* Defines for Core 4 */
   #elif MB4
       #define DMA_CMEM_OUT 		mb4_cmemout0_BASEADDR
       #define DMA_BASE_ADDR		mb4_dma0_BASEADDR
       #define DMA_OUT_SIZE			mb4_cmemout0_SIZE
   #endif
   
   int count = 0;
   extern unsigned int __stack;
   extern unsigned int _stack;
   extern unsigned int _stack_end;
   
   void benchmark_malloc_max_usage()
   {
   #if BENCHMARK_EN
       mk_mon_debug_info(MALLOC_MAX_INFO);
       mk_mon_debug_info(mk_malloc_max_usage());
   #endif
   }
   
   void benchmark_malloc_usage()
   {
   #if BENCHMARK_EN
       mk_mon_debug_info(MALLOC_USE_INFO);
       mk_mon_debug_info(mk_malloc_usage());
   #endif
   }
   
   void benchmark_malloc_num_mallocs()
   {
   #if BENCHMARK_EN
       mk_mon_debug_info(MALLOC_NUM_MALLOCS_INFO);
       mk_mon_debug_info(mk_malloc_num_mallocs());
   #endif
   }
   
   void benchmark_malloc_num_malloc_free()
   {
   #if BENCHMARK_EN
       mk_mon_debug_info(MALLOC_NUM_FREES_INFO);
       mk_mon_debug_info(mk_malloc_num_frees());
   #endif
   }
   
   /*--------------------------Communication Memory Start----------------------------*/
   
   void benchmark_cmem_pattern()
   {
   #if BENCHMARK_EN
   #ifdef MB1
   	unsigned char *pointer = (unsigned char*)mb1_cmemin0_BASEADDR;
   	memset(pointer, 0xAA, mb1_cmemout0_SIZE+mb1_cmemin0_SIZE);
   #endif
   #ifdef MB2
   	unsigned char *pointer = (unsigned char*)mb2_cmemin0_BASEADDR;
   	memset(pointer, 0xAA, mb2_cmemout0_SIZE+mb2_cmemin0_SIZE);
   #endif
   #ifdef MB3
   	unsigned char *pointer = (unsigned char*)(mb3_cmemin0_BASEADDR);
   	memset(pointer, 0xAA, mb3_cmemout0_SIZE+mb3_cmemout1_SIZE+mb3_cmemin0_SIZE+mb3_cmemin1_SIZE);
   #endif
   #ifdef MB4
   	unsigned char *pointer = (unsigned char*)mb4_cmemin0_BASEADDR;
   	memset(pointer, 0xAA, mb4_cmemout0_SIZE+mb4_cmemout1_SIZE+mb4_cmemin0_SIZE+mb4_cmemin1_SIZE);
   #endif
   #endif
   }
   
   void benchmark_cmem_usage()
   {
   #if BENCHMARK_EN
   	int i = 0, cmem_usage = 0;
   
   #ifdef MB1
   	unsigned char *cmem_pointer = (unsigned char*)mb1_cmemin0_BASEADDR;
    	for(i=0; i<mb1_cmemin0_SIZE+mb1_cmemout0_SIZE; i += 1){
   		cmem_pointer = (unsigned char*)(mb1_cmemin0_BASEADDR + i);
   		if( (*cmem_pointer) != 0xAA)
   			cmem_usage++;
   	}
   #endif
   #ifdef MB2
   	unsigned char *cmem_pointer = (unsigned char*)mb2_cmemin0_BASEADDR;
   	for(i=0; i<mb2_cmemin0_SIZE+mb2_cmemout0_SIZE; i += 1){
   		cmem_pointer = (unsigned char*)(mb2_cmemin0_BASEADDR + i);
   		if( (*cmem_pointer) != 0xAA)
   			cmem_usage++;
   	}
   #endif	
   #ifdef MB3
   	unsigned char *cmem_pointer = (unsigned char*)mb3_cmemin0_BASEADDR;
   	for(i=0; i<mb3_cmemout0_SIZE+mb3_cmemout1_SIZE+mb3_cmemin0_SIZE+mb3_cmemin1_SIZE; i += 1){
   		cmem_pointer = (unsigned char*)(mb3_cmemin0_BASEADDR + i);
   		if( (*cmem_pointer) != 0xAA)
   			cmem_usage++;
   	}
   #endif
   #ifdef MB4
   	unsigned char *cmem_pointer = (unsigned char*)mb4_cmemin0_BASEADDR;
   	for(i=0; i<mb4_cmemout0_SIZE+mb4_cmemout1_SIZE+mb4_cmemin0_SIZE+mb4_cmemin1_SIZE; i += 1){
   		cmem_pointer = (unsigned char*)(mb4_cmemin0_BASEADDR + i);
   		// mk_mon_debug_info(i); 
   		if( (*cmem_pointer) != 0xAA)
   			cmem_usage++;
   	}
   #endif
   
   	mk_mon_debug_info(CMEM_USAGE); 
   	mk_mon_debug_info(cmem_usage); 
   #endif
   }
   
   /*--------------------------Communication Memory End----------------------------*/
   
   void mask_DDR()
   {
   #if BENCHMARK_EN
       int i;
       int * pt_ddr = (int *) MASK_BASE_DDR;
   #ifdef USE_DMA
       volatile int * out = (int *) DMA_CMEM_OUT;
       for (i = 0; i < DMA_OUT_SIZE/4; i++)
           out[i] = MASK_VALUE;
       for (i = 0; i < INPUT_OFFSET/4; i += DMA_OUT_SIZE/4)
           hw_dma_send_addr((int *)(pt_ddr + i), out, DMA_OUT_SIZE/4, (void *)DMA_BASE_ADDR);
   #else
       for (i = 0; i < INPUT_OFFSET/4; i++)
       {
           pt_ddr[i] = MASK_VALUE;
       }
   #endif
   #endif
   }
   
   void readback_DDR()
   {
   #if BENCHMARK_EN
       int i;
       int j;
       volatile int * pt_ddr = (int *) MASK_BASE_DDR;
   #ifdef USE_DMA
       int * in = (int *) DMA_CMEM_OUT;
       for (i = 0; i < DDR_SIZE_PER_CORE/4; )
       {
           hw_dma_receive_addr(in, (int*)(pt_ddr + i), DMA_OUT_SIZE/4, (void *)DMA_BASE_ADDR);
           for (j = 0; j < DMA_OUT_SIZE/4; j++)
           {
               if (in[j] != MASK_VALUE)
                   count++;
           }
           i += (DMA_OUT_SIZE/4);
       }
   #else
       for (i = 0; i < DDR_SIZE_PER_CORE/4; i++)
       {
           if (pt_ddr[i] != MASK_VALUE)
               count++;
       }
   #endif
   #endif
   }
   
   void print_DDR_usage()
   {
   #if BENCHMARK_EN
       mk_mon_debug_info(DDR_USAGE_INFO);
       mk_mon_debug_info(count*4 - 16);
   #endif
   }
       
   /* ------------------------------------ Stack memory --------------------------------------*/
       
   unsigned int st_top=&__stack;
   unsigned int st_bottom=&_stack_end;
   void paint_stack()
   {
   #if BENCHMARK_EN
       
       
   int *painter = (unsigned int ) st_bottom;
       
       
   while(painter <= st_top)
   {
       if(*painter == 0) *painter=STACK_PAINT_VALUE; //paint the unused portion of the stack.
       painter++;
   }  
   #endif
   }
   void count_stack()
   {
   #if BENCHMARK_EN
       
       int *painter = (unsigned int )st_bottom;
       int count=0;
   painter=st_bottom;
       while(painter < st_top)
   {
       if (*painter != STACK_PAINT_VALUE)count++; // count the stack utilization
       painter++;
   } 
   
   mk_mon_debug_info(STACK_USAGE_INFO);
   mk_mon_debug_info((count + 0x1F)*4);	//0x1F is the overhead of count_stack() and mon_debug_tile_finished()
   #endif
   }   