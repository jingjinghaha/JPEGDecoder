 #include <global_memmap.h>
 #include <dma_usage.h>
 unsigned int to_core1 = 0;
 unsigned int to_core2 = 0;
 unsigned int to_core3 = 0;
 unsigned int to_core4 = 0;
 unsigned int to_DDR = 0;
 unsigned int from_core1 = 0;
 unsigned int from_core2 = 0;  
 unsigned int from_core3 = 0;
 unsigned int from_core4 = 0;
 unsigned int from_DDR = 0;
 void DMA_usage(unsigned int dst, unsigned int src, int block, unsigned int dma_base_addr)
 {
       //check if the DMA is used to push the data
       if(dst>=START_OF_REMOTE_ADDR && src < START_OF_REMOTE_ADDR){
               //DMA usaed by surrogate, just return?
               if(dst >= tft_surrogate_pt_REMOTEADDR && dst < mb4_cmemin0_pt_REMOTEADDR)
                       return;
               if(dst < mb1_cmemin0_pt_REMOTEADDR + mb1_cmemin0_SIZE)
                       to_core1++;
               else if(dst < mb2_cmemin0_pt_REMOTEADDR + mb2_cmemin0_SIZE)
                       to_core2++;
               else if(dst < mb3_cmemin0_pt_REMOTEADDR + mb3_cmemin0_SIZE)
                       to_core3++;
               else if(dst < mb3_cmemin1_pt_REMOTEADDR + mb3_cmemin1_SIZE)
                       to_core3++;
               else if(dst < mb4_cmemin0_pt_REMOTEADDR + mb4_cmemin0_SIZE)
                       to_core4++;
               else if(dst < mb4_cmemin1_pt_REMOTEADDR + mb4_cmemin1_SIZE)
                       to_core4++;
               else
                       to_DDR++;
       }
 else
       //check if the DMA is used to pull the data
       if(src>=START_OF_REMOTE_ADDR && dst < START_OF_REMOTE_ADDR){
               //DMA usaed by surrogate, just return?
               if(src >= tft_surrogate_pt_REMOTEADDR && src < mb4_cmemin0_pt_REMOTEADDR)
                       return;
               if(src < mb1_cmemin0_pt_REMOTEADDR + mb1_cmemin0_SIZE)
                       from_core1++;
               else if(src < mb2_cmemin0_pt_REMOTEADDR + mb2_cmemin0_SIZE)
                       from_core2++;
               else if(src < mb3_cmemin0_pt_REMOTEADDR + mb3_cmemin0_SIZE)
                       from_core3++;
               else if(src < mb3_cmemin1_pt_REMOTEADDR + mb3_cmemin1_SIZE)
                       from_core3++;
               else if(src < mb4_cmemin0_pt_REMOTEADDR + mb4_cmemin0_SIZE)
                       from_core4++;
               else if(src < mb4_cmemin1_pt_REMOTEADDR + mb4_cmemin1_SIZE)
                       from_core4++;
               else
                       from_DDR++;
       }
       return;
 }
 