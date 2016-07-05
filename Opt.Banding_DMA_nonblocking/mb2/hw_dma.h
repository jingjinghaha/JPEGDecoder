/**
 * Driver functions for the DMA device
 *
 * Copyright 2010 Delft University of Technology
 * Created by Andrew Nelson
 * Based on work done by Dongrui She
 *
 * 23/12/2010 Jude Angelo Ambrose
 *             - remove DMA function added
 */

#ifndef __MK_DMA_H__
#define __MK_DMA_H__

#ifdef __cplusplus
extern "C" {
#endif
/**
 * \defgroup DMA DMA API
 * @{
 */

#include "mk_types.h"

/**
 * remove the DMA space and set  NULL in the list
 */
void hw_dma_remove(DMA *dma) __attribute__((nonnull));

/**
 * declare the number of dmas that are available for use
 * this is used to allocate the appropriate amount of space
 * to store the pointers to the DMA structs
 */
void hw_declare_dmas(int num_dmas);

/**
 * declare a DMA giving it a unique id
 * (0<id<num_dmas as declared using hw_declare_dmas(num_dmas))
 * The base_addr of the DMA is stored in its struct
 */
DMA * hw_dma_add(int id, void * base_addr) __attribute__((nonnull(2)));

/**
 * mk_malloc space for the DMA's list of connections
 */
void hw_dma_add_conns_list(DMA * dma, int num_conns) __attribute__((nonnull(1)));

/**
 * add the connections pointer to the DMA's connection list
 */
void hw_dma_add_conn(DMA * dma, CONN * conn) __attribute__((nonnull));

/**
 * returns the current DMA status
 */
int hw_dma_status(DMA * dma) __attribute__((nonnull));

/**
 * move data of size 'block_size' using 'dma' from 'src' to 'dst'
 */
void hw_dma_receive(void * dst, void * src, int block_size, DMA * dma, Persistance persistance) __attribute__((nonnull(1,2,4)));

/**
 * move data of size 'block_size' using 'dma' from 'src' to 'dst'
 */
void hw_dma_send(void * dst, void * src, int block_size, DMA * dma, Persistance persistance) __attribute__((nonnull(1,2,4)));

/**
 * returns the pointer to the DMA id
 */
DMA* hw_get_dma(int id);


/** @}*/

#ifdef __cplusplus
}
#endif

#endif
