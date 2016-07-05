#ifndef __MEMMAP_H
#define __MEMMAP_H

// use a memory map to keep offsets consistent!!!
#define ADDR_VAR1 (0)
#define ADDR_VAR2 (ADDR_VAR1 + 17*sizeof(int))
#define ADDR_VAR3 (ADDR_VAR2 + 91*sizeof(float))


#endif
