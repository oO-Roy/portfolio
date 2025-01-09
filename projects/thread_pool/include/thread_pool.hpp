// File: Thread Pool (C++).
// Status: APPROVED.

#ifndef __THREAD_POOL_HPP__
#define __THREAD_POOL_HPP__ 

#include <memory>       // std::shared_ptr
#include <thread>       // std::thread::hardware_concurrency
#include <unordered_map>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <future>

#include "wqueue.hpp"   // waitable queue 
#include "wpqueue.hpp"  // priority queue wrapper

class ThreadPool
{
public:
    explicit ThreadPool(size_t numOfThreads 
                                        = std::thread::hardware_concurrency()); // May throw bad_alloc, system_error.
                                                                                // NOTE: hardware_concurrency is a recommendation only and may fail.
                                                                                // In case hardware_concurrency fails default is DEFAULT_NUM_THREADS.
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ~ThreadPool();
    
    class ITask
    {
    public:
        ITask() = default;
        ITask(const ITask&) = default;
        virtual ~ITask() = default;
        virtual void operator()() = 0;
    };
    
    template< class F, class... Args >
    class FutureTask : public ITask
    {
    public:
        FutureTask(std::function<F(Args...)> func);
        FutureTask(const FutureTask&) = default;
        virtual~FutureTask() = default;
        virtual void operator()();
        F GetVal();

    private:
        std::future<F> m_val;
        std::function<F(Args...)> m_func;
    };

    enum Priority {LOW, MID, HIGH};
    void Add(std::shared_ptr<ITask> task, enum Priority prior = LOW);
    void Resume();
    void Pause(); 
    void Stop();
    void SetNumThread(size_t numOfThreads); // may throw system_error

private:
    class WorkerThread;
    class StopTask;
    
    void AddThreads(size_t amount);
    void RemoveThreads(size_t amount);
    std::shared_ptr<WorkerThread> GetWorkerThread();
    
    enum PQPriority {NONE_P = -1, LOW_P, MID_P, HIGH_P, TOP_P};
    enum FlagState {RUN, PAUSE, STOP};
    
    using TaskPair = std::pair<PQPriority, std::shared_ptr<ITask>>;
    
    static const size_t DEFAULT_NUM_THREADS = 8;
    size_t m_numOfThreads;
    std::atomic<FlagState> m_flag;
    std::condition_variable m_cv;
    mutable std::mutex m_mtx;
    
    WQueue<TaskPair, WPQueue<TaskPair>> m_pq;
    std::unordered_map<std::thread::id, std::shared_ptr<WorkerThread>> m_map;
    std::vector<std::shared_ptr<WorkerThread>> m_joinableThreads;
};

#endif //__THREAD_POOLHPP__