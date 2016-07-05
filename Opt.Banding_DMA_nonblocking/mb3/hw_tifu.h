/*
 * HW_TIFU
 *
 * TIFU Drivers
 *
 * Copyright 2013 Delft University of Technology
 */

#ifndef __HW_TIFU_H__
#define __HW_TIFU_H__

/**
 * \defgroup TIFU TIFU API
 * \brief API to access the Timer centric Interrupt and Frequency Unit.
 * \{
 */


/**
 * \brief initialize the TIFU
 *
 * sets the sysint numerator and denominator (both used after first system interrupt),
 * sets par slot preload time and sysint_stop_offset parameters, sets the mk slot and period length
 * first argument signifies when the first system interrupt will occur
 * will generate an error when the mk_slot_length or par_slot_length is not a multiple of the denominator,
 * which is required by TIFU
 */
void hw_tifu_init(TIME first_sysint, TIME mk_slot_length, TIME par_slot_length,
                  int par_slot_preload_length, int sysint_stop_offset,
                  int sysint_numerator, int denominator);

/**
 * wrapper function to initialize the TIFU using the standaard comik values:
 * uses mk_slot_length, par_slot_length , par slot preload length and sysint stop offset parameters
 * given by mk_global.h and MAX_FREQ as sysint numerator and denominator
 * argument is used for first system interrupt
 */
void mk_tifu_init (TIME time);

/** \} */

/**
 *\defgroup TIFU_interrupt Interrupt
 *
 * \brief interrupt manager functions
 * \ingroup TIFU
 * \{
 */
/**
 * raise bits of the TIFU interrupt vector
 * ORs the given vector with the TIFU interrupt vector
 */
void hw_tifu_int_raise_vector(unsigned int vector);

/**
 * acknowledge bits of the TIFU interrupt vector
 * lowers the bits of the TIFU interrupt vector where the given vector has raised bits
 * generates an error if an interrupt is acknowledged that is not raised
 */
void hw_tifu_int_ack_vector(unsigned int vector);

/**
 * acknowledge bits of the TIFU interrupt vector
 * lowers the bits of the TIFU interrupt vector where the given vector has raised bits
 * does not generate an error: should only be used by CoMiK
 */
void hw_tifu_int_lower_vector(unsigned int vector);

/**
 * get the interrupt vector of the TIFU, regardless of the mask
 */
int hw_tifu_int_get_vector();

/**
 * get the interrupt vector after ANDed with the mask
 */
int hw_tifu_int_get_masked_vector();

/**
 * sets the mask
 * interrupts are only propagated when the corresponding mask bit is raised
 */
void hw_tifu_int_set_mask(unsigned int mask);

/**
 * programs a system interrupt based on the system timer
 * the interrupt is generated at the given timevalue
 * if the value is in the past, it is generated immediately and generates a warning
 */
void hw_tifu_int_set_sysint_systimer_abs(TIME timevalue);

/**
 * programs a system interrupt based on the system timer
 * the interrupt is generated in the given timevalue, i.e. the interrupt is generated at the timevalue: (current system timer value + given timevalue)
 */
void hw_tifu_int_set_sysint_systimer_rel(TIME timevalue);

/**
 * programs a partition interrupt based on the partition timer
 * the interrupt is generated at the given timevalue
 * if the value is in the past, it is generated immediately and generates a warning
 */
void hw_tifu_int_set_parint_partimer_abs(TIME timevalue);

/**
 * programs a partition interrupt based on the partition timer
 * the interrupt is generated in the given timevalue, i.e. the interrupt is generated at the timevalue: (current partition timer value + given timevalue)
 */
void hw_tifu_int_set_parint_partimer_rel(TIME timevalue);

/**
 * programs a partition interrupt based on the system timer
 * the interrupt is generated at the given timevalue
 * if the value is in the past, it is generated immediately and generates a warning
 */
