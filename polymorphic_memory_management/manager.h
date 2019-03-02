#ifndef __MANAGER_H__
#define __MANAGER_H__

#include <cstring>

class memManager_t 
{
    public:
        memManager_t();
        virtual ~memManager_t() =0; /* pure virtual */
        inline unsigned int getPosition() const;
        inline bool setPosition(unsigned int pos);
        inline bool isEmpty() const;
        inline unsigned int getActualSize() const;
        virtual unsigned int write(const void* src, unsigned int size) =0;
        virtual unsigned int write(const void* src, unsigned int size, unsigned int pos) =0;
        virtual unsigned int read(void* dest, unsigned int size) const =0;
        virtual unsigned int read(void* dest, unsigned int size, unsigned int pos) const =0;
    protected:
        inline void AddToActualSize(unsigned int size);
        inline bool isValidPosition(unsigned int pos) const;
    private:
        memManager_t(memManager_t& mng);
        unsigned int position;    
        unsigned int actualSize; /* bytes written */
};
        
/* CTOR: */
inline memManager_t::memManager_t() { actualSize = 0; position = 0; }

inline bool memManager_t::isEmpty() const { return (0 == actualSize); }
inline unsigned int memManager_t::getActualSize() const { return actualSize; }
inline void memManager_t::AddToActualSize(unsigned int size) { actualSize += size; }

inline unsigned int memManager_t::getPosition() const { return position; }
inline bool memManager_t::isValidPosition(unsigned int pos) const { return pos <= actualSize;}
inline bool memManager_t::setPosition(unsigned int pos) 
{ 
    if(!isValidPosition(pos)) return false; 
    position = pos; 
    return true;
}


#endif
