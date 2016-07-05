 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #include <comik.h>
 #include <global_memmap.h>
 #include <5kk03-utils.h>
 #include <hw_tifu.h>
 TIME measure_begin_timer;
 TIME measure_end_timer;
 TIME measure_diff_timer;
 void begin_use()
 {
   measure_begin_timer = hw_tifu_systimer_get();
 }
 void end_use()
 {
   measure_end_timer = hw_tifu_systimer_get();
   measure_diff_timer = measure_end_timer - measure_begin_timer;
   mk_mon_debug_info(2147483579+LO_64(measure_diff_timer));
 }