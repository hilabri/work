#ifndef __RUNNABLE_H__
#define __RUNNABLE_H__

namespace SH{

class Runnable
{
public:
	Runnable () {}
	virtual ~Runnable(){}
	virtual void Run() = 0;
};

}
#endif
