//#include <stdio.h>


#include "5kk03.h"
#include "jpeg.h"
#include "surfer.jpg.h"
#define From_host
//extern unsigned int input_buffer[JPGBUFFER_SIZE / sizeof(int)];
extern int vld_count;
volatile unsigned int *jpg_from_host = (unsigned int*)(shared_pt_REMOTEADDR+4*1024*1024);
#ifndef From_host
unsigned int FGETC()
{
	
	unsigned int c = ((surfer_jpg[vld_count / 4] << (8 * (vld_count % 4))) >> 24) & 0x00ff;
	vld_count++;
	return c;
}
#else
unsigned int FGETC()
{
	
	unsigned int c = ((jpg_from_host[vld_count / 4] >> (8 * (vld_count % 4))) ) & 0x00ff;
	vld_count++;
	return c;
}
#endif
int FSEEK(int offset, int start)
{
	vld_count += offset + (start - start);	/* Just to use start... */
	return 0;
}

int FTELL()
{
	return vld_count;
}
