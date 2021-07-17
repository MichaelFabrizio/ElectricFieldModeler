#include "ThreadPool.h"
#include <thread>

ThreadPool::ThreadPool()
{
	maxThreads_ = std::thread::hardware_concurrency();
	//Spin up threads at start??
}

// One option is to have the threadpool handle execution internally
// Possible challenge: Difficulty with different input parameters/return types?
// Must be generic

//void ThreadPool::PerformJob(void (*f)(int))
//{
//	// Threadpool decides which thread to call
//}
//
//// Another option is to rent out the thread, maybe some kind of scoped pointer
//// Threadpool needs a bool for which threads are rented
//std::thread ThreadPool::RequestThread()
//{
//}