void hw_tifu_int_set_parint_systimer_abs(TIME timevalue);

/**
 * programs a partition interrupt based on the system timer
 * the interrupt is generated in the given timevalue, i.e. the interrupt is generated at the timevalue: (current system timer value + given timevalue)
 */
void hw_tifu_int_set_parint_systimer_rel(TIME timevalue);

/**
 * enables the partition interrupts based on the system timer
 */
void hw_tifu_int_enable_parint_systimer();

/**
 * disables the partition interrupts based on the system timer
 * when disabled, a partition interrupt based on the system timer is never generated until enabled
 */
void hw_tifu_int_disable_parint_systimer();

/**
 * get the time at which the next system interrupt based on the system timer is programmed
 */
TIME hw_tifu_int_get_next_sysint_raise_systimer();

/**
 * get the time at which the next partition interrupt based on the partition timer is programmed
 */
TIME hw_tifu_int_get_next_parint_raise_partimer();

/**
 * get the time at which the next partition interrupt based on the system timer is programmed
 */
TIME hw_tifu_int_get_next_parint_raise_systimer();

/** \} */


/**
 *\defgroup TIFU_systimer System Timer
 *
 * \brief system timer functions
 * \ingroup TIFU
 * \{
 */
/**
 * set the current value of the system timer
 */
void hw_tifu_systimer_set(TIME timevalue);

/**
 * get the current value of the system timer
 */
TIME hw_tifu_systimer_get();

/**
 * start the counting of the system timer
 * the system timer increases every system clock cycle
 */
void hw_tifu_systimer_start();

/**
 * stop the counting of the system timer
 * the system timer value remains the same
 */
void hw_tifu_systimer_stop();

/**
 * enable the system interrupts based on the system timer
 */
void hw_tifu_systimer_enable_int();

/**
 * disables the system interrupt based on the system timer
 * when disabled, a partition interrupt based on the system timer is never generated until enabled
 */
void hw_tifu_systimer_disable_int();

/**
 * set the system timer value and immediately start the timer
 * the system timer increases every system clock cycle
 */
void hw_tifu_systimer_set_start(TIME timevalue);

/**
 * set the period of the system interrupts
 * a new system interrupt is generated exactly the period length number of cycles after the previous interrupt
 * will only be used when interrupt period is enabled
 */
void hw_tifu_systimer_set_int_period(TIME timevalue);

/**
 * enable the period of the system interrupts
 * a new system interrupt is generated exactly the period length number of cycles after the previous interrupt
 */
void hw_tifu_systimer_enable_int_period();

/**
 * disable the period of the system interrupts
 * only one interrupt is generated when an interrupt is programmed
 */
void hw_tifu_systimer_disable_int_period();

/**
 * set the numerator of the system interrupt
 * the TIFU will change to this numerator when a system interrupt is generated
 */
void hw_tifu_systimer_set_int_numerator(int numerator);

/**
 * set the numerator of the system interrupt and sets the denominator
 * the denominator is only changed after the next system interrupt
 * the TIFU will change to the given numerator when a system interrupt is generated
 */
void hw_tifu_systimer_set_numerator_denominator_from_sysint(int numerator, int denominator);

/**
 * get the TIME value of the last generated system interrupt
 */
TIME hw_tifu_systimer_get_last_sysint();

/**
 * set the TIME value of the last generated system interrupt
 * only used for restoring context, as this value can only be read
 */
void hw_tifu_systimer_set_last_sysint(TIME timevalue);

/**
 * set the preload_par_context value
 * the TIFU is waked up this value of cycles before the next partition slot
 * in this time the context of the partition is loaded
 * when loading the context takes longer or shorter, this value needs to be corrected, otherwhise the partition executes less or more cycles respectively
 */
void hw_tifu_systimer_set_preload_par_context(int length);

/**
 * get the extra work of the system interrupt
 * the extra work is the number of partition cycles before the system interrupt handler is invoked
 */
