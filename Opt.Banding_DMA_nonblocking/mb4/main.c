#include <comik.h>
#include <global_memmap.h>
#include <5kk03-utils.h>
#include <jpeg.h>
#include "core_number_measure.h"
// #ifndef CORE_NUMBER_MEASURE_H
 #define CORE_NUMBER_MEASURE_H
 extern TIME measure_begin_timer;
 extern TIME measure_end_timer;
 extern TIME measure_diff_timer;


int main (void)
{
	// Sync with the monitor.
	mk_mon_sync();
	// Enable stack checking.
	start_stack_check();
// /*
	begin_use();
  	
	int output = JpegToBmp();
	
	mk_mon_debug_info(output);

	// Signal the monitor we are done.
	 end_use();
// */
	mk_mon_debug_tile_finished();
	return 0;
}

// #endif
