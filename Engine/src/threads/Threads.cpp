#include "Threads.hpp"

ThreadPool::ThreadPool(size_t threadCount) : m_shutdownRequested(false), m_busyThreadCount(0)
{
    m_threads.reserve(threadCount);
    for (size_t i = 0; i < threadCount; ++i)
    {
        m_threads.emplace_back([this] {
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
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_busyThreadCount;
}

size_t ThreadPool::GetTaskCount() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_tasks.size();
}

void ThreadPool::Shutdown() 
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_shutdownRequested = true;
    }
    m_condition.notify_all();

    for (auto& thread : m_threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}