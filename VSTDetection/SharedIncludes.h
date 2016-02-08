#pragma once
#define MaxIteration ((int64_t)1<<32)
#define TestIteration ((int64_t)1<<17)
#define CacheSizeInMB 6
#define CacheLineSize 64
#include <cinttypes>
void sweep(int64_t i);
void fullSweep();
