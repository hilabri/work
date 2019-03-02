#ifndef __LOADCOMMAND__
#define __LOADCOMMAND__

#include "icommand.h"
#include <iostream>

class Load1Command : public Icommand
{
public:
	Load1Command() {};
	virtual  ~Load1Command() {}
	
	virtual int executor(Istack& _istack);

private:

};


int Load1Command::executor(Istack& _istack)
{
	_istack.Push(1);
	std::cout << "Load 1" << std::endl; 
	// move itr by one
	return 1; 
}

#endif
