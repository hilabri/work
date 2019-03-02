#ifndef __EXECUTOR__
#define __EXECUTOR__

#include "instructionIterator.h"
#include <iostream> 

class Executor
{
public:
	Executor(Istack* _istack, const int _commandsCount);
	~Executor() {};
	
	template <class T> void Run(T& _itrBegin, T& _itrEnd);
	
private:
	//IO* m_io;
	Istack* m_istack; 
    const int m_size;
};




Executor::Executor(Istack* _istack, const int _commandsCount)
	: m_istack (_istack)
    , m_size (_commandsCount)
{
	
}


template <class T>
void Executor::Run(T& _itrBegin, T& _itrEnd)
{
	int count = 0;
    while(_itrBegin != _itrEnd)
	{
		int jump = (*_itrBegin)->executor(*m_istack);
        
        // check if out of bounds
        count += jump;
        const bool isOutOfBounds = (0 > count) || (m_size <= count);
        if (isOutOfBounds)
        {
            std::cout << "Exit => Out of bounds" << std::endl;
            return;
        }

        // move itr to next command    	
        _itrBegin+=jump;
	}
}


#endif
