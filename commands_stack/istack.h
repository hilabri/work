#ifndef __ISTACK__
#define __ISTACK__


class Istack
{
public:
	Istack() {};
	virtual ~Istack(){};
	
	virtual void Push( int _num) =0;
	virtual int Pop() =0;
	virtual int Top() const =0;
    virtual bool Empty() const =0;
	
private:

};

#endif
