#pragma once
#define MaxIteration ((int64_t)1<<32)
//#define TestIteration ((int64_t)1<<17)
#define TestIteration ((int64_t)1<<17)
//#define TestIteration 2
#define CacheSizeInMB 30
#define CacheLineSize 64
#include <cinttypes>
void sweep(int64_t i);
void fullSweep();
#ifdef LINUX
//#include <stdint.h>
//int64_t __rdtsc();
#endif
