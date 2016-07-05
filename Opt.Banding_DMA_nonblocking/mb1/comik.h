/**
 * CoMik API
 *
 * Copyright 2012 Delft University of Technology
 */
 #ifdef __cplusplus
 extern "C" {
 #endif

/**
 * \defgroup COMIK CoMik Public API
 * @{
 */

#ifndef __COMIK_H__
#define __COMIK_H__

/*
 * Global typedefs and structs
 */
#include "mk_types.h"


/**@{
 * @name CoMik
 */

/**
 * @param id The mk ID.
 * @param num_partitions The number of supported partitions (currently ignored)
 * @param partition_sched The tdm schedulers table.
 *
 * Initializes the MK.
 *
 * implemented in mk_comik.c
 */
void mk_init_comik(int id, int num_partitions, TABLE * partition_sched) __attribute__((nonnull(3)));

/**
 * starts the MK by initializing some CPU parameters
 *
 * @return the function never returns
 *
 * implemented in mk_comik.c
 */
void mk_start() __attribute__((noreturn));

/**
 * set the duration of the mk slot
 * @param val duration in cycles
 */
void mk_set_mk_slot_duration(int val);

/**@}*/

/**@{
 * @name Monitor interaction
 */

/**
 * sync with the monitor
 * should be called at the beginning, before doing any initialization
 * redeclared in mk_monitor.h
 */
void mk_mon_sync();

/**
 * @param val 32bit value to send to host pc.
 *
 * Sends val to be sent to the monitor (on behalf of the current partition)
 * to output it directly to the host PC.
 * The output format on the host PC depends on the monitor!
 */
void mk_mon_debug_info(int val);

/**
 * Send error messages to the monitor (on behalf of the current partition)
 * err is of one of the MON_ERROR_* types, and msg is a user message.
 *
 * @param err error flag
 * @param msg error code
 */
void mk_mon_error(int err, int msg);

/**
 * Send warning messages to the monitor (on behalf of the current partition)
 * warn is of one of the MON_WARNING_*, and msg is a user message.
 *
 * @param warn warning flag
 * @param msg warning code
 */
void mk_mon_warning(int warn, int msg);

/**
 * Sends out a given number of data after the given flag
 *
 * @param flag the first integer to be sent to the monitor
 * @param data pointer to the array of integers to be sent out
 * @param size the size of integer elements in the data
 *
 * redeclared in mk_monitor.h
 */
void mk_mon_info(int flag, int* data, int size);


/**@}*/

/**@{
 * @name Partition
 */

/**
 * @param id The partition id.
 * @param stack_size The stack size in bytes.
 * @param heap_size The heap size in bytes.
 * @param main_function The main function pointer.
 * @param interrupt_handler The interrupt handler function pointer.
 * @param exception_handler The exception handler function pointer.
 *
 * Create a new partition.
 * @returns the partition id.
 */
int mk_init_partition(int id, int stack_size, int heap_size,
        mk_main_callback main_function,
        mk_callback interrupt_handler,
        mk_callback exception_handler) __attribute__((nonnull(4)));

/**
 * @param id A partition id.
 * Removes partition id from CoMik.
 *
 * NOTE: A partition can only be removed if it is not currently running.
 * NOTE: A partition can only be removed if it is removed from the same heap
 * as allocated on.
 *
 * @returns 1 if successful, 0 if failed.
 */
int mk_remove_partition(int id);

/**
 * set the duration of the partition slot
 * @param val duration in cycles
 * redefined in mk_util.h
 */
void mk_set_partition_slot_duration(int val);

int mk_get_partition_slot_duration();

/**
 * Reset the stack of the current partition
 * to the original state.
 */
void mk_reset_current_partition_stack();

/**
 * Get the partition's interrupt vector.
 *
 * @returns the partition's interrupt vector as a bitmask.
 */
int mk_get_current_partition_interrupt_vec();

int mk_get_current_partition_xtra_work();

/**
 * @param mask The interrupt mask.
 *
 * Set the partition's interrupt mask.
 * This allows you to mask out certain interrupts.
 */
void mk_set_current_partition_interrupt_msk(int mask);

/**
 * Enable the partition's interrupt.
 */
void mk_enable_current_partition_interrupt();

/**
 * Disable the partition's interrupt.
 */
void mk_disable_current_partition_interrupt();

/**
 * Return the status of the partition's interrupt.
 */
int mk_current_partition_interrupt_status();

void** mk_get_current_partition_heap_lo_ptr();

void mk_ack_current_partition_interrupt(int ack_vector);

void mk_set_partition_interrupt_hndlr_type(int partition_id, InterruptHandler type);

/**
 * Get the cause of the exception.
 * The code are described in mb_ref_guide.pdf
 * @returns an integer with the exception code. (5 bit number)
 */
int mk_get_current_parition_exception_cause();

/**
 * Get the current partition id.
 * @returns an integer representing the current partition id.
 */
int mk_get_current_partition_id();

/**
 * Get the exception specific message.
 * Depending on the exception this returns extra information.
 * For the stack it is the stack pointer at the time of the exception.
 * (TODO) For unaligned memory access it indicates if it is halfword or word access.
 *
 * @returns the exception specific message
 */
int mk_get_current_parition_exception_msg();

/**
 * The program counter at the time the exception occurred.
 *
 * @returns the program counter at the time the exception occurred.
 */
int mk_get_current_parition_exception_addr();

/**@}*/

/**
 * pointer macros
 */
#ifndef VOID_PTR

/**
 * @param a The pointer to cast
 *
 * Cast pointer a to a _void *_ pointer
 */
#define VOID_PTR(a) (void *)(a)

/**
 * @param a The pointer to cast
 *
 * Cast pointer a to a _int *_ pointer
 */
#define INT_PTR(a) (int *)(a)

/**
 * @param a The pointer to cast
 *
 * Cast pointer a to a _void *_ pointer,
 * used by the DMA driver.
 */
#define DMAHW_PTR(a) (void *)(a)
#endif


/**@{
 * @name Dynamic memory allocation
 */

/**
 * Warning when using libc's malloc
 */
#define malloc(...)	please_use_mk_malloc

/**
 * Warning when using libc's free
 */
#define free(...)	please_use_mk_free



/**
 * function to allocate a memory block.
 * @ingroup MK_MALLOC
 * @param size size of the memory to be allocated, in bytes.
 * @return pointer to the allocated memory, NULL if it fails.
 * @see mk_free()
 */
void* mk_malloc(size_t size) __attribute__((malloc));

/**
 * function to free a memory location.
 * @ingroup MK_MALLOC
 * @param p pointer to the location to be freed. If p is NULL it does nothing
 * @see mk_malloc()
 */
void mk_free(void* p);

/**@}*/

/**
 * Abort the current partition.
 *
 * This will trigger the exception handler,
 * allowing dumping of state.
 *
 * Then unschedule the partition.
 */
void abort() __attribute__((noreturn));

/**@{
 * @Composable energy distribution
 */

/**
 * Composable energy distribution
 *
 * This will set the energy budgets of the partitions, of CoMik and a reserve energy budget such that energy composability is guaranteed
 * Accounting the energy is automatically initiated after calling this function
 *
 * @param energy Amount of core level energy in nano-Joules to be composably distributed
 */
void mk_energy_management_core_level_energy_distibution(ENERGY energy);

/**
 * Composable power distribution
 *
 * This will set the energy budgets of the partitions, of CoMik and a reserve energy budget such that energy composability is guaranteed
 * and the budgets are automatically replenished at a certain interval determined by the second parameter.
 * Note that function might alter the replenishment frequency and the amount of energy such that the replenishment interval is a multiple of TDM-table iterations
 *
 * @param power Amount of core level power in nano-Watts to be composably distributed
 * @param replenishment_frequency Number of replenishments per second [Hz]
 */
void mk_energy_management_core_level_power_distibution(POWER power, int replenishment_frequency);

/**
 * Get remaining partition energy
 *
 * @returns partition's remaining energy budget
 */
ENERGY mk_energy_get_partition_remaining_budget();

/**
 * Get budgeted partition energy
 *
 * @returns partition's energy budget
 */
ENERGY mk_energy_get_partition_budget();

/**
 * Get consumed partition energy
 *
 * @returns partition's consumed energy
 */
ENERGY mk_energy_get_partition_consumed_budget();

/**
 * TIFU wrapper function until
 *
 * Set a numerator from now on until a certain moment in (system) time.
 * Automatically updates the energy budgets if energy management is set.
 *
 * @param numerator Numerator value to be set
 * @param timevalue Moment in system time when the numerator is changed back to the old one
 */
void mk_tifu_freq_set_numerator_systimer_until(int numerator, TIME timevalue);

/**
 * TIFU wrapper function from
 * Set a numerator from a certain moment in (system) time.
 * Automatically updates the energy budgets if energy management is set.
 *
 * @param numerator Numerator value to be set
 * @param timevalue Moment in system time when the new numerator is set
 */
void mk_tifu_freq_set_numerator_systimer_from(int numerator, TIME timevalue);

/**
 * TIFU wrapper function duration
 * Set a numerator during a certain amount of (system) time.
 * Automatically updates the energy budgets if energy management is set.
 *
 * @param numerator Numerator value to be set
 * @param timevalue Amount of system time until the numerator is changed back to the old one
 */
void mk_tifu_freq_set_numerator_systimer_duration(int numerator, TIME timevalue);

/**
 * TIFU wrapper function in
 * Set a numerator after a certain amount of (system) time.
 * Automatically updates the energy budgets if energy management is set.
 *
 * @param numerator Numerator value to be set
 * @param timevalue Amount of system time until the numerator is set
 */
void mk_tifu_freq_set_numerator_systimer_in(int numerator, TIME timevalue);

/**
 * TIFU wrapper function now
 * Set a numerator now.
 * Automatically updates the energy budgets if energy management is set.
 *
 * @param numerator Numerator value to be set
 */
void mk_tifu_freq_set_numerator_now(int numerator);


/**
 * @param id A partition id.
 *
 * @returns the value with witch the partition returned, PARTITION_NOT_RETURNED
 * if the partition has not yet returned or does not exists.
 */
int mk_get_partition_return_value(int id);
/**@}*/

/**
 * abort - cause abnormal process termination
 * Disables the partition throwing the abort.
 *
 * defined in mk_comik.h
 */
void abort(void);

/**
 * Reads out the tile-id (>=1) from the Processor Version Register.
 * If it returns <=0, no id is set.
 *
 * defined in mk_comik.h
 */
int mk_get_tile_id();

/**
 * Indicate that this tile is done processing.
 */
void mk_mon_debug_tile_finished();
#endif

/** @}*/

#ifdef __cplusplus
}
#endif