int hw_tifu_systimer_get_sysint_extra_work();

/** \} */

/**
 *\defgroup TIFU_partimer Partition Timer
 *
 * \brief partition timer functions
 * \ingroup TIFU
 * \{
 */

/**
 * sets the current value of the partition timer
 */
void hw_tifu_partimer_set(TIME timevalue);

/**
 * get the current value of the partition timer
 */
TIME hw_tifu_partimer_get();

/**
 * start the counting of the partition timer
 * the partition timer increases every partition clock cycle
 */
void hw_tifu_partimer_start();

/**
 * stop the counting of the partition timer
 * the partition timer value remains the same
 */
void hw_tifu_partimer_stop();

/**
 * enable the partition interrupts based on the partition timer
 */
void hw_tifu_partimer_enable_int();

/**
 * disables the partition interrupt based on the partition timer
 * when disabled, a partition interrupt based on the partition timer is never generated until enabled
 */
void hw_tifu_partimer_disable_int();

/**
 * set the partition timer value and immediately start the timer
 * the partition timer increases every partition clock cycle
 */
void hw_tifu_partimer_set_start(TIME timevalue);

/**
 * set the period of the partition interrupts
 * a new partition interrupt is generated exactly the period length number of cycles after the previous interrupt
 * will only be used when interrupt period is enabled
 */
void hw_tifu_partimer_set_int_period(TIME timevalue);

/**
 * enable the period of the partition interrupts
 * a new partition interrupt is generated exactly the period length number of cycles after the previous interrupt
 */
void hw_tifu_partimer_enable_int_period();

/**
 * disable the period of the partition interrupts
 * only one interrupt is generated when an interrupt is programmed
 */
void hw_tifu_partimer_disable_int_period();

/**
 * set the sysint_stop_offset value
 * the sysint stop offset is the number of cycles between starting the interrupt handler and the hw_tifu_stop_partimer_at_start_mkslot (at the beginnning of the interrupt handler)
 * is used to revert the partition timer value to the time of the last executed instruction of the partition
 * should be updated when the start of the system interrupt handler is changed
 */
void hw_tifu_partimer_set_sysint_stop_offset(int offset);

/**
 * get the TIME value of the last generated partition interrupt
 */
TIME hw_tifu_partimer_get_last_parint();

/**
 * set the TIME value of the last generated partition interrupt
 * only used for restoring context, as this value can only be read
 */
void hw_tifu_partimer_set_last_parint(TIME timevalue);

/**
 * get the extra work of the partition interrupt
 * the extra work is the number of partition instructions executed before the partition interrupt handler is invoked
 */
int hw_tifu_partimer_get_parint_extra_work();

/**
 * set the par_int numerator
 * this numerator is what TIFU will change to when a partition interrupt is raised
 */
void hw_tifu_partimer_set_par_int_numerator(int int_numerator);

/**
 * get the par_int numerator
 * this numerator is what TIFU will change to when a partition interrupt is raised
 * used for saving the context
 */
int hw_tifu_partimer_get_par_int_numerator();

/**
 * get the par numerator
 * this numerator keeps track of the numerator except for the partition interrupts
 */
int hw_tifu_partimer_get_par_numerator();

/**
 * get the par_slot numerator
 * this numerator keeps track of the numerator of the partition, including partition interrupts
 * the TIFU will go to this numerator when the comik slot ends
 */
int hw_tifu_partimer_get_par_slot_numerator();

/**
 * set the par numerator
 * this numerator keeps track of the numerator except for the partition interrupts
 */
void hw_tifu_partimer_set_par_numerator(int main_numerator);

/**
 * set the par_slot numerator
 * this numerator keeps track of the numerator of the partition, including partition interrupts
 * the TIFU will go to this numerator when the comik slot ends
 */
void hw_tifu_partimer_set_par_slot_numerator(int slot_numerator);


