#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include <semaphore.h>
#include <stdexcept>

namespace advcpp{
namespace SH{


class Semaphore
{

public:
	Semaphore (int _size = 1024);
	~Semaphore ();
	void Wait();
	void Post();

private:
	sem_t m_sem;
};



Semaphore::Semaphore(int _size)
{
    if(sem_init(&m_sem, 0, _size) == -1)
    {
        throw std::runtime_error("initialize semaphore failed");
    }
}



Semaphore::~Semaphore()
{
    assert(sem_destroy(&m_sem) != -1);
}



void Semaphore::Wait()
{
  if(sem_wait(&m_sem) == -1)
  {
    throw std::runtime_error("semaphore wait empty failed");
  }
}



void Semaphore::Post()
{
  if(sem_post(&m_sem) == -1)
  {
    throw std::runtime_error("semaphore post empty");
  }
}




}}
#endif
