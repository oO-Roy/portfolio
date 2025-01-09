// File: Thread Pool (C++).
// Status: APPROVED.

#include <atomic>
#include <future>

#include "thread_pool.hpp"

/// FutureTask class ///////////////////////////////////////////////////////////
template<class F, class... Args>
ThreadPool::FutureTask<F, Args...>::FutureTask(std::function<F(Args...)> func)
: m_func(func)
{
    // empty.
}

template<class F, class... Args>
void ThreadPool::FutureTask<F, Args...>::operator()()
{
    m_val = std::async(std::launch::async, m_func, Args()...);
}

template<class F, class... Args>
F ThreadPool::FutureTask<F, Args...>::GetVal()
{
    return m_val.get();
}


/// WorkerThread class ///////////////////////////////////////////////////////////////
class ThreadPool::WorkerThread
{
public:
    WorkerThread(ThreadPool* tp);
    bool GetFlag();
    void Resume();
    void Stop();
    void Join();
    bool Joinable();
    std::thread::id GetId();
    void ThreadLoop(ThreadPool* tp);

private:
    std::atomic<bool> m_isRunning;
    std::thread m_thread;
};

ThreadPool::WorkerThread::WorkerThread(ThreadPool* tp)
: m_isRunning(true), m_thread(&WorkerThread::ThreadLoop, this, tp)
{
    // empty.
}

bool ThreadPool::WorkerThread::GetFlag()
{
    return m_isRunning.load();
}

void ThreadPool::WorkerThread::Resume()
{
    m_isRunning.store(true, std::memory_order_acquire);
}

void ThreadPool::WorkerThread::Stop()
{
    m_isRunning.store(false, std::memory_order_acquire);
}

void ThreadPool::WorkerThread::Join()
{
    m_thread.join();
}

bool ThreadPool::WorkerThread::Joinable()
{
    return m_thread.joinable();
}

std::thread::id ThreadPool::WorkerThread::GetId()
{
    return m_thread.get_id();
}

void ThreadPool::WorkerThread::ThreadLoop(ThreadPool* tp)
{
    TaskPair task;
    
    while (tp->GetWorkerThread() && m_isRunning.load()) 
    {
        tp->m_pq.Pop(&task);
        if (PAUSE == tp->m_flag.load())
        {
            std::unique_lock<std::mutex> lock(tp->m_mtx);
            tp->m_cv.wait(lock);
        }
        task.second->operator()();
    }
}
 
// Tasks ///////////////////////////////////////////////////////////////////////
class ThreadPool::StopTask : public ITask
{
public:
    StopTask(ThreadPool* tp) : m_tp(tp) {}
    void operator()()
    {
        std::shared_ptr<ThreadPool::WorkerThread> w_thread;
        w_thread = m_tp->GetWorkerThread();
        
        if (w_thread)
        {
            w_thread->Stop();
        }
    }

private:
    ThreadPool* m_tp;
};

// class ThreadPool ////////////////////////////////////////////////////////////
ThreadPool::ThreadPool(size_t numOfThreads)
: m_numOfThreads(numOfThreads ? numOfThreads : DEFAULT_NUM_THREADS)
, m_flag(RUN)
{
    AddThreads(m_numOfThreads);
}

ThreadPool::~ThreadPool()
{
    m_flag.store(STOP, std::memory_order_acquire);
    m_cv.notify_all();

    for (size_t i = 0; i < m_map.size(); ++i)
    {
        std::shared_ptr<ThreadPool::StopTask> stop = 
                        std::make_shared<ThreadPool::StopTask>(this);
        TaskPair task(TOP_P, stop);
        m_pq.Push(task);
    }

    for (auto& pair : m_map)
    {
        pair.second->Join();
    }
}

void ThreadPool::Add(std::shared_ptr<ITask> task, enum Priority prior)
{   
    if (task && STOP != m_flag.load())
    {
        TaskPair pqTask(static_cast<PQPriority>(prior), task);
        m_pq.Push(pqTask);
    }
}

void ThreadPool::Resume()
{
    m_flag.store(RUN, std::memory_order_acquire);
    m_cv.notify_all();
}

void ThreadPool::Pause()
{
    m_flag.store(PAUSE, std::memory_order_acquire);
}

void ThreadPool::Stop()
{
    m_flag.store(STOP, std::memory_order_acquire);

    for (size_t i = 0; i < m_map.size(); ++i)
    {
        std::shared_ptr<ThreadPool::StopTask> stop = 
                            std::make_shared<ThreadPool::StopTask>(this);
        TaskPair task(NONE_P, stop);
        m_pq.Push(task);
    }
}

void ThreadPool::SetNumThread(size_t numOfThreads)
{
    if (numOfThreads)
    {
        int diff = (m_numOfThreads - numOfThreads);

        if (0 < diff)
        {
            RemoveThreads(diff);
        }

        else if (0 > diff)
        {
            AddThreads(-diff);
        }

        m_numOfThreads = numOfThreads;
    }
}

void ThreadPool::AddThreads(size_t amount)
{
    for (size_t i = 0; i < amount; ++i)
    {
        std::shared_ptr<WorkerThread> thread = 
                                        std::make_shared<WorkerThread>(this);

        std::unique_lock<std::mutex> lock(m_mtx);
        m_map.emplace(thread->GetId(), thread);
    }
}

void ThreadPool::RemoveThreads(size_t amount)
{
    size_t i = 0;
    
    m_flag.store(STOP, std::memory_order_acquire);

    for (auto pair = m_map.begin(); i < amount; ++pair, ++i)
    {
        std::shared_ptr<ThreadPool::StopTask> stop = 
                        std::make_shared<ThreadPool::StopTask>(this);
        
        TaskPair task(TOP_P, stop);
        m_pq.Push(task);

        if (pair->second->Joinable()) 
        {
            m_joinableThreads.push_back(pair->second);
        }
    }

    m_flag.store(RUN, std::memory_order_acquire);
}

std::shared_ptr<ThreadPool::WorkerThread> ThreadPool::GetWorkerThread()
{   
    std::unique_lock<std::mutex> lock(m_mtx);
    auto iterator = m_map.find(std::this_thread::get_id());

    return ((iterator != m_map.end()) ? (iterator->second) : NULL);
}