#ifndef __REGISTRATION_H__
#define __REGISTRATION_H__

#include <map>
#include <set>
#include "iterator.h"

namespace SH{

template <class K, class V>
class Registration
{
public:

    // TODO: add critical section for retrieve, insert, remove
	
    Registration() {};
	//~Registration();
    
    typedef std::set<V> Set;
    Set Retrieve(const K& _key) const;
	
    void Insert(K& _key, V _value);
    void Remove(K& _key, V _value);


private:
    std::map<K, Set> m_subscribers;
};



template <class K, class V>    
std::set<V> Registration<K,V>::Retrieve(const K& _key) const 
{
    typename std::map<K,std::set<V> >::const_iterator mapItr;
    mapItr = m_subscribers.find(_key);
    if(mapItr == m_subscribers.end())
    {
        std::set<V> emptySet;
        return emptySet;
    }
    return mapItr->second;
}

template <class K, class V>
void Registration<K,V>::Insert(K& _key, V _value)
{
    (m_subscribers[_key]).insert(_value);
}

template <class K, class V>
void Registration<K,V>::Remove(K& _key, V _value)
{
     (m_subscribers[_key]).erase(_value);
}


}
#endif
