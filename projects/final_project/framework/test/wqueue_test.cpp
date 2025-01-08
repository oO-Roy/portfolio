// Developer: Roy Steiner.
// File: Waitable queue (C++).
// Reviewer: Irena Aleshin,
// Date: 03/12/2024
// Version 1.1.
// Status: APPROVED.

#include <iostream>
#include <queue>
#include <thread>
#include <vector>
#include <atomic>
#include "wqueue.hpp"

using namespace ilrd;
using namespace std;

atomic<int> i(0);
atomic<int> counter(0);

int Test2()
{
    WQueue<int> q;
    vector<thread> push_threads;
    vector<thread> pop_threads;

    auto push = [](WQueue<int>& q)
    {
        q.Push(i.fetch_add(1));
    };
    
    auto pop = [](WQueue<int>& q)
    {
        int out_p;

        if (SUCCESS != q.Pop(&out_p, chrono::milliseconds(1000)))
        {
            cout << "Threaded pop failed" << endl;
        }

        else
        {
            ++counter;
        }
    };

    for (size_t i = 0; i < 10; ++i)
    {
        push_threads.push_back(thread(push, ref(q)));
    }

    for (auto& thread : push_threads)
    {
        thread.join();
    }

    for (size_t i = 0; i < 10; ++i)
    {
        pop_threads.push_back(thread(pop, ref(q)));
    }

    for (auto& thread : pop_threads)
    {
        thread.join();
    }

    return !(counter == i);
}

int Test()
{
    int status = 0;
    
    {  
        WQueue<int> w_q;
        int out_p;
    
        w_q.Push(10);
        w_q.Push(5);
        w_q.Pop(&out_p);

        if (10 != out_p)
        {
            cout << "Push or Pop failed" << endl;
            status = true;
        }

        w_q.Pop(&out_p, chrono::milliseconds(1000));

        if (5 != out_p)
        {
            cout << "Push or Pop failed" << endl;
            status = true;
        }

        if (!w_q.IsEmpty())
        {
            cout << "IsEmpty failed" << endl;
            status = true;
        }

        if (TIME_OUT_EMPTY != w_q.Pop(&out_p, chrono::milliseconds(1000)))
        {
            cout << "Pop(out_p, timeout) failed" << endl;
            status = true; 
        }
    }

    return status;
}

int main()
{
    if (Test() || Test2())
    {
        cout << "Test failed!" << endl;
    }
    
    else
    {
        cout << "Test succeed!" << endl;
    }

    return 0;
}