/**\}*/

/**
 *\defgroup TIFU_freqfunc  Frequency functions
 *
 * \brief frequency change functions
 * \ingroup TIFU
 * \{
 */

/**
 * program a numerator change based on the system timer
 * the numerator is changed to the provided numerator immediately, and returns to the numerator when this function is issued at the given timevalue
 * when the timevalue is in the past, it reverts immediately and gives a warning
 * only changes the numerator at the end of a denominator block
 */
void hw_tifu_freq_set_numerator_systimer_until(int numerator, TIME timevalue);

/**
 * program a numerator change based on the system timer
 * the numerator is changed to the provided numerator immediately, and returns to the numerator when this function is issued at the given timevalue
 * when the timevalue is in the past, it reverts immediately and gives a warning
 * changes the numerator at exactly the given timevalue, ignoring the denominator blocks
 */
void hw_tifu_freq_set_numerator_systimer_until_ignoring_denominator_block(int numerator, TIME timevalue);

/**
 * program a numerator change based on the system timer
 * the numerator is changed to the given value permanently from the given timevalue onward
 * when the timevalue is in the past, the numerator is chagned immediately and gives a warning
 * only changes the numerator at the end of a denominator block
 */
void hw_tifu_freq_set_numerator_systimer_from(int numerator, TIME timevalue);

/**
 * program a numerator change based on the system timer
 * the numerator is changed to the given value permanently from the given timevalue onward
 * when the timevalue is in the past, the numerator is chagned immediately and gives a warning
 * changes the numerator at exactly the given timevalue, ignoring the denominator blocks
 */
void hw_tifu_freq_set_numerator_systimer_from_ignoring_denominator_block(int numerator, TIME timevalue);

/**
 * program a numerator change based on the system timer
 * the numerator is changed to the provided numerator immediately, and returns to the numerator when this function is issued after the given timevalue number of cycles
 * only changes the numerator at the end of a denominator block
 */
void hw_tifu_freq_set_numerator_systimer_duration(int numerator, TIME timevalue);

/**
 * program a numerator change based on the system timer
 * the numerator is changed to the provided numerator immediately, and returns to the numerator when this function is issued  after the given timevalue number of cycles
 * changes the numerator at exactly the given timevalue, ignoring the denominator blocks
 */
void hw_tifu_freq_set_numerator_systimer_duration_ignoring_denominator_block(int numerator, TIME timevalue);

/**
 * program a numerator change based on the system timer
 * the numerator is changed to the provided numerator permanently after the provided number of cycles
 * only changes the numerator at the end of a denominator block
 */
void hw_tifu_freq_set_numerator_systimer_in(int numerator, TIME timevalue);

/**
 * program a numerator change based on the system timer
 * the numerator is changed to the provided numerator permanently after the provided number of cycles
 * changes the numerator at exactly the given timevalue, ignoring the denominator blocks
 */
void hw_tifu_freq_set_numerator_systimer_in_ignoring_denominator_block(int numerator, TIME timevalue);

/**
 * program a numerator change based on the partition timer
 * the numerator is changed to the provided numerator immediately, and returns to the numerator when this function is issued  at the given timevalue
 * when the timevalue is in the past, it reverts immediately and gives a warning
 * only changes the numerator at the end of a denominator block
 */
void hw_tifu_freq_set_numerator_partimer_until(int numerator, TIME timevalue);

/**
 * program a numerator change based on the partition timer
 * the numerator is changed to the given value permanently from the given timevalue onward
 * when the timevalue is in the past, the numerator is chagned immediately and gives a warning
 * only changes the numerator at the end of a denominator block
 */
void hw_tifu_freq_set_numerator_partimer_from(int numerator, TIME timevalue);

/**
 * program a numerator change based on the partition timer
 * the numerator is changed to the provided numerator immediately, and returns to the numerator when this function is issued  after the given timevalue number of cycles
 * only changes the numerator at the end of a denominator block
 */
