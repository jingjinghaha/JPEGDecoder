 #ifndef __DMA_USAGE_H
 #define __DMA_USAGE_H
 #define START_OF_REMOTE_ADDR 0x10000000
 //counters
 extern unsigned int to_core1;
 extern unsigned int to_core2;
 extern unsigned int to_core3;
 extern unsigned int to_core4;
 extern unsigned int to_DDR;
 extern unsigned int from_core1;
 extern unsigned int from_core2;
 extern unsigned int from_core3;
 extern unsigned int from_core4;
 extern unsigned int from_DDR;
 void DMA_usage(unsigned int dst, unsigned int src, int block, unsigned int dma_base_addr);
 #endif