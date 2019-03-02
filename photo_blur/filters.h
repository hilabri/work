#ifndef __FILTERS_H__
#define __FILTERS_H__

#include "filter.h"
template<class W>
class Filters_t 
{
	public:
		static Filter_t<W> MakeMean(int _size);
		//gaus func
};

template<class W>
Filter_t<W> Filters_t<W>::MakeMean(int _size)
{
	Filter_t<W> filter(_size);
	const W weight = 1.0/( _size * _size);
    filter.SetAllWeights(weight);
	return filter;
}

#endif