void hw_tifu_freq_set_numerator_partimer_duration(int numerator, TIME timevalue);

/**
 * program a numerator change based on the system timer
 * the numerator is changed to the provided numerator permanently after the provided number of cycles
 * only changes the numerator at the end of a denominator block
 */
void hw_tifu_freq_set_numerator_partimer_in(int numerator, TIME timevalue);

/**
 * immediately changes the numerator to the given value
 * only changes the numerator at the end of a denominator block
 */
void hw_tifu_freq_set_numerator_now(int numerator);

/**
 * immediately changes the numerator to the given value
 * changes the numerator instantly, ignoring the denominator blocks
 */
void hw_tifu_freq_set_numerator_now_ignoring_denominator_block(int numerator);

/**
 * get the time when the last frequency change occured, based on the system timer
 */
TIME hw_tifu_freq_get_last_numerator_change_systimer();

/**
 * get the time when the one before last frequency change occured, based on the system timer
 */
TIME hw_tifu_freq_get_one_before_last_numerator_change_systimer();

/**
 * get the time when the last frequency change occured, based on the partition timer
 */
TIME hw_tifu_freq_get_last_numerator_change_partimer();

/**
 * get the time when the one before last frequency change occured, based on the partition timer
 */
TIME hw_tifu_freq_get_one_before_last_numerator_change_partimer();

/**
 * set the time when the last frequency change occured, based on the system timer
 * only needed for restoring the context as it can only be read
 */
void hw_tifu_freq_set_last_numerator_change_systimer(TIME timevalue);

/**
 * set the time when the last frequency change occured, based on the partition timer
 * only needed for restoring the context as it can only be read
 */
void hw_tifu_freq_set_last_numerator_change_partimer(TIME timevalue);

/**
 * get the time when the next frequency change will occur, based on the system timer
 * will be set to 0xFFFFFFFFFFFFFFFF when a numerator change is not scheduled
 * must be set to highest number as a value in the past is resolved immediately
 */
TIME hw_tifu_freq_get_next_numerator_change_systimer();

/**
 * get the time when the next frequency change will occur, based on the partition timer
 * will be set to 0xFFFFFFFFFFFFFFFF when a numerator change is not scheduled
 * must be set to highest number as a value in the past is resolved immediately
 */
TIME hw_tifu_freq_get_next_numerator_change_partimer();

/**
 * get the numerator to change to when the next frequency change will occur, based on the system timer
 */
int hw_tifu_freq_get_next_change_systimer_numerator_value();

/**
 * get the numerator to change to when the next frequency change will occur, based on the partition timer
 */
int hw_tifu_freq_get_next_change_partimer_numerator_value();

/** \} */

/**
 *\defgroup TIFU_osfunc OS functions
 *
 * \brief OS functions
 * \ingroup TIFU
 * \{
 */


/**
 * set the length of the mk slot
 * is needed for the TIFU to calculate when the partition context loading should be started before the partition begins
 */
void hw_tifu_set_mk_slot_length(TIME length);

/**
 * get the error register, in which the last 8 bits represent (from left to right descending):
 * (7) a system interrupt occurs when the previous was not acknowledged
 * (6) acknowledging an unset interrupt
 * (5) not used
 * (4) not gated when preload should begin, indicating that the mk slot was not long enough
 * (3) denominator of 0
 * (2) system interrupt was raised before the end of the mk slot
 * (1) partition counter warning programmed in past
 * (0) system counter warning programmed in past
 */
int hw_tifu_get_error_reg();

/**
 * get the currently active denominator
 */
int hw_tifu_get_denominator();

/**
 * stops and revert back the partition timer at the beginning of comik interrupt handler,
 * and in case of partition interrupt handler, adjusts the partition extra work.
 */
void hw_tifu_stop_partimer_at_start_mkslot();

/** \} */
#endif

