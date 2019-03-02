#ifndef __RUNABLE_H__
#define __RUNABLE_H__

namespace advcpp{
namespace SH{

class Runable
{
public:
	Runable () {}
	virtual ~Runable(){}
	virtual void Run() = 0;
};

}}
#endif
