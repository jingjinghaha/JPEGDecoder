#ifndef _GLOBAL_MEMMAP_H_
#define _GLOBAL_MEMMAP_H_

/* Tile mb1 local addresses */

#define mb1_imem_SIZE 0x20000
#define mb1_dmem_SIZE 0x20000
#define mb1_CMEMOUTS 1
#define mb1_DMAS 1
#define mb1_cmemout0_SIZE 0x8000
#define mb1_CMEMINS 1
#define mb1_cmemin0_SIZE 0x8000
#define mb1_imem_BASEADDR 0x00000000
#define mb1_dmem_BASEADDR 0x00020000
#define mb1_cmemin0_BASEADDR 0x00040000
#define mb1_cmemout0_BASEADDR 0x00048000
#define mb1_dma0_BASEADDR 0x000F0000

/* Tile mb2 local addresses */

#define mb2_imem_SIZE 0x20000
#define mb2_dmem_SIZE 0x20000
#define mb2_CMEMOUTS 1
#define mb2_DMAS 1
#define mb2_cmemout0_SIZE 0x8000
#define mb2_CMEMINS 1
#define mb2_cmemin0_SIZE 0x8000
#define mb2_imem_BASEADDR 0x00000000
#define mb2_dmem_BASEADDR 0x00020000
#define mb2_cmemin0_BASEADDR 0x00040000
#define mb2_cmemout0_BASEADDR 0x00048000
#define mb2_dma0_BASEADDR 0x000F0000

/* Tile mb3 local addresses */

#define mb3_mem_SIZE 0x10000
#define mb3_CMEMOUTS 2
#define mb3_DMAS 2
#define mb3_cmemout0_SIZE 0x2000
#define mb3_cmemout1_SIZE 0x2000
#define mb3_CMEMINS 2
#define mb3_cmemin0_SIZE 0x2000
#define mb3_cmemin1_SIZE 0x2000
#define mb3_mem_BASEADDR 0x00000000
#define mb3_cmemin0_BASEADDR 0x00010000
#define mb3_cmemin1_BASEADDR 0x00012000
#define mb3_cmemout0_BASEADDR 0x00014000
#define mb3_dma0_BASEADDR 0x000F0000
#define mb3_cmemout1_BASEADDR 0x00016000
#define mb3_dma1_BASEADDR 0x000F1000

/* Tile mb4 local addresses */

#define mb4_mem_SIZE 0x10000
#define mb4_CMEMOUTS 2
#define mb4_DMAS 2
#define mb4_cmemout0_SIZE 0x2000
#define mb4_cmemout1_SIZE 0x2000
#define mb4_CMEMINS 2
#define mb4_cmemin0_SIZE 0x2000
#define mb4_cmemin1_SIZE 0x2000
#define mb4_mem_BASEADDR 0x00000000
#define mb4_cmemin0_BASEADDR 0x00010000
#define mb4_cmemin1_BASEADDR 0x00012000
#define mb4_cmemout0_BASEADDR 0x00014000
#define mb4_dma0_BASEADDR 0x000F0000
#define mb4_cmemout1_BASEADDR 0x00016000
#define mb4_dma1_BASEADDR 0x000F1000

/* NoC addresses */

#define mb1_cmemin0_pt_REMOTEADDR 0x10000000
#define mb2_cmemin0_pt_REMOTEADDR 0x20000000
#define mb3_cmemin0_pt_REMOTEADDR 0x30000000
#define mb3_cmemin1_pt_REMOTEADDR 0x34000000
#define mb4_cmemin0_pt_REMOTEADDR 0x40000000
#define mb4_cmemin1_pt_REMOTEADDR 0x44000000
#define tft_surrogate_pt_REMOTEADDR 0x3F000000
#define shared_pt_REMOTEADDR 0x48000000

/* FSL connections */

#define mb1_fsl 0
#define mb2_fsl 1
#define mb3_fsl 2
#define mb4_fsl 3
#define raptor_fsl 4
#define shared_fsl 4

#endif
