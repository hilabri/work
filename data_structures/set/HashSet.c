#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ADTDefs.h"
#include "HashSet.h"
#define MAGIC 0XBEEFBEEF
#define IS_VALID(SET)  ((SET) && (MAGIC == (SET)->m_magic))
#define FALSE 0
#define TRUE 1
#define ERROR_EXISTS -1
#define ERROR_NO_SPACE -2
#define ERROR_NOT_EXIST -3
#define ERROR_UNKNOWN_FLAG -4
#define HASH_CODE(SET,DATA)  (SET->_hashFunction(DATA) % SET->m_hashSize)
#define RE_HASH(SET,HASH_CODE) ((HASH_CODE + 1) % SET->m_hashSize)
#define MAX_LOAD(SET) (SET->m_load * SET->m_hashSize)

typedef enum
{
	FREE_INDEX = 0,    
	USED_INDEX,    
    PRE_USED
} Flags;

struct HashSet
{
	ssize_t *m_items;
	ssize_t *m_flags;
	size_t m_hashSize;
	size_t m_numItems;
	size_t m_load;
    size_t m_magic;

	size_t	m_numInserts;
	size_t m_numRehashOps;
	size_t m_maxRehash;
	
	size_t (*_hashFunction)(size_t);
};

HashSet* HashSetCreate(size_t _capacity, float _loadFactor, size_t (*_hashFunction)(size_t))
{
	HashSet* set = malloc(sizeof(HashSet));
	if(!set)
	{	
		return NULL;
	}
	set->m_items = malloc(_capacity*sizeof(ssize_t));
    if(!set->m_items)
    {
        free(set);
        return NULL;
    }
    set->m_flags = calloc(_capacity, sizeof(ssize_t));
	if(!set->m_flags)
	{
        free(set->m_items);
        free(set);		
		return NULL;
	}

	set->m_hashSize =  _capacity;
	set->m_load = _loadFactor;
	set->_hashFunction = _hashFunction;
	set->m_numItems = 0;	
	set->m_numInserts =0;
	set->m_numRehashOps = 0;
	set->m_maxRehash = 0;
    set->m_magic = MAGIC;
	return set;
}

static int FindInsertLocation(HashSet *_set, int _data)
{
	int rehashCount = 0;
    int index = HASH_CODE(_set, _data);
	int isPreUsed = FALSE;
    int preUsedIndex;

    while(rehashCount < _set->m_hashSize)
	{
		switch(_set->m_flags[index])
        {
            case FREE_INDEX:
                            return isPreUsed ? preUsedIndex : index;
			case USED_INDEX:
                            if (_data == _set->m_items[index]) 
                            {
                                return ERROR_EXISTS;
                            }
                            index = RE_HASH(_set, index);
                            break;
            case PRE_USED:
                            isPreUsed = TRUE;
                            preUsedIndex = index;
                            index = RE_HASH(_set, index);
                            break;
            default:
                            return ERROR_UNKNOWN_FLAG;
        }
        rehashCount++;
		_set->m_numRehashOps++;
        if (rehashCount > _set->m_maxRehash)
		{
			_set->m_maxRehash = rehashCount;
		}
    }
    return ERROR_NO_SPACE;
}


ADTErr HashSetInsert(HashSet *_set,  size_t _data)
{
    int index;
    if (!IS_VALID(_set))
    {
        return ERR_NOT_INITIALIZED;
    }
    index = FindInsertLocation(_set, _data);
    if (ERROR_EXISTS == index)
    {
        return ERR_ALREADY_EXIST;
    }
    if (ERROR_NO_SPACE == index)
    {
        return ERR_OVERFLOW;
    }
    if (ERROR_UNKNOWN_FLAG == index)
    {
        return ERR_GENERAL;
    }

	_set->m_items[index] = _data;
	_set->m_flags[index] = USED_INDEX;
	_set->m_numItems++;
	_set->m_numInserts++;
	
    return ERR_OK;
}

int FindDataIndex(const HashSet* _set, size_t _data)
{
	int rehashCount = 0;
    int index = HASH_CODE(_set, _data);
    
    while(rehashCount < _set->m_hashSize)
	{
		switch(_set->m_flags[index])
        {
            case FREE_INDEX:
                            return ERROR_NOT_EXIST;
			case USED_INDEX:
                            if (_data == _set->m_items[index]) 
                            {
                                return index;
                            }
                            index = RE_HASH(_set, index);
                            break;
            case PRE_USED:
                            index = RE_HASH(_set, index);
                            break;
            default:
                            return ERROR_UNKNOWN_FLAG;
        }
        rehashCount++;
    }
    return ERROR_NOT_EXIST;
}

ADTErr HashSetRemove(HashSet* _set, size_t _data)
{
    int index;
    if (!IS_VALID(_set))
    {
        return ERR_NOT_INITIALIZED;
    }
    index = FindDataIndex(_set,_data);
    if (index == ERROR_NOT_EXIST)
    {
        return ERR_NOT_EXIST;
    }
    if (ERROR_UNKNOWN_FLAG == index)
    {
        return ERR_GENERAL;
    }
    _set->m_flags[index] = PRE_USED;
    _set->m_numItems--;
    return ERR_OK;
}

int HashSetContains(const HashSet* _set, size_t _data)
{
    if (!IS_VALID(_set))
    {
        return FALSE;
    }
    return (0 < FindDataIndex(_set,_data));
}

size_t HashSetSize(const HashSet* _set)
{
    if (!IS_VALID(_set))
    {
        return ERR_NOT_INITIALIZED;
    }
    return _set->m_numItems;
}

void HashSetDestroy(HashSet* _set)
{	
    if (!IS_VALID(_set))
    {
        return;
    }
    free(_set->m_flags);
	_set->m_magic = 0xDEADBEEF;
    free(_set->m_items);
	free(_set);
}

void HashSetForEach(const HashSet* _hash, void (*_userFunction)(size_t _data))
{
	int i;	
    if (!IS_VALID(_hash) || !_userFunction)
    {
        return;
    } 
	for(i =0; i < _hash->m_hashSize; i++)
	{
		if(_hash->m_flags[i] ==  USED_INDEX)
		{		
			_userFunction(_hash->m_items[i]);
		}  	
	}
}

ADTErr HashSetStatistics(const HashSet* _set, size_t *_maxCollisionsEver, float *_averageCollisions)
{
    if (!IS_VALID(_set))
    {
        return ERR_NOT_INITIALIZED;
    }
	*_maxCollisionsEver = _set->m_maxRehash;
    *_averageCollisions = _set->m_numRehashOps / _set->m_numInserts; /* Search and remove operations missing? */
    return ERR_OK;
}

