// File: Shared pointer (C++).
// Status: APPROVED.

#include <iostream>

#include "shared_ptr.hpp"

class Base
{
public:
    Base(int a = 0)
    : m_test(a), m_a(a){}
    int m_test;
private:
    int m_a;
};

class Derived : public Base
{
public:
    Derived(int b = 0)
    : Base(b) ,m_b(b){}
private:
    int m_b;
};

using namespace std;

int Test()
{
    int status = 0;
    
    {
        SharedPtr<Base> a1(new Base); // Base type
        SharedPtr<Base> a2(a1); // Base type
        
        if (a1.GetPtr() != a2.GetPtr())
        {
            cout << "a1.GetPtr() != a2.GetPtr() failed!" << endl;
            status = 1; 
        }
    }

    {
        SharedPtr<Derived> b1(new Derived); // Derived type
        SharedPtr<Base> b2(b1); // Base type

        if (b1.GetPtr() != b2.GetPtr())
        {
            cout << "b1.GetPtr() != b2.GetPtr() failed!" << endl;
            status = 1; 
        }
    }

    {
        SharedPtr<int> c1(new int);
        SharedPtr<int> c2(c1);
        SharedPtr<int> c3(c1);
        SharedPtr<int> c4(c1);

        if (c1.GetPtr() != c2.GetPtr() && c1.GetPtr() != c3.GetPtr()
            && c1.GetPtr() != c4.GetPtr())
        {
            cout << "c1.GetPtr() != c2.GetPtr()" 
                << "&& c1.GetPtr() != c3.GetPtr()" 
                << "&& c1.GetPtr() != c4.GetPtr() failed!" << endl;
            
            status = 1; 
        }
    }
    
    {
        SharedPtr<int> d1(new int);
        SharedPtr<int> d2(d1);
        SharedPtr<int> d3(d2);
        SharedPtr<int> d4(d3);

        if (d1.GetPtr() != d2.GetPtr() && d1.GetPtr() != d3.GetPtr()
            && d1.GetPtr() != d4.GetPtr())
        {
            cout << "d1.GetPtr() != d2.GetPtr()" 
                << "&& d1.GetPtr() != d3.GetPtr()" 
                << "&& d1.GetPtr() != d4.GetPtr() failed!" << endl;
            
            status = 1; 
        }
    }

    {
        int* i_ptr = new int(5);
        SharedPtr<int> e1(i_ptr);

        if (*e1 != *i_ptr)
        {
            cout << "*e1 != *i_ptr" << endl;

            status = 1; 
        }
    }

    {
        SharedPtr<Base> f1(new Base(10));

        if (10 != f1->m_test)
        {
            cout << "10 != f1->m_test" << endl;

            status = 1; 
        }
    }

    return status;
}

int main()
{
    if (Test())
    {
        cout << "Test failed!" << endl;
    }
    
    else
    {
        cout << "Test succeed!" << endl;
    }
    
    return 0;
}
