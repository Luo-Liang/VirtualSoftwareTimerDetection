#ifdef LINUX
#include <sched.h>
#include <unistd.h>
#include <stdint.h>
#include <x86intrin.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>
#elif WINDOWS
#include <intrin.h>
#include <Windows.h>
#endif
#include <atomic>
#include "SharedIncludes.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

mutex threadConn;
condition_variable cv;
bool ready = false;
bool stop = false;

#ifdef LINUX
volatile int64_t SharedTimer __attribute__((aligned(CacheLineSize))) = 0;
#elif WINDOWS
__declspec(align(CacheLineSize)) int64_t SharedTimer = 0;
#endif
double cyclesPerIteration = 0;

void pinThreadToCore(int coreId, void* handle)
{
#ifdef LINUX
	int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
	if (coreId < 0 || coreId >= num_cores)
                perror("too many cores\n");

	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(coreId, &cpuset);

	pthread_t current_thread = pthread_self();
	pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
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
	//cout << "----- Collecting performance information for this device -----" << endl;
	int64_t start, end;
	start = __rdtsc();
	while (SharedTimer < MaxIteration)
	{
		SharedTimer++;
	}
	end = __rdtsc();

	cyclesPerIteration = 1.0*(end - start) / MaxIteration;

	//cout << "----- iAdd operation: " << cyclesPerIteration << " cycles per op -----" << endl;

	unique_lock<std::mutex> lk(threadConn);
	ready = true;
	//automatic mfence.
	lk.unlock();
	cv.notify_one();
	//cout << " ----- Notification -----" << endl;
//	while (SharedTimer < INT64_MAX)
	while (!stop)
	{
		SharedTimer++;
	}
}

#ifdef LINUX
void timer_handler(int signum)
{
    exit(0);
}
#endif


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

#ifdef LINUX
        // Add a timer here
        struct sigaction sa;
        struct itimerval timer;

        // Install timer handler as the singal hander for SIGALRM
        memset(&sa, 0x00, sizeof(sa));
        sa.sa_handler = &timer_handler;
        sigaction(SIGALRM, &sa, NULL);

        // Configure the timer to expire after 1 milliseconds
        timer.it_value.tv_sec = 20;
        timer.it_value.tv_usec = 0;

        // Configure the iterative timer
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_usec = 0;

        // Set the timer
        setitimer(ITIMER_REAL, &timer, NULL);
#endif

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
	//cout << "----- Detected Overhead per iteration: " << 1.0 * iterationOverhead / MaxIteration << " -----" << endl;
	double adjustedOverhead = 1.0 * iterationOverhead * TestIteration / MaxIteration;
	//cout << "----- Detected Accuracy " << ((cycleAccumulator - adjustedOverhead) / TestIteration)*cyclesPerIteration << " -----" << endl;
	i = 0;
	cycleAccumulator = 0;
	while (i < TestIteration)
	{
                //cout << "Iteration:" << i << endl;
		fullSweep();
		start = SharedTimer;
		sweep(i);
		end = SharedTimer;
		if (end > start)
		{
			i++;
			cycleAccumulator += (end - start);
		}
                //cout << "Iteration END:" << i << endl;
                //cout << "Start: " << start << endl;
                //cout << "End: " << end << endl;
	}
	adjustedOverhead = 1.0 * iterationOverhead * TestIteration / MaxIteration;
	//cout << "----- Detected Accuracy Miss " << ((cycleAccumulator - adjustedOverhead) / TestIteration)*cyclesPerIteration << " -----" << endl;
	stop = true;
	timingThread.join();
	return 0;
}
