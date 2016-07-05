#include <comik.h>
#include <global_memmap.h>
#include "core_number_measure.h"
#include <5kk03-utils.h>


int main (void)
{
	// Sync with the monitor.
	mk_mon_sync();
	// Enable stack checking.
	start_stack_check();
	begin_use();
	end_use();
	

	// Signal the monitor we are done.
	mk_mon_debug_tile_finished();
	return 0;
}
