/*-----------------------------------------*/
/* File : utils.c, utilities for jfif view */
/* Author : Pierre Guerrier, march 1998	   */
/*-----------------------------------------*/



#include "jpeg.h"

/*-------------------------------------------*/

void aborted_stream(int debug)
{
	mk_mon_debug_info(debug);
	mk_mon_debug_tile_finished();
}


/*----------------------------------------------------------*/

/* Returns ceil(N/D). */
int ceil_div(int N, int D)
{
	int i = N / D;

	if (N > D * i)
		i++;
	return i;
}

/* Returns floor(N/D). */
int floor_div(int N, int D)
{
	int i = N / D;

	if (N < D * i)
		i--;
	return i;
}

/*----------------------------------------------------------*/

/* For all components reset DC prediction value to 0. */
void reset_prediction(void)
{
	int i;

	for (i = 0; i < 3; i++)
		comp[i].PRED = 0;
}
/*---------------------------------------------------------*/

/* Transform JPEG number format into usual 2's-complement format. */
int reformat(unsigned long S, int good)
{
	int St;

	if (!good)
		return 0;
	St = 1 << (good - 1);	/* 2^(good-1) */
	if (S < (unsigned long)St)
		return (S + 1 + ((-1) << good));
	else
		return S;
}


