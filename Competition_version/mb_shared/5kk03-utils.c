#undef DMA
#include <mk_types.h>
#include <hw_dma.h>
#include <comik.h>
#include "5kk03-utils.h"

void mk_mon_error(int, int);

extern unsigned int __stack;
extern unsigned int _stack;
extern unsigned int _stack_end;
void start_stack_check()
{
	// Initialize comik, this is needed for the exception handler to work.
	unsigned int a; // hack to make compiler happy.
	mk_init_comik(0, 0, (TABLE *)&a);
    // This is normally already set by compiler (slr and shr)
    // However, never hurts to reset.

    // Stack grows from high to low.
    unsigned int stack_l = (unsigned int) &_stack_end;
    unsigned int stack_h = (unsigned int) &__stack;
    // Bit to set when enabling exceptions.
    unsigned int exp_bit = 0x100;

    // ASM VooDoo
    __asm__("mts rslr,%0":: "r"(stack_l));
    __asm__("mts rshr,%0":: "r" (stack_h));
    __asm__("mts rmsr,%0;":: "r" (exp_bit));

}

void stop_stack_check()
{
    __asm__ ("mts rmsr,r0;");
}
#if 1
#define likely(x)    __builtin_expect (!!(x), 1)
#define unlikely(x)  __builtin_expect (!!(x), 0)
#define MON_ERROR_DMA_UNALIGNED 0x00000040

void hw_dma_receive_addr(void *dst, void*src, int block_size, void *dma_base_addr)
{
	DMA dma = {0,dma_base_addr, 0, NULL};
	if(unlikely((((unsigned int )src)&3) != 0)) {
		unsigned int origin = 0;
		__asm__("add %0,r15,r0": "=r"(origin));
		mk_mon_error(MON_ERROR_DMA_UNALIGNED, origin);
		return;
	}
	hw_dma_receive(dst,src,block_size, &dma, BLOCKING);
}


void hw_dma_send_addr(void *dst, void*src, int block_size, void *dma_base_addr)
{
	DMA dma = {0,dma_base_addr, 0, NULL};
	if(unlikely((((unsigned int )dst)&3) != 0)) {
		unsigned int origin = 0;
		__asm__("add %0,r15,r0": "=r"(origin));
		mk_mon_error(MON_ERROR_DMA_UNALIGNED, origin);
		return;
	}
	hw_dma_send(dst,src,block_size, &dma, BLOCKING);
}

void hw_dma_receive_non_block_addr(void *dst, void*src, int block_size, void *dma_base_addr)
{
	DMA dma = {0,dma_base_addr, 0, NULL};
	if(unlikely((((unsigned int )src)&3) != 0)) {
		unsigned int origin = 0;
		__asm__("add %0,r15,r0": "=r"(origin));
		mk_mon_error(MON_ERROR_DMA_UNALIGNED, origin);
		return;
	}
	hw_dma_receive(dst,src,block_size, &dma, NON_BLOCKING);
}

void hw_dma_send_non_block_addr(void *dst, void*src, int block_size, void *dma_base_addr)
{
	DMA dma = {0,dma_base_addr, 0, NULL};
	if(unlikely((((unsigned int )dst)&3) != 0)) {
		unsigned int origin = 0;
		__asm__("add %0,r15,r0": "=r"(origin));
		mk_mon_error(MON_ERROR_DMA_UNALIGNED, origin);
		return;
	}
	hw_dma_send(dst,src,block_size, &dma, NON_BLOCKING);
}

inline int hw_dma_status_addr(void *dma_base_addr)
{
	DMA dma = {0, dma_base_addr, 0, NULL};
	return hw_dma_status(&dma);
}
#endif
