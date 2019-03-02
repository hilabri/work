#ifndef __SUBCOMMAND__
#define __SUBCOMMAND__

#include "icommand.h"
#include <iostream>

class SubCommand : public Icommand
{
public:
	SubCommand() {};
	virtual ~SubCommand() {}

	virtual int executor(Istack& _istack);

private:

};


int SubCommand::executor(Istack& _istack)
{
	const int valueA = _istack.Top();
	_istack.Pop();
	const int valueB = _istack.Top();
	_istack.Pop();
	_istack.Push(valueA - valueB);
	std::cout << "Sub: " << valueA << "-" << valueB << std::endl;
	// move itr by one
	return 1; 
}

#endif
