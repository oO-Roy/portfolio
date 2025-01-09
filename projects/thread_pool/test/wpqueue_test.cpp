// File: Waitable Priority queue (C++).
// Status: APPROVED.

#include <iostream>
#include <chrono>

#include "wqueue.hpp"
#include "wpqueue.hpp"

using namespace std;
using namespace std::chrono;

int main()
{
    WQueue<int, WPQueue<int>> w_pq;
    int out_p;

    w_pq.Push(8);
    
    w_pq.Pop(&out_p);
    
    // w_pq.Pop(&out_p);

    return 0;
}
