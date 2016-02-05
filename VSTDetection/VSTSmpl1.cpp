#include <intrin.h>
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

int64_t SharedTimer = 0;
double cyclesPerIteration = 0;

void timingWork()
{
	cout << "----- Making sure Turbo Boost(TM) and EIST(TM) are stablized -----" << endl;
	for (int64_t i = 0; i < MaxIteration; i++)
	{
		SharedTimer = 0;
	}
	SharedTimer = 0;
	cout << "----- Collecting performance information for this device -----" << endl;
	int64_t start = __rdtsc();
	for (int64_t i = 0; i < MaxIteration; i++)
	{
		SharedTimer++;
	}
	int64_t end = __rdtsc();

	cyclesPerIteration = MaxIteration *1.0 / (end - start);

	cout << "----- iAdd operation: " << cyclesPerIteration << " cycles per op -----" << endl;

	unique_lock<std::mutex> lk(threadConn);
	ready = true;
	//automatic mfence.
	lk.unlock();
	cv.notify_one();
	cout << " ----- Notification -----" << endl;
	while (!stop)
	{
		SharedTimer++;
	}
}


int __main()
{
	thread timingThread(timingWork);
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
		if (end != start)
		{
			i++;
			cycleAccumulator += (end - start);
			//cout << i << endl;
		}
	}
	cout << "----- Detected Accuracy " << (cycleAccumulator*1.0 / TestIteration)*cyclesPerIteration << endl;
	stop = true;
	timingThread.join();
	getchar();
	return 0;
}