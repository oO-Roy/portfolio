#include <iostream>
#include <string>
#include <cstring>

#include "factory.hpp"
#include "thread_pool.hpp"

#define RED     "\033[0;31m"
#define BRED    "\033[1;31m"
#define BGRN    "\033[1;32m"
#define RESET   "\033[0m"

using namespace ilrd;
using namespace std;

size_t g_sum;

class ICommand
{
public:
    virtual void operator()() = 0;
};

class WriteCommand : public ICommand
{
public:
    WriteCommand(char* src, char* dest)
    : m_src(src), m_dest(dest){} 
    void operator()()
    {
        strcpy(m_dest, m_src);
    }

private:
    char* m_src;
    char* m_dest;
};

std::shared_ptr<WriteCommand> WriteCMDCreator(char* src, char* dest)
{
    return std::make_shared<WriteCommand>(src, dest);
}

class IShape : public ThreadPool::ITask
{
public:
    virtual void operator()() = 0;
    virtual int get() = 0;
};

class Circle : public IShape
{
public:
    Circle() : m_circ(1) {}
    void operator()() {g_sum += m_circ;}
    int get() { return m_circ; }

private:
    int m_circ;
};

class Triangle : public IShape
{
public:
    Triangle() : m_tri(3) {}
    void operator()() {g_sum += m_tri;}
    int get() { return m_tri; }

private:
    int m_tri;
};

class Rectangle : public IShape
{
public:
    Rectangle() : m_rec(4) {}
    void operator()() {g_sum += m_rec;}
    int get() { return m_rec; }
private:
    int m_rec;
};

template <typename T>
std::shared_ptr<T> Creator()
{
    return std::make_shared<T>();
}

size_t WriteReadTest()
{
    size_t errors = 0;

    char memory[1024] = {"Hello World!"};
    char file[1024] = {0};
    
    Factory<int, ICommand, char*, char*> factory;

    factory.Add(0, WriteCMDCreator);

    factory.Create(0, memory, file).get()->operator()();

    errors += strcmp(memory, file);

    return errors;
}

size_t ThreadsTest()
{
    size_t errors = 0;
    
    ThreadPool tp(16);
    Factory<int, IShape> factory;

    factory.Add(0, Creator<Circle>);
    factory.Add(1, Creator<Triangle>);
    factory.Add(2, Creator<Rectangle>);

    for (size_t i = 0; i < 20; ++i)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        tp.Add(factory.Create(0), ThreadPool::LOW);
    }
    
    for (size_t i = 0; i < 30; ++i)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        tp.Add(factory.Create(1), ThreadPool::LOW);
    }

    for (size_t i = 0; i < 10; ++i)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        tp.Add(factory.Create(2), ThreadPool::LOW);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    if (g_sum != (1*20 + 3*30 + 4*10))
    {
        ++errors;
    }

    return errors;
}

size_t GeneralTest()
{
    size_t errors = 0;

    Factory<int, IShape> factory; 

    factory.Add(0, Creator<Circle>);
    factory.Add(1, Creator<Triangle>);
    factory.Add(2, Creator<Rectangle>);

    errors += (1 != factory.Create(0).get()->get());
    errors += (1 != factory.Create(0).get()->get());
    errors += (3 != factory.Create(1).get()->get());
    errors += (3 != factory.Create(1).get()->get());
    errors += (4 != factory.Create(2).get()->get());
    errors += (4 != factory.Create(2).get()->get());
    
    return errors;
}

int main()
{
    int errors = 0;

    errors += GeneralTest();
    errors += ThreadsTest();
    errors += WriteReadTest();
    
    if (errors)
    {
        cout << BRED << "Test failed!" << RESET << endl;
    }

    else
    {
        cout << BGRN << "Test succeed!" << RESET << endl;
    } 

    return 0;
}
