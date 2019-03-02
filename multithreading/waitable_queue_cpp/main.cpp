#include <iostream>
#include "thread.h"
#include "waitable_queue.h"

using namespace advcpp;
using namespace SH;

//producer 
class Producer : public Runable
{
public:
    Producer(WaitableQueue<int>& _wq);
    void Run();
private:
    WaitableQueue<int>& m_wq;
};


Producer::Producer(WaitableQueue<int>& _wq)
    : m_wq (_wq)
{}

void  Producer::Run()
{
    int n = 101;
    while (--n)
    {
	try
        {
            m_wq.Push(n);
	}
	catch(...)
        {
            std::cout << "Push failed" << std::endl; 
        }
    }
}

//consumer
class Consumer : public Runable
{
public:
    Consumer(WaitableQueue<int>& _wq);
    void Run();

    static void AddToSum(int _value);
    static Mutex sumMutex;
    static int sum;
private:
     WaitableQueue<int>& m_wq;
};

int Consumer::sum = 0;
Mutex Consumer::sumMutex;

void Consumer::AddToSum(int _value)
{
    Consumer::sumMutex.Lock();
    Consumer::sum += _value;
    Consumer::sumMutex.UnLock();
}


Consumer::Consumer(WaitableQueue<int>& _wq)
    : m_wq(_wq)
{}


void  Consumer::Run()
{
    int res;
    int n=101;
    while (--n)
    {
        try
        {
            res = m_wq.Pop();
	}
	catch(...)
        {
            std::cout << "Pop failed" << std::endl; 
        }
	Consumer::AddToSum(res);
    }
}


// test main
int main()
{

    WaitableQueue<int> wq;
    Producer prd(wq);
    Consumer cns(wq);

    try
    {
        Thread t1(prd),t2(prd),t3(prd);
        Thread s1(cns),s2(cns),s3(cns);

        t1.Join();
        t2.Join();
        t3.Join();
        s1.Join();
        s2.Join();
        s3.Join();
    }
    catch (...)
    {
        std::cout << "Thread exception raised" << std::endl;
    }	
    
    std::cout << "3 * sum of 1..100 should be 15150" << std::endl;
    std::cout << "result is:" << Consumer::sum << std::endl;

    return 0;
}


