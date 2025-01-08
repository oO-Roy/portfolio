// Developer: Roy Steiner.
// File: Waitable Priority queue (C++).
// Reviewer:
// Date:
// Version 1.0.
// Status: 

#include <iostream>
#include <chrono>

#include "wqueue.hpp"
#include "wpqueue.hpp"

using namespace ilrd;
using namespace std;
using namespace std::chrono;

int main()
{
    WQueue<int, WPQueue<int>> w_pq;
    int out_p;

    w_pq.Push(8);
    
    w_pq.Pop(&out_p);
    
    w_pq.Pop(&out_p);

    return 0;
}
