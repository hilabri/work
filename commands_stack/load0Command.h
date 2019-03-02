#ifndef __LOAD0COMMAND__
#define __LOAD0COMMAND__

#include "icommand.h"
#include <iostream>

class Load0Command : public Icommand
{
public:
	Load0Command() {};
	virtual  ~Load0Command() {}
	
	virtual int executor(Istack& _istack);

private:

};


int Load0Command::executor(Istack& _istack)
{
	_istack.Push(0);
	std::cout << "Load 0" << std::endl; 
	// move itr by one
	return 1; 
}

#endif
