#ifndef __LOAD_N_COMMAND__
#define __LOAD_N_COMMAND__

#include "icommand.h"
#include <iostream>

class LoadNCommand : public Icommand
{
public:
	LoadNCommand(int _n);
	virtual  ~LoadNCommand() {}
	
	virtual int executor(Istack& _istack);

private:
    int m_num;
};

LoadNCommand::LoadNCommand(int _n) 
    : m_num(_n)
{    
}

int LoadNCommand::executor(Istack& _istack)
{
	_istack.Push(m_num);
	std::cout << "Load " << m_num << std::endl; 
	// move itr by one
	return 1; 
}

#endif
