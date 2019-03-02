#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include <semaphore.h>
#include <stdexcept>
#include <assert.h>

namespace SH{


class Semaphore
{

public:
	Semaphore (const char* _name, unsigned int _size = 1024);
	~Semaphore ();
	void Wait();
	void Post();

private:
    const char* m_name;
	sem_t *m_sem;
};


}
#endif
