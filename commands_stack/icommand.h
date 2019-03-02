#ifndef __ICOMMAND__
#define __ICOMMAND__

#include "istack.h"

class Icommand
{
public:
	Icommand() {};
	virtual ~Icommand() {};
	
	virtual int executor(Istack& _istack) =0;

private:
	
};

#endif
