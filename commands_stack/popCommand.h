#ifndef __POPCOMMAND__
#define __POPCOMMAND__

#include "icommand.h"
#include <iostream>

class PopCommand : public Icommand
{
public:
	PopCommand() {};
	virtual  ~PopCommand() {}
	
	virtual int executor(Istack& _istack);

private:

};


int PopCommand::executor(Istack& _istack)
{
	std::cout << "Pop: " << _istack.Top() << std::endl; 
	_istack.Pop();
	// move itr by one
	return 1; 
}

#endif
