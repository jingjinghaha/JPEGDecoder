/**
 * MK_TYPES
 *
 * Definition of all comik types
 *
 * Copyright 2012 Delft University of Technology
 */

#ifndef __MK_TYPES_H__
#define __MK_TYPES_H__

/*
 * Standard C types
 */
#include <stddef.h>
#include <limits.h>

#define MAX_PARTITIONS                     8
#define PARTITION_NOT_RETURNED             INT_MAX

/*
 * Typedefs
 */
typedef long long int          int64;
typedef unsigned long long int uint64;
typedef int64                  TIME;
typedef int64                  ENERGY;
typedef int64                  POWER;

typedef struct CCB_s           CCB;
typedef struct PCB_s           PCB;
typedef struct MCB_s           MCB;

/* High level Memory Management Control Block */
typedef struct _MMCB MMCB;
typedef struct CONN_s          CONN;
typedef struct DMA_s           DMA;
typedef struct TIFU_s          TIFU;
typedef struct PEA_s           PEA;
typedef struct CEA_s           CEA;
typedef struct RPM_s           RPM;

typedef struct TABLE_s         TABLE;

/* Function type for callbacks */
typedef void(*mk_callback)(void);
typedef int (*mk_main_callback)(void);

typedef enum Persistance_e {
	NON_BLOCKING,
	BLOCKING
} Persistance;

typedef union {
	int64  si64;
	uint64 ui64;
	struct {
/* This order is for little endian */
		unsigned int lo;
		unsigned int hi;
	}hilo;
} hilo64;

#define HI_64(d) (((hilo64*)&(d))->hilo.hi)
#define LO_64(d) (((hilo64*)&(d))->hilo.lo)

typedef enum InterruptHandler_e {
	STANDALONE,
	INTEGRATED
} InterruptHandler;

/**
 * Control block for dynamic memory allocation.
 */
struct _MMCB {
    int *heap_start;
    int *heap_end;

    int initialized;
    /**
     * Variables for heap management
     */
    int* last_allocated_mcb;
    /**
     * Bookkeeping.
     */
    int heap_usage;
    int heap_max;
    int num_mallocs;
    int num_frees;
};

/**
 * Partition control block
 */
struct PCB_s{
	int id;
	CCB * parent;

	void * stack_lo;
	void * stack_hi;
	int    stack_size;
	void * stack_ptr;

	void * intrd_stack_lo[2];
	void * intrd_stack_hi[2];
	void * intrd_stack_ptr[2];
	int nested_int;

    MMCB heap_mmcb;

	mk_main_callback main_function;
	mk_callback interrupt_handler;
	mk_callback exception_handler;

    // The cause for the exception. (See mb_ref_guide)
    int exception_cause;
    // Exception specific message.
    // (for stack, the cur stack pointer)
    int exception_msg;
    // The Program Counter at the time of the exception.
    int exception_addr;

    // Return value of the partition.
    int return_value;

	int interrupt_vec;
	int interrupt_vec_tmp;
	int interrupt_msk;
	int int_xtra_work;
	int tifu_interrupt_enbld;
	int tifu_periodic_enbld;

	int slot_numerator;
	int int_numerator;
	int main_numerator;
	TIME tifu_par_timer;
	TIME tifu_next_parint_partimer;
	TIME tifu_freq_next_change_partimer;
	int  tifu_freq_next_partimer_numerator;
	TIME tifu_next_parint_systimer;
	TIME tifu_freq_next_change_systimer;
	int  tifu_freq_next_systimer_numerator;

	InterruptHandler interrupt_hndlr_type;
	PEA * par_energy_account;

	PCB * BE_prev_par;
	PCB * BE_next_par;

    // Pointer to the imem_region of this partition.
    void *imem_region;
    // Pointer to the dmem_region (for program code) of this partition.
    void *dmem_region;
};

/**
 * A comik instance
 * all comik variables can be accessed from this struct
 */
struct CCB_s{
	int id;

	void * stack_lo;
	void * stack_hi;
	int    stack_size;
	void * stack_ptr;

    MMCB * heap_mmcb;
    // Manage the instruction memory.
    MMCB * imem_mmcb;

	PCB partitions[MAX_PARTITIONS];
	int num_partitions;
	PCB * current_partition;
	PCB * slot_owner;

	TABLE * partition_sched;

	PCB * BE_par_head;
	PCB * BE_par_tail;

	DMA ** dma_list; //array of pointers
	int dma_list_len;

	TIFU * tifu;

	int partition_slot_duration; //cycles@fmax

	int mk_slot_duration;
	int interrupt_max_jitter;

	TIME last_interrupt_systime;
	TIME last_read_system_time;
	int int_xtra_work;

	ENERGY * processing_tile_power_table;
	CEA * core_energy_account;
	RPM * replenishment_manager;
	int tifu_numerator;
	int tifu_denominator;
};

/**
 * Connection descriptor
 */

struct CONN_s{
	int id;
	void * LO_addr;
	void * HI_addr;
};

/**
 * DMA descriptor
 */
struct DMA_s{
	int id;
	void * base_addr;
	int num_conns;
	CONN ** conns;
};

/**
 * Timer-based Interrupt and Frequency Unit
 */
struct TIFU_s{
	unsigned int freq_max_hz;
	unsigned int freq_steps;
	unsigned int * freq_list_hz;
	unsigned int * freq_list_pa;
	unsigned int * freq_to_energy;
};

/**
 * Partition Energy account struct
 */
struct PEA_s{
    ENERGY par_energy_budget_total;
	ENERGY par_energy_budget_remaining;
	TIME last_budget_update_sys_time;
	int previous_numerator;             //Store old numerator for accounting. //FIXME should be done in hardware
	int par_energy_budget_depleted;     //If true, partition is gated until replenishment
	TIME programmed_num_change_systime; //If a numerator change is programmed in the future, store future time of change here.
};

/**
 * Core Energy account struct
 */
struct CEA_s{
    ENERGY core_energy_budget_total;
    ENERGY core_energy_budget_unused;       //Amount of energy not usable by RT applications. Can be assigned to BE application(s)
	ENERGY comik_energy_budget_total;
	ENERGY comik_energy_budget_remaining;
	ENERGY reserve_energy_budget_total;
	ENERGY reserve_energy_budget_remaining;
	int comik_energy_budget_depleted;       //If true, Energy in the system has been depleted.
	int reserve_energy_budget_depleted;     //If true, Energy in the system has been depleted.
	int ignore_depletion;
};

/**
 * Replenishment management struct
 */
struct RPM_s{
    ENERGY replenishment_energy;
    TIME replenishment_interval;
    int total_TDM_iterations_in_interval;
    int total_slots_in_interval;
    int current_slot_in_interval;
    ENERGY rounding_error_energy;
};

/**
 * Struct for dynamic memory allocation
 */
struct MCB_s {
  int is_used;        /*! Indicates if the block is used */
  int wsize;          /*! Size of this block in words */
  MMCB *parent;
#ifdef __USE_PREVIOUS_MCB
  int* previous_mcb;  /*! Link to the previous mcb; currently not used, but maintained for future extension  */
#endif
};

struct TABLE_s{
	int  index; // current table index
	int  size;  // number of ints in the table
	int* table; // pointer to the table
};

#endif
