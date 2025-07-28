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
            std::lock_guard<std::mutex> lock(mutex);
            if (shutdownRequested) 
            {
                throw std::runtime_error("ThreadPool is shutting down");
            }
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
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
        ThreadWorker(ThreadPool* pool) : pool(pool) {}

        void operator()() {
            std::unique_lock<std::mutex> lock(pool->mutex);

            while (!pool->shutdownRequested || !pool->tasks.empty()) 
            {
                // Wait for task or shutdown signal
                pool->condition.wait(lock, [this] {
                    return !this->pool->tasks.empty() || this->pool->shutdownRequested;
                });

                if (this->pool->shutdownRequested && this->pool->tasks.empty())
                {
                    break;
                }

                if (!this->pool->tasks.empty())
                {
                    auto task = std::move(pool->tasks.front());
                    pool->tasks.pop();
                    ++pool->busyThreads;

                    lock.unlock();
                    task();
                    lock.lock();

                    --pool->busyThreads;
                }
            }
        }

    private:
        ThreadPool* pool;
    };

    mutable std::mutex mutex;
    std::condition_variable condition;

    std::vector<std::thread> threads;
    size_t busyThreads;
    
    std::queue<std::function<void()>> tasks;
    
    bool shutdownRequested;
};