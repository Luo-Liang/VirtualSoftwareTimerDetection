#include <intrin.h>
#include "SharedIncludes.h"
#include <iostream>
using namespace std;

#ifdef LINUX
int64_t cacheFiller[CacheSizeInMB * 1024 * 1024 / 8] __attribute__((aligned(CacheLineSize)));
#elif WINDOWS
__declspec(align(CacheLineSize)) int64_t cacheFiller[CacheSizeInMB * 1024 * 1024 / 8];
#endif

int64_t additionalStub;

void sweep(int64_t i)
{
	int64_t readerStub = additionalStub;
}

void fullSweep()
{
	int64_t readerStub = 0;
	for (int i = 0; i < CacheSizeInMB * 1024 * 1024 / CacheLineSize; i++)
	{
		readerStub = cacheFiller[i * 8];
	}
}

int __main()
{
	int64_t accumulator = 0;
	//we do not want to waste time on seeking, although linear traversal is fast.
	//we want super fast.
	int64_t rdtscNoSeek = __rdtsc();
	for (int64_t i = 0; i < TestIteration; i++)
	{
		sweep(i);
	}
	accumulator += __rdtsc() - rdtscNoSeek;
	cout << " ------ Average maximum accuracy no seek " << accumulator*1.0 / TestIteration << " ------" << endl;
	accumulator = 0;
	for (int64_t i = 0; i < TestIteration; i++)
	{
		fullSweep();
		rdtscNoSeek = __rdtsc();
		sweep(i);
		accumulator += __rdtsc() - rdtscNoSeek;
	}
	cout << " ------ Average maximum accuracy full seek " << accumulator*1.0 / TestIteration << " ------" << endl;
	getchar();
	return 0;
}