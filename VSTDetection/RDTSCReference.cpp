#include <intrin.h>
#include "SharedIncludes.h"
#include <iostream>
using namespace std;

int64_t cacheFiller[CacheSizeInMB * 1024 * 1024 / 8];
int64_t readerStub;

void sweep(int64_t i)
{
	readerStub = cacheFiller[i % (CacheSizeInMB * 1024 * 1024 / 8)];
}


int main()
{
	int64_t accumulator = 0;
	//we do not want to waste time on seeking, although linear traversal is fast.
	//we want super fast.
	int64_t rdtscNoSeek = 0;
	int64_t rdtscNoSeekEnd = 0;

	for (int64_t i = 0; i < MaxIteration; i++)
	{
		rdtscNoSeek = __rdtsc();
		sweep(i);
		accumulator += __rdtsc() - rdtscNoSeek;
	}

	cout << " ------ Average maximum accuracy " << accumulator*1.0 / MaxIteration << " ------" << endl;
	getchar();
	return 0;
}