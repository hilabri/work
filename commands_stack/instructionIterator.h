#ifndef __INSTRACTION_ITERATOR__
#define __INSTRACTION_ITERATOR__

#include "icommand.h"

template <class T> // std::iterator
class InstructionIterator
{
public:
	InstructionIterator(T& _itr);
	~InstructionIterator() {};
	
	bool operator!=(InstructionIterator& _itr); 
	
	InstructionIterator& operator++(int);
    InstructionIterator& operator--(int);
    InstructionIterator& operator+=(int _n);  
	Icommand* operator*();
	
private:
	T& m_itr; 
};


template <class T> 
bool InstructionIterator<T>::operator!=(InstructionIterator& _itr)
{
	return ( m_itr != _itr.m_itr);
}

template <class T>
InstructionIterator<T>::InstructionIterator(T& _itr)
	: m_itr (_itr)
{}


template <class T>
InstructionIterator<T>& InstructionIterator<T>::operator++(int)
{
	m_itr++;
	return *this;
}


template <class T>
InstructionIterator<T>& InstructionIterator<T>::operator--(int)
{
	m_itr--;
	return *this;
}


template <class T>
InstructionIterator<T>& InstructionIterator<T>::operator+=(int  _n)
{
	m_itr += _n;
	return *this;
}


template <class T>
Icommand* InstructionIterator<T>::operator*()
{
	return *m_itr;
}

#endif
