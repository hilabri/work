#include "pool.h"

unsigned int memPool_t::defaultSize = 6;

/* CTOR: */
memPool_t::memPool_t()
{
    memPage_t* page = new memPage_t(defaultSize);
    vec.insert(vec.end(), page);
}

/* DTOR: */
memPool_t::~memPool_t()
{
    for (memPage_t *page : vec)
    {
        delete page;
    }
}

unsigned int memPool_t::read(void* dest, unsigned int size) const
{
    return read(dest, size, getPosition());
}


unsigned int memPool_t::read(void* dest, unsigned int size, unsigned int pos) const
{
    if(!isValidPosition(pos)) return 0;
    unsigned int pagePos = pos;
    unsigned int leftToRead = size;
    unsigned int destOffset = 0;
    
    for (memPage_t *page : vec)   /* itr on vector */ 
    {
        if(pagePos < page->getCapacity())  /* are you on the relevant page? */
        {
            destOffset = size - leftToRead;
            leftToRead -= page->read((char *)dest + destOffset, leftToRead, pagePos);
            if(0 == leftToRead) return size;
            pagePos = 0;
        }
        else
        {
            pagePos -= page->getCapacity();  /* skip to next page pos */
        }
    }
    return size - leftToRead;  /* read size exceeded bytes written */         
}

unsigned int memPool_t::write(const void* src, unsigned int size)
{
    return write(src, size, getPosition());
}

unsigned int memPool_t::write(const void* src, unsigned int size, unsigned int pos)
{
    if(!isValidPosition(pos)) return 0;
    unsigned int pagePos = pos;
    unsigned int leftToWrite = size;
    unsigned int srcOffset = 0;
    const int sizeDiff = pos + size - getActualSize();
    if (0 < sizeDiff) AddToActualSize(sizeDiff); /* new bytes written */
    
    for (memPage_t *page : vec)   
    {
        if(pagePos < page->getCapacity())  
        {
            srcOffset = size - leftToWrite;
            leftToWrite -= page->write((const char *)src + srcOffset, leftToWrite, pagePos);
            if(0 == leftToWrite) return size;
            pagePos = 0;
        }
        else
        {
            pagePos -= page->getCapacity();  
        }
    }

    while(0 < leftToWrite)    /* if the mem pool ended, add more pages */
    {
        memPage_t* page = new memPage_t(defaultSize);
        vec.insert(vec.end(), page);
        srcOffset = size - leftToWrite;
        leftToWrite -= page->write((const char *)src + srcOffset, leftToWrite, pagePos);
    }
    return size;
}
