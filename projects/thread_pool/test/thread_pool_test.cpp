// File: Thread Pool (C++).
// Status: APPROVED. 

#include <iostream>

#include "thread_pool.hpp"

#define RED     "\033[0;31m"
#define BRED    "\033[1;31m"
#define BGRN    "\033[1;32m"
#define RESET   "\033[0m"

using namespace std;

class Add1: public ThreadPool::ITask
{
public:
    Add1(int* sum)
    : m_sum(sum){}

    void operator()()
    {
        *m_sum += 1;
    }

private:
    int* m_sum;
};

size_t TestAddition(size_t numOfThreads)
{
    int sum = 0;
    size_t fails = 0;
    size_t numOfTasks = 100;
    ThreadPool tp(numOfThreads);

    for (size_t i = 0; i < numOfTasks; ++i)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        tp.Add(std::make_shared<Add1>(&sum), ThreadPool::LOW);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    if (100 != sum)
    {
        ++fails;
        std::cout << BRED << "=========================\n";
        std::cout << "| TestAddition Failed " << RESET << std::endl;
        std::cout << BRED << "|" << RESET << " Num of threads: " << numOfThreads 
                                                                << std::endl;
        std::cout << BRED << "|" << RESET << " Expected: 100, " << "Got: " 
                                                            << sum << std::endl;
        std::cout << BRED << "=========================" << RESET 
                                                                << std::endl;
    }
    
    return fails;
}

size_t TestStop(size_t numOfThreads)
{
    int sum = 0;
    size_t fails = 0;
    size_t numOfTasks = 100;
    ThreadPool tp(numOfThreads);

    for (size_t i = 0; i < numOfTasks; ++i)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        tp.Add(std::make_shared<Add1>(&sum), ThreadPool::LOW);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    tp.Stop();

    for (size_t i = 0; i < numOfTasks; ++i)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        tp.Add(std::make_shared<Add1>(&sum), ThreadPool::LOW);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    if (100 != sum)
    {
        ++fails;
        std::cout << BRED << "=========================\n";
        std::cout << "| TestStop Failed " << RESET << std::endl;
        std::cout << BRED << "|" << RESET << " Num of threads: " << numOfThreads 
                                                                << std::endl;
        std::cout << BRED << "|" << RESET << " Expected: 100, " << "Got: " 
                                                            << sum << std::endl;
        std::cout << BRED << "=========================" << RESET 
                                                                << std::endl;
    }
    
    return fails;
}

size_t TestPause(size_t numOfThreads)
{
    int sum = 0;
    size_t fails = 0;
    size_t numOfTasks = 100;
    ThreadPool tp(numOfThreads);

    for (size_t i = 0; i < numOfTasks; ++i)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        tp.Add(std::make_shared<Add1>(&sum), ThreadPool::LOW);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    tp.Pause();

    for (size_t i = 0; i < numOfTasks; ++i)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        tp.Add(std::make_shared<Add1>(&sum), ThreadPool::LOW);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    if (100 != sum)
    {
        ++fails;
        std::cout << BRED << "=========================\n";
        std::cout << "| TestPause Failed " << RESET << std::endl;
        std::cout << BRED << "|" << RESET << " Num of threads: " << numOfThreads 
                                                                << std::endl;
        std::cout << BRED << "|" << RESET << " Expected: 100, " << "Got: " 
                                                            << sum << std::endl;
        std::cout << BRED << "=========================" << RESET 
                                                                << std::endl;
    }
    
    return fails;
}

