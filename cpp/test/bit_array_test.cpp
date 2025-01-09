// File: Bit Array (C++).
// Status: APPROVED.

#include <iostream>

#include "bit_array.hpp"

using namespace std;

bool Foo(const BitArr<8>& bit_arr)
{
    if (!bit_arr[0])
    {
        return true;
    }

    return false;
}

bool EvilFoo(const BitArr<8>& bit_arr)
{
    if (bit_arr[100] || !bit_arr[100])
    {
        return false;
    } 

    return false;
}

int Test()
{
    int status = 0;
    
/// BitProxy Tests /////////////////////////////////////////////////////////////    
    
    {   // BitProxy operator bool    
        BitArr<8> bit_arr;

        if (bit_arr[0])
        {
            cout << "bit_arr[0]" << endl;
            status = true;
        }
    }

    {   // BitProxy operator !
        BitArr<8> bit_arr;
        
        bit_arr[0] = true;

        if (!bit_arr[0])
        {
            cout << "!bit_arr[0]" << endl;
            status = true;
        }
    }

    {   // BitProxy operator = (val)    
        BitArr<8> bit_arr;

        bit_arr[0] = true;

        if (true != bit_arr[0])
        {
            cout << "bit_arr[0] = true" << endl;
            status = true;
        }
    }

    {   // BitProxy operator = (other) 
        BitArr<8> bit_arr1;
        BitArr<8> bit_arr2;

        bit_arr1[0] = true;
        bit_arr2[0] = bit_arr1[0];

        if (bit_arr1[0] != bit_arr2[0])
        {
            cout << "bit_arr2[0] = bit_arr1[0]" << endl;
            status = true;
        }
    }

/// BitArr Tests ///////////////////////////////////////////////////////////////

    {   // operator &=
        BitArr<10> bit_arr1;
        BitArr<10> bit_arr2;

        bit_arr1[0] = true;
        bit_arr1[9] = true;

        bit_arr2[9] = true;

        bit_arr1 &= bit_arr2;

        if ((false == bit_arr1[9]) || (true == bit_arr1[0]))
        {
            cout << "bit_arr1 &= bit_arr2" << endl;
            status = true;
        }
    }
    
    {   // operator |=
        BitArr<4> bit_arr1;
        BitArr<4> bit_arr2;

        bit_arr1[0] = true;
        bit_arr1[2] = true;

        bit_arr2[1] = true;
        bit_arr2[3] = true;

        bit_arr1 |= bit_arr2;

        if ((false == bit_arr1[0]) || (false == bit_arr1[1])
            || (false == bit_arr1[2]) || (false == bit_arr1[3]))
        {
            cout << "bit_arr1 |= bit_arr2" << endl;
            status = true;
        }
    }
    
    {   // operator ^=
        BitArr<5> bit_arr1;
        BitArr<5> bit_arr2;

        bit_arr1[0] = true;
        bit_arr1[2] = true;

        bit_arr2[1] = true;
        bit_arr2[3] = true;

        bit_arr1 ^= bit_arr2;

        if ((true == bit_arr1[4]) 
                        || (false == bit_arr1[0]) || (false == bit_arr1[1])
                        || (false == bit_arr1[2]) || (false == bit_arr1[3]))
        {
            cout << "bit_arr1 ^= bit_arr2" << endl;
            status = true;
        }
    }
    
    {   // operator [] const
        BitArr<8> bit_arr;

        bit_arr[0] = true;

        if (Foo(bit_arr))
        {
            cout << "bit_arr[] const" << endl;
            status = true;
        }
    }

    {   // operator [] const
        BitArr<8> bit_arr;

        bit_arr[0] = true;

        if (EvilFoo(bit_arr))
        {
            cout << "bit_arr[] const" << endl;
            status = true;
        }
    }

    {   // Set (val, pos)
        BitArr<8> bit_arr;

        bit_arr.Set(true, 0);

        if (!bit_arr[0])
        {
            cout << "bit_arr.Set(val, pos)" << endl;
            status = true;
        }
    }

    {   // Set (val)
        BitArr<8> bit_arr;

        bit_arr.Set(true);

        if (!bit_arr[0])
        {
            cout << "bit_arr.Set(val)" << endl;
            status = true;
        }
    }
    
    {   // Flip (pos)
        BitArr<8> bit_arr;

        bit_arr.Flip(0);
        
        if (!bit_arr[0])
        {
            cout << "bit_arr.Flip(pos)" << endl;
            status = true;
        }
    }
    
    {   // Flip ()
        BitArr<4> bit_arr;

        bit_arr.Flip();

        if (!bit_arr[0] || !bit_arr[1] || !bit_arr[2] || !bit_arr[3])
        {
            cout << "bit_arr.Flip()" << endl;
            status = true;
        }
    }

    {   // Get
        BitArr<8> bit_arr;

        bit_arr[0] = true;

        if (!bit_arr.Get(0))
        {
            cout << "bit_arr.Get()" << endl;
            status = true;
        }
    }

    {   // Count
        BitArr<23> bit_arr;

        bit_arr[0] = true;
        bit_arr[2] = true;

        bit_arr[8] = true;
        bit_arr[10] = true;

        bit_arr[19] = true;
        bit_arr[21] = true;
        bit_arr[22] = true;
        bit_arr[23] = true;

        if (7 != bit_arr.Count())
        {
            cout << "bit_arr.Count()" << endl;
            status = true;
        }
    }

    {
        BitArr<1> bit_arr;

        bit_arr.Set(true);   

        if (1 != bit_arr.Count())
        {
            cout << "bit_arr.Count() " << bit_arr.Count() << endl;
            status = true;
        }
    }

    {
        BitArr<1> bit_arr; 

        if (0 != bit_arr.Count())
        {
            cout << "bit_arr.Count()" << endl;
            status = true;
        }
    }

    {   // ToString
        BitArr<3> bit_arr;

        bit_arr[0] = true;
        bit_arr[1] = true;
        bit_arr[3] = true;
        bit_arr[4] = true;
        bit_arr[6] = true;
        bit_arr[7] = true;
        bit_arr[9] = true;
        bit_arr[10] = true;
        bit_arr[12] = true;
        bit_arr[13] = true;
        bit_arr[15] = true;

        // cout << bit_arr.ToString() << endl;
    }

    {   // operator <<=
    }

    {   // operator >>=

    }
    
    {   // operator ==, operator !=
        BitArr<8> bit_arr1;
        BitArr<8> bit_arr2;

        bit_arr1[0] = true;
        bit_arr2[1] = true;

        if ((bit_arr1 == bit_arr2) || !(bit_arr1 != bit_arr2))
        {
            cout << "(bit_arr1 == bit_arr2) && !(bit_arr1 != bit_arr2)" << endl;
            status = true;
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
