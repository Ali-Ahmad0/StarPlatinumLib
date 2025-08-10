#pragma once

#include <mutex>
#include <future>
#include <vector>
#include <queue>
#include <utility>
#include <functional>
#include <thread>
#include <condition_variable>
#include <stdexcept>
#include <memory>

class ThreadPool
{
public:
    // Initialize thread pool with specified number of threads (defaults to CPU cores)
    ThreadPool(size_t threadCount = std::thread::hardware_concurrency());

    // Clean up threads on destruction
    ~ThreadPool();


    // Add a task to be executed by the thread pool
    template <typename F, typename... Args>
    auto AddTask(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
    {
        using ReturnType = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        auto result = task->get_future();
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_shutdownRequested) 
            {
                throw std::runtime_error("[RUNTIME ERROR]: ThreadPool is shutting down");
            }
            m_tasks.emplace([task]() { (*task)(); });
        }
        m_condition.notify_one();
        return result;
    }

    // Get number of currently working threads
    size_t GetBusyThreads() const;

    // Get number of pending tasks
    size_t GetTaskCount() const;

    // Stop the thread pool and wait for all threads
    void Shutdown();

private:
    class ThreadWorker {
    public:
        ThreadWorker(ThreadPool* pool) : m_pool(pool) {}

        void operator()() {
            std::unique_lock<std::mutex> lock(m_pool->m_mutex);

            while (!m_pool->m_shutdownRequested || !m_pool->m_tasks.empty()) 
            {
                // Wait for task or shutdown signal
                m_pool->m_condition.wait(lock, [this] {
                    return !this->m_pool->m_tasks.empty() || this->m_pool->m_shutdownRequested;
                });

                if (this->m_pool->m_shutdownRequested && this->m_pool->m_tasks.empty())
                {
                    break;
                }

                if (!this->m_pool->m_tasks.empty())
                {
                    auto task = std::move(m_pool->m_tasks.front());
                    m_pool->m_tasks.pop();
                    ++m_pool->m_busyThreadCount;

                    lock.unlock();
                    task();
                    lock.lock();

                    --m_pool->m_busyThreadCount;
                }
            }
        }

    private:
        ThreadPool* m_pool;
    };

    mutable std::mutex m_mutex;
    std::condition_variable m_condition;

    std::vector<std::thread> m_threads;
    size_t m_busyThreadCount;
    
    std::queue<std::function<void()>> m_tasks;
    
    bool m_shutdownRequested;
};