size_t TestPauseAndResume(size_t numOfThreads)
{
    int sum = 0;
    size_t fails = 0;
    size_t numOfTasks = 100;
    ThreadPool tp(numOfThreads);

    for (size_t i = 0; i < (numOfTasks / 2); ++i)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        tp.Add(std::make_shared<Add1>(&sum), ThreadPool::LOW);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    tp.Pause();

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    tp.Resume();

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    for (size_t i = 0; i < (numOfTasks / 2); ++i)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        tp.Add(std::make_shared<Add1>(&sum), ThreadPool::LOW);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    if (100 != sum)
    {
        ++fails;
        std::cout << BRED << "=========================\n";
        std::cout << "| TestPause Failed " << RESET << std::endl;
        std::cout << BRED << "|" << RESET << " Num of threads: " << numOfThreads 
                                                                << std::endl;
        std::cout << BRED << "|" << RESET << " Expected: 100, " << "Got: " 
                                                            << sum << std::endl;
        std::cout << BRED << "=========================" << RESET 
                                                                << std::endl;
    }
    
    return fails;
}

size_t TestSetNumOfThreadsHigher(size_t numOfThreads)
{
    int sum = 0;
    size_t fails = 0;
    size_t numOfTasks = 100;
    ThreadPool tp(numOfThreads);

    for (size_t i = 0; i < (numOfTasks / 2); ++i)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        tp.Add(std::make_shared<Add1>(&sum), ThreadPool::LOW);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    tp.SetNumThread(numOfThreads * 2);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

        for (size_t i = 0; i < (numOfTasks / 2); ++i)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        tp.Add(std::make_shared<Add1>(&sum), ThreadPool::LOW);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    if (100 != sum)
    {
        ++fails;
        std::cout << BRED << "=========================\n";
        std::cout << "| TestSetNumOfThreadsHigher Failed " << RESET 
                                                                << std::endl;
        std::cout << BRED << "|" << RESET << " Num of threads: " << numOfThreads 
                                                                << std::endl;
        std::cout << BRED << "|" << RESET << " Expected: 100, " << "Got: " 
                                                            << sum << std::endl;
        std::cout << BRED << "=========================" << RESET 
                                                                << std::endl;
    }
    
    return fails;   
}

size_t TestSetNumOfThreadsLower(size_t numOfThreads)
{
    int sum = 0;
    size_t fails = 0;
    size_t numOfTasks = 100;
    ThreadPool tp(numOfThreads);

    for (size_t i = 0; i < (numOfTasks / 2); ++i)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        tp.Add(std::make_shared<Add1>(&sum), ThreadPool::LOW);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    tp.SetNumThread(numOfThreads / 2);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

        for (size_t i = 0; i < (numOfTasks / 2); ++i)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        tp.Add(std::make_shared<Add1>(&sum), ThreadPool::LOW);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    if (100 != sum)
    {
        ++fails;
        std::cout << BRED << "=========================\n";
        std::cout << "| TestSetNumOfThreadsLower Failed " << RESET 
                                                                << std::endl;
        std::cout << BRED << "|" << RESET << " Num of threads: " << numOfThreads 
                                                                << std::endl;
        std::cout << BRED << "|" << RESET << " Expected: 100, " << "Got: " 
                                                            << sum << std::endl;
        std::cout << BRED << "=========================" << RESET 
                                                                << std::endl;
    }
    
    return fails;   
}

int main()
{
    size_t fails = 0;
    size_t RangeOfThreadsToTest = 32;
    
    for (size_t i = 0; i <= RangeOfThreadsToTest; ++i)
    {
        fails += (TestAddition(i));
    }

    for (size_t i = 0; i <= RangeOfThreadsToTest; ++i)
    {
        fails += (TestStop(i));
    }

    for (size_t i = 0; i <= RangeOfThreadsToTest; ++i)
    {
        fails += (TestPause(i));
    }

    for (size_t i = 0; i <= RangeOfThreadsToTest; ++i)
    {
        fails += (TestPauseAndResume(i));
    }

    for (size_t i = 0; i <= RangeOfThreadsToTest; ++i)
    {
        fails += (TestSetNumOfThreadsHigher(i));
    }

    for (size_t i = 0; i <= RangeOfThreadsToTest; ++i)
    {
        fails += (TestSetNumOfThreadsLower(i));
    }

    if (fails)
    {
        cout << BRED << "Test failed!" << RESET << endl;
    }

    else
    {
        cout << BGRN << "Test succeed!" << RESET << endl;
    } 
    
    return 0;
}
