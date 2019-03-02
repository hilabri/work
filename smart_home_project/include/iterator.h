#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#include <set>

template <class V>
class SetItr
{
public:
    SetItr();
    typedef std::set<V> Set;
    SetItr(const Set& _set);
    //~SetItr();
    //SetItr(const SetItr& _itr);
    //SetItr& operator=(const SetItr& _itr);

    SetItr<V>& Begin();
    SetItr<V> End();
    
    //pre-increment ++a
    SetItr<V>& operator++();

    bool operator!=(const SetItr& _itr) const;
    V operator*();

private:
    typename std::set<V>::iterator m_current;
    typename std::set<V>::iterator m_begin;
    typename std::set<V>::iterator m_end;
    bool m_setExist;
};



template <class V>
SetItr<V>::SetItr()
    : m_setExist(false)
{
}


template <class V>
SetItr<V>::SetItr(const std::set<V>& _set)
    : m_current(_set.begin())
    , m_begin(_set.begin())
    , m_end(_set.end())
    , m_setExist(true)
{}


template <class V>
SetItr<V>& SetItr<V>::Begin()
{
    if(m_setExist)
    {
        m_current = m_begin;
    }
    return *this;
}

template <class V>
SetItr<V> SetItr<V>::End()
{
    if(m_setExist)
    {
        SetItr newItr(*this);
        newItr.m_current = m_end;
        return newItr;
    }
    return *this;
}

template <class V>
SetItr<V>& SetItr<V>::operator++()
{
    if(m_setExist)
    {
        ++m_current;
    }
    return *this;
}


template <class V>
bool SetItr<V>::operator!=(const SetItr& _it) const
{
    if (!m_setExist || !_it.m_setExist)
    {
        return (m_setExist != _it.m_setExist);
    }

    return _it.m_current != m_current;
}


template <class V>
V SetItr<V>::operator*()
{
    //TODO: handle !m_setExist - exception
    return *m_current;
}

#endif
