#ifndef __STACK__
#define __STACK__

#include "istack.h"
#include <stack>

class Stack: public Istack 
{
public:
	Stack() {};
	virtual ~Stack() {}
	
	virtual void Push( int _num);
	virtual int Pop();
	virtual int Top() const;
    virtual bool Empty() const;
private:
	std::stack<int> m_stack;
};


void Stack::Push( int _num)
{
	m_stack.push(_num);
}


int Stack::Pop()
{
	int value = m_stack.top();
	m_stack.pop();
	return value;
}

int Stack::Top() const
{
	return m_stack.top();
}

bool Stack::Empty() const
{
    return m_stack.empty();
}

#endif
