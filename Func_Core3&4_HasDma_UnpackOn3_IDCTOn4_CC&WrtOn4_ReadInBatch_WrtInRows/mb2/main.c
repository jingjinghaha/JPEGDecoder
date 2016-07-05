#include <comik.h>
#include <global_memmap.h>
#include <5kk03-utils.h>
#include <core_number_measure.h>
#include <dma_usage.h>

int main (void)
{
	// Sync with the monitor.
	mk_mon_sync();
	// Enable stack checking.
	start_stack_check();

	begin_use();
	end_use();
   mk_mon_debug_info(0x00444D41);//start of DMA_counters
   //print the dma_counters according to the following order here
     mk_mon_debug_info(to_core1);
     mk_mon_debug_info(to_core2);
     mk_mon_debug_info(to_core3);
     mk_mon_debug_info(to_core4);
     mk_mon_debug_info(to_DDR);
     mk_mon_debug_info(from_core1);
     mk_mon_debug_info(from_core2);
     mk_mon_debug_info(from_core3);
     mk_mon_debug_info(from_core4);
     mk_mon_debug_info(from_DDR);
   mk_mon_debug_info(0xFF444D41);//end of DMA_counters
	// Signal the monitor we are done.
	mk_mon_debug_tile_finished();
	return 0;
}
