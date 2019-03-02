#ifndef __HLT_COMMAND__
#define __HLT_COMMAND__

#include "icommand.h"
#include <iostream>

class HltCommand : public Icommand
{
public:
	HltCommand(int _n) {};
	virtual  ~HltCommand() {}
	
	virtual int executor(Istack& _istack);

private:
    int m_num;
};

HltCommand::HltCommand(int _n) 
    : m_num(_n)
{    
}

int HltCommand::executor(Istack& _istack)
{
    while (!_istack.Empty())
    {
        _istack.Pop();
    }
	std::cout << "Halting" << std::endl; 
	// move itr by one
	return 1; 
}

#endif
