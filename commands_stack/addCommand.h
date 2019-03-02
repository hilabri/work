#ifndef __ADDCOMMAND__
#define __ADDCOMMAND__

#include "icommand.h"
#include <iostream>

class AddCommand : public Icommand
{
public:
	AddCommand() {};
	virtual ~AddCommand() {}

	virtual int executor(Istack& _istack);

private:

};


int AddCommand::executor(Istack& _istack)
{
	const int valueA = _istack.Top();
	_istack.Pop();
	const int valueB = _istack.Top();
	_istack.Pop();
	_istack.Push(valueA + valueB);
	std::cout << "Add: " << valueA << "+" << valueB << std::endl;
	// move itr by one
	return 1; 
}

#endif
