#ifndef __POOL_H__
#define __POOL_H__

#include "page.h"
#include <iostream>
#include <vector>

using namespace std;

class memPool_t: public memManager_t
{
    public:
        memPool_t(); 
        virtual ~memPool_t();
        static inline unsigned int getDefaultSize();
        static inline void setDefaultSize(unsigned int size);
        virtual unsigned int write(const void* src, unsigned int size);
        virtual unsigned int write(const void* src, unsigned int size, unsigned int pos);
        virtual unsigned int read(void* dest, unsigned int size) const;
        virtual unsigned int read(void* dest, unsigned int size, unsigned int pos) const;
    private:
        vector<memPage_t*> vec; 
        static unsigned int defaultSize;
        memPool_t(memPool_t& pool);
};

/* Inline functions: */
inline unsigned int memPool_t::getDefaultSize(){ return defaultSize; }
inline void memPool_t::setDefaultSize(unsigned int size){ defaultSize = size; }

#endif
