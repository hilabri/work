#ifndef __PAGE_H__
#define __PAGE_H__

#include "manager.h"

using namespace std;

class memPage_t: public memManager_t
{
    public:
        memPage_t(unsigned int cap);
        virtual~memPage_t();
        inline bool isFull()const; 
        inline unsigned int getCapacity() const;
        virtual unsigned int read(void* dest, unsigned int size) const;
        virtual unsigned int read(void* dest, unsigned int size, unsigned int pos) const;
        virtual unsigned int write(const void* src, unsigned int size);
        virtual unsigned int write(const void* src, unsigned int size, unsigned int pos);
    private:
        char* page;
        unsigned int capacity;
        memPage_t(memPage_t& pg);
};

/* Inline functions: */
inline bool memPage_t::isFull() const { return (capacity == getActualSize()); }
inline unsigned int memPage_t::getCapacity() const { return capacity; }

#endif
