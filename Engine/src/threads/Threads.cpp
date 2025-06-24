//#include "Threads.hpp"
//
//void ThreadWorker::operator()()
//{
//	// Create a lock on the mutex
//	std::unique_lock<std::mutex> lock(pool->mutex);
//	
//	while (!pool->shutdownRequested || (pool->shutdownRequested && !pool->tasks.empty())) 
//	{
//		pool->busyThreads--;
//
//		// Condition variable releases mutex taken by the lock
//		// If mutex is woken up, check condition
//		// If condition is true, continue executing, otherwise sleep
//		pool->condition.wait(lock, [this] 
//		{
//			return this->pool->shutdownRequested || !this->pool->tasks.empty();
//		});
//		pool->busyThreads++;
//
//		// Process the queue if not empty
//		if (!this->pool->tasks.empty()) 
//		{
//			auto func = pool->tasks.front();
//			pool->tasks.pop();
//
//			lock.unlock();
//			func();
//			lock.lock();
//		}
//	}
//}