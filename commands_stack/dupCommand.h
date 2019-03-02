#ifndef __DUPCOMMAND__
#define __DUPCOMMAND__

#include "icommand.h"
#include <iostream>

class DupCommand : public Icommand
{
public:
	DupCommand() {};
	virtual ~DupCommand() {};
	
	virtual int executor(Istack& _istack);

private:

};



int DupCommand::executor(Istack& _istack)
{
	_istack.Push(_istack.Top());
	std::cout << "Duplicate " << _istack.Top() << std::endl; 
	// move itr by one
	return 1; 
}

#endif
