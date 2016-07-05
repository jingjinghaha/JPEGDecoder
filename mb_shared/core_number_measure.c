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
    if(measure_diff_timer > 68) {
        measure_diff_timer -= 68;
    } else {
        measure_diff_timer = 0;
    }
    mk_mon_debug_info(0xBE000007 + mk_get_tile_id());
    mk_mon_debug_info(HI_64(measure_diff_timer));
    mk_mon_debug_info(LO_64(measure_diff_timer));
}

