#ifndef __JNZ_COMMAND__
#define __JNZ_COMMAND__

#include "icommand.h"
#include <iostream>

class JnzCommand : public Icommand
{
public:
	JnzCommand(int _n);
	virtual  ~JnzCommand() {}
	
	virtual int executor(Istack& _istack);

private:
    int m_num;
};

JnzCommand::JnzCommand(int _n) 
    : m_num(_n)
{    
}

int JnzCommand::executor(Istack& _istack)
{
	if (0 != _istack.Top())
    {
	    std::cout << "JNZ " << m_num << std::endl; 
        // move itr by n
        return m_num;
    }
	// move itr by one
	return 1; 
}

#endif
