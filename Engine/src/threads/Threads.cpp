#include "Threads.hpp"

ThreadPool::ThreadPool(size_t threadCount) : shutdownRequested(false), busyThreads(0)
{
    threads.reserve(threadCount);
    for (size_t i = 0; i < threadCount; ++i)
    {
        threads.emplace_back([this] {
            ThreadWorker worker(this);
            worker();
            });
    }
}

ThreadPool::~ThreadPool() 
{
    Shutdown();
}

size_t ThreadPool::GetBusyThreads() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return busyThreads;
}

size_t ThreadPool::GetTaskCount() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return tasks.size();
}

void ThreadPool::Shutdown() 
{
    {
        std::lock_guard<std::mutex> lock(mutex);
        shutdownRequested = true;
    }
    condition.notify_all();

    for (auto& thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}