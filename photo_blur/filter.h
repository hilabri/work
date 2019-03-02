#ifndef __FILTER_H__
#define __FILTER_H__

#include "matrix.h"

template<class W>
class Filter_t 
{
	public:
        Filter_t(int _size);
		//~Filter_t();
		
		int GetSize() const; 
		
		void SetAllWeights(W _value);		
		void SetWeight(W _value ,int _rows, int _columns);
		
        W GetWeight(int _rows, int _columns) const;
		
	private:
		Matrix_t<W> m_weightsMatrix;
		int m_size;
};


/* ---------------------------------------------------*/
template<class W>
Filter_t<W>::Filter_t(int _size): m_weightsMatrix(_size,_size), m_size(_size)
{ }

template<class W>
int Filter_t<W>::GetSize() const
{
	return m_size;
}

template<class W>
void Filter_t<W>::SetWeight(W _value ,int _rows, int _columns)
{
	m_weightsMatrix.SetValue(_value, _rows,_columns);
}

template<class W>
W Filter_t<W>::GetWeight(int _rows, int _columns) const
{
	return m_weightsMatrix.GetValue(_rows,_columns);
}

template<class W>
void Filter_t<W>::SetAllWeights(W _value)
{
	return m_weightsMatrix.SetAllValues(_value);
}

#endif
