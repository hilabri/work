#ifndef __MULCOMMAND__
#define __MULCOMMAND__

#include "icommand.h"
#include <iostream>

class MulCommand : public Icommand
{
public:
	MulCommand() {};
	virtual ~MulCommand() {}

	virtual int executor(Istack& _istack);

private:

};


int MulCommand::executor(Istack& _istack)
{
	const int valueA = _istack.Top();
	_istack.Pop();
	const int valueB = _istack.Top();
	_istack.Pop();
	_istack.Push(valueA * valueB);
	std::cout << "Multi: " << valueA << "*" << valueB << std::endl;
	// move itr by one
	return 1; 
}

#endif
