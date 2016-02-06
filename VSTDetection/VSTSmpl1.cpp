#include <intrin.h>
#include "SharedIncludes.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#ifdef LINUX
#include <sched.h>
#include <unistd.h>
#elif WINDOWS
#include <Windows.h>
#endif
using namespace std;

mutex threadConn;
condition_variable cv;
bool ready = false;
bool stop = false;

#ifdef LINUX
int64_t SharedTimer __attribute__((aligned(CacheLineSize))) = 0;
#elif WINDOWS
__declspec(align(CacheLineSize)) int64_t SharedTimer = 0;
#endif
double cyclesPerIteration = 0;

void pinThreadToCore(int coreId, void* handle)
{
#ifdef LINUX
	int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
	if (coreId < 0 || coreId >= num_cores)
		return EINVAL;

	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(coreId, &cpuset);

	pthread_t current_thread = pthread_self();
	return pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);

#elif WINDOWS
	SetThreadAffinityMask(handle, coreId);
#endif
}

void timingWork()
{
	//SharedTimer = 0;
#ifdef LINUX
	pinThreadToCore(4, NULL);
#elif WINDOWS
	pinThreadToCore(4, GetCurrentThread());
#endif
	cout << "----- Collecting performance information for this device -----" << endl;
	int64_t start, end;
	start = __rdtsc();
	while (SharedTimer < MaxIteration)
	{
		SharedTimer++;
	}
	end = __rdtsc();

	cyclesPerIteration = 1.0*(end - start) / MaxIteration;

	cout << "----- iAdd operation: " << cyclesPerIteration << " cycles per op -----" << endl;

	unique_lock<std::mutex> lk(threadConn);
	ready = true;
	//automatic mfence.
	lk.unlock();
	cv.notify_one();
	cout << " ----- Notification -----" << endl;
	while (SharedTimer < INT64_MAX)
	{
		SharedTimer++;
	}
}


int main()
{
#ifdef LINUX
	pinThreadToCore(1, NULL);
#elif WINDOWS
	pinThreadToCore(1, GetCurrentThread());
#endif
	thread timingThread(timingWork);
	int64_t local = 0, local1 = 0, rdtscStart = 0, rdtscEnd = 0;
	rdtscStart = __rdtsc();
	while (local < MaxIteration)
	{
		local = SharedTimer;
		local1 = SharedTimer;
	}
	rdtscEnd = __rdtsc();
	int64_t iterationOverhead = (rdtscEnd - rdtscStart);
	int64_t attempts = SharedTimer;
	int64_t cycleAccumulator = 0;
	int64_t i = 0, start, end;
	unique_lock<std::mutex> lk(threadConn);
	cv.wait(lk, [] {return ready; });
	lk.unlock();
	cv.notify_one();

	while (i < TestIteration)
	{
		start = SharedTimer;
		sweep(i);
		end = SharedTimer;
		if (end > start)
		{
			i++;
			cycleAccumulator += (end - start);
			//cout << i << endl;
		}
	}
	double adjustedOverhead = 1.0 * iterationOverhead * TestIteration / MaxIteration;
	cout << "----- Detected Accuracy " << ((cycleAccumulator - adjustedOverhead) / TestIteration)*cyclesPerIteration << endl;
	cout << "----- Detected Overhead per iteration: " << 1.0 * iterationOverhead / MaxIteration << " -----" << endl;
	stop = true;
	timingThread.join();
	getchar();
	return 0;
}