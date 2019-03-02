#include "waitable_queue/semaphore.h"

using namespace SH;

Semaphore::Semaphore(const char* _name, unsigned int _size)
    : m_name(_name)
{
    m_sem = sem_open(_name, O_CREAT, 0644, _size);
    if (m_sem == SEM_FAILED)
    {
        throw std::runtime_error("initialize semaphore failed");
    }
}


Semaphore::~Semaphore()
{
    assert(0 == sem_close(m_sem));
    assert(0 == sem_unlink(m_name));
}


void Semaphore::Wait()
{
  if(sem_wait(m_sem) == -1)
  {
    throw std::runtime_error("semaphore wait empty failed");
  }
}



void Semaphore::Post()
{
  if(sem_post(m_sem) == -1)
  {
    throw std::runtime_error("semaphore post empty");
  }
}


