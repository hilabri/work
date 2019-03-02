#ifndef __TIMER_H__
#define __TIMER_H__

#include <iostream>
#include "runable.h"
 
namespace advcpp{
namespace SH{

class Timer : public Runable
{
public:
  Timer(){}
  ~Timer(){}
  void Run();
};


void Timer::Run()
{
  // join()
  std::cout << "DO SOMTHING" << std::endl;
}

}}
#endif
