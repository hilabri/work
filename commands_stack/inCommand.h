#ifndef __INCOMMAND__
#define __INCOMMAND__

#include "icommand.h"
#include <iostream>

class InCommand : public Icommand
{
public:
	InCommand() {};
	virtual  ~InCommand() {}
	
	virtual int executor(Istack& _istack);

private:

};


int InCommand::executor(Istack& _istack)
{
	std::cout << "Enter Number: " << std::endl; 
    int num;
	std::cin >> num; 
	_istack.Push(num);
	std::cout << "In: " << num << std::endl; 
	// move itr by one
	return 1; 
}

#endif
