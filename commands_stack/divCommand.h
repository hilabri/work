#ifndef __DIVCOMMAND__
#define __DIVCOMMAND__

#include "icommand.h"
#include <iostream>

class DivCommand : public Icommand
{
public:
	DivCommand() {};
	virtual ~DivCommand() {}

	virtual int executor(Istack& _istack);

private:

};


int DivCommand::executor(Istack& _istack)
{
	const int valueA = _istack.Top();
	_istack.Pop();
	const int valueB = _istack.Top();
	_istack.Pop();
	_istack.Push(valueA / valueB);
	std::cout << "Div: " << valueA << "/" << valueB << std::endl;
	// move itr by one
	return 1; 
}

#endif
