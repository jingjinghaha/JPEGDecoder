/**
 * HW_TIFU_DEF
 *
 * Copyright 2012 Delft University of Technology
 */

#ifndef __HW_TIFU_DEF__
#define __HW_TIFU_DEF__

#define SYSTEM_TIMER_INTERRUPT_MASK              0x00000001
#define SYSTEM_TIMER_INTERRUPT_MASK_OUT          0x0000EFFE

#define PARTITION_TIMER_INTERRUPT_MASK           0x00001002
#define PARTITION_EXTERNAL_INTERRUPT_MASK        0x00000FFC
#define PARTITION_INTERRUPT_PRECEDENCE           0x00001000



//functions for transfering the opcode to the TIFU
#define TIFU_FUNCTION_WITH_LENGTH(component, functionnumber, length) putfslx(component+functionnumber|(length&0x0000FFFF),FSL_TIFU,FSL_DEFAULT);

#define TIFU_FUNCTION_WITH_OFFSET(component, functionnumber, offset) putfslx(component+functionnumber|(offset&0x0000FFFF),FSL_TIFU,FSL_DEFAULT);

#define TIFU_FUNCTION_WITH_VECTOR(component, functionnumber, vector) putfslx(component+functionnumber|(vector&0x00FF),FSL_TIFU,FSL_DEFAULT);

#define TIFU_FUNCTION_WITH_VECTOR_CONTROLSIGNAL(component, functionnumber, vector) putfslx(component+functionnumber|(vector&0x00FF),FSL_TIFU,FSL_CONTROL);

#define TIFU_FUNCTION_WITH_NUMERATOR(component, functionnumber, numerator) putfslx(component+functionnumber|(( (numerator&0x00FF))<<8),FSL_TIFU,FSL_DEFAULT);

#define TIFU_FUNCTION_WITH_NUMERATOR_CONTROLSIGNAL(component, functionnumber, numerator) putfslx(component+functionnumber|(( (numerator&0x00FF))<<8),FSL_TIFU,FSL_CONTROL);

#define TIFU_FUNCTION_WITH_NUMERATOR_AND_DENOMINATOR_CONTROLSIGNAL(component, functionnumber, numerator, denominator) putfslx(component+functionnumber|(( (numerator&0x00FF))<<8)|( (denominator&0x00FF)),FSL_TIFU,FSL_CONTROL);

#define TIFU_FUNCTION(component, functionnumber) putfslx(component+functionnumber,FSL_TIFU,FSL_DEFAULT);

#define TIFU_FUNCTION_CONTROLSIGNAL(component, functionnumber) putfslx(component+functionnumber,FSL_TIFU,FSL_CONTROL);


//components
#define INTERRUPT 							0x01000000
#define SYSTEMTIMER 							0x02000000
#define PARTITIONTIMER 							0x03000000
#define FREQUENCY 							0x04000000
#define OS								0x05000000

#define INT_RAISE_VECTOR						0x00010000
#define INT_ACK_VECTOR							0x00020000
#define INT_LOWER_VECTOR						0x00020000
#define GET_VECTOR 							0x00030000
#define GET_MASKED_VECTOR						0x00040000
#define	SET_MASK							0x00050000
#define	SET_SYSINT_SYSTIMER_ABS					0x00060000
#define	SET_SYSINT_SYSTIMER_REL					0x00070000
#define	SET_PARINT_PARTIMER_ABS					0x00080000
#define	SET_PARINT_PARTIMER_REL					0x00090000
#define	SET_PARINT_SYSTIMER_ABS					0x000a0000
#define	SET_PARINT_SYSTIMER_REL					0x000b0000
#define INT_ENABLE_PARINT_SYSTIMER					0x000a0000
#define INT_DISABLE_PARINT_SYSTIMER					0x000b0000
#define GET_NEXT_SYSINT_RAISE_SYSTIMER 					0x00030000
#define GET_NEXT_PARINT_RAISE_PARTIMER					0x00040000
#define	GET_NEXT_PARINT_RAISE_SYSTIMER					0x000c0000


#define	SET								0x00010000
#define	GET								0x00020000
#define	START								0x00030000
#define	STOP								0x00040000
#define	ENABLE_INT							0x00030000
#define DISABLE_INT							0x00040000
#define SET_START							0x00050000
#define	SET_INT_PERIOD							0x00060000
#define ENABLE_PERIOD							0x00070000
#define DISABLE_PERIOD							0x00080000
#define SET_INT_NUMERATOR						0x00090000
#define SET_INT_NUMERATOR_FROM_SYSINT					0x00090000

#define GET_LAST_SYSINT							0x00030000
#define	SET_LAST_SYSINT							0x000b0000
#define	SET_PRELOAD_PAR_CONTEXT						0x00070000
#define	GET_SYSINT_EXTRA_WORK							0x00080000

#define	SET_SYSINT_STOP_OFFSET					0x000c0000
#define GET_LAST_PARINT							0x00040000
#define	SET_LAST_PARINT							0x000d0000
#define	GET_PARINT_EXTRA_WORK						0x00090000
#define	SET_PARINT_NUMERATOR						0x00090000
#define GET_PARINT_NUMERATOR						0x000b0000
#define	GET_PAR_NUMERATOR						0x000e0000
#define	GET_PAR_SLOT_NUMERATOR						0x000e0000
#define SET_PAR_NUMERATOR						0x000f0000
#define SET_PAR_SLOT_NUMERATOR						0x000f0000

#define SET_NUMERATOR_SYSTIMER_UNTIL					0x00010000
#define SET_NUMERATOR_SYSTIMER_FROM					0x00020000
#define SET_NUMERATOR_SYSTIMER_DURATION					0x00030000
#define SET_NUMERATOR_SYSTIMER_IN					0x00040000
#define SET_NUMERATOR_PARTIMER_UNTIL					0x00050000
#define SET_NUMERATOR_PARTIMER_FROM					0x00060000
#define SET_NUMERATOR_PARTIMER_DURATION					0x00070000
#define SET_NUMERATOR_PARTIMER_IN					0x00080000
#define SET_NUMERATOR_NOW						0x00090000

#define GET_ONE_BEFORE_LAST_NUMERATOR_CHANGE_SYSTIMER					0x00110000
#define GET_ONE_BEFORE_LAST_NUMERATOR_CHANGE_PARTIMER						0x00120000

#define	GET_LAST_NUMERATOR_CHANGE_SYSTIMER		0x00010000
#define	GET_LAST_NUMERATOR_CHANGE_PARTIMER		0x00020000
#define	SET_LAST_NUMERATOR_CHANGE_SYSTIMER		0x000b0000
#define	SET_LAST_NUMERATOR_CHANGE_PARTIMER		0x000d0000
#define	GET_NEXT_NUMERATOR_CHANGE_SYSTIMER		0x00010000
#define	GET_NEXT_NUMERATOR_CHANGE_PARTIMER		0x00020000
#define	GET_NEXT_CHANGE_SYSTIMER_NUMERATOR_VALUE	0x000a0000
#define	GET_NEXT_CHANGE_PARTIMER_NUMERATOR_VALUE	0x000a0000

#define	SET_MK_SLOT_LENGTH				0x00050000
#define	GET_ERROR_REG					0x00060000
#define	GET_DENOMINATOR					0x000d0000
#define	STOP_PARTIMER_AT_START_MKSLOT			0x000c0000

#endif
