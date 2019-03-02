#include "page.h"

#define MIN(A,B) (A < B) ? A : B

/* CTOR: */
memPage_t::memPage_t(unsigned int cap)
{
    page = new char[cap];
    capacity = cap;
}

/* DTOR: */
memPage_t::~memPage_t()
{
    delete[] page;
}


unsigned int memPage_t::read(void* dest, unsigned int size) const
{
    const unsigned int pos = getPosition();
    return read(dest, size, pos);
}

unsigned int memPage_t::read(void* dest, unsigned int size, unsigned int pos) const
{
    if (!isValidPosition(pos)) return 0;
    const unsigned int remainingSize = getActualSize() - pos;
    const unsigned int bytesToRead = MIN(size, remainingSize);
    memcpy(dest, page + pos, bytesToRead); /* does nothing for size==0 */
    return bytesToRead;
}

unsigned int memPage_t::write(const void* src, unsigned int size)
{
    const unsigned int pos = getPosition();
    return write(src, size, pos);
}

unsigned int memPage_t::write(const void* src, unsigned int size, unsigned int pos)
{
    if (!isValidPosition(pos)) return 0;
    const unsigned int remainingSize = capacity - pos;
    const unsigned int bytesToWrite = MIN(size, remainingSize);
    const int sizeDiff = pos + bytesToWrite - getActualSize();
    if (0 < sizeDiff) AddToActualSize(sizeDiff); /* new bytes written */
    
    memcpy(page + pos, src, bytesToWrite); /* does nothing for size==0 */ 
    return bytesToWrite;
}

