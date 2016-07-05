#ifndef __5KK03_UTILS_H__
#define __5KK03_UTILS_H__

void start_stack_check();
void stop_stack_check();

void hw_dma_receive_addr(void *dst, void*src, int block_size, void *dma_base_addr);
void hw_dma_receive_non_block_addr(void *dst, void*src, int block_size, void *dma_base_addr);
void hw_dma_send_addr(void *dst, void*src, int block_size, void *dma_base_addr);
void hw_dma_send_non_block_addr(void *dst, void*src, int block_size, void *dma_base_addr);
inline int hw_dma_status_addr(void *dma_base_addr);
#endif
