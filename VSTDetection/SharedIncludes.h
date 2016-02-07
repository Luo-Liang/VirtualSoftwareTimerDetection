#pragma once
#define MaxIteration ((int64_t)1<<32)
#define TestIteration ((int64_t)1<<29)
#define CacheSizeInMB 4
#define CacheLineSize 64
#include <cinttypes>
void sweep(int64_t i);
