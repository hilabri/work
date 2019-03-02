#ifndef __OUTCOMMAND__
#define __OUTCOMMAND__

#include "icommand.h"
#include <iostream>

class OutCommand : public Icommand
{
public:
	OutCommand() {};
	virtual  ~OutCommand() {}
	
	virtual int executor(Istack& _istack);

private:

};


int OutCommand::executor(Istack& _istack)
{
	std::cout << "Out: " << _istack.Top() << std::endl; 
	_istack.Pop();
	// move itr by one
	return 1; 
}

#endif
