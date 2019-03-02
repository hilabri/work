#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "HashMap.h"
#include "list_itr.h"

#define FALSE 0
#define TRUE 1

#define MAGIC        0xDEADBEEF
#define DEAD_MAGIC   0XDEADDEAD
#define IS_VALID(H)      ((H) && (MAGIC == (H)->m_magic))  

#define INDEX(MAP,KEY)       ((MAP)->m_hashFunction(KEY)) % (MAP)->m_size
#define BUCKET(MAP,I)    (MAP)->m_buckets[(I)]
#define BUCKET_BY_KEY(MAP, KEY) (BUCKET(MAP, INDEX(MAP,KEY)))

#define KEY(PAIR)        (PAIR)->m_key
#define VALUE(PAIR)       (PAIR)->m_value

typedef struct Pair Pair;

struct Pair
{
    const void* m_key;
    const void* m_value;
};


struct HashMap 
{
    size_t m_magic; 
    List*  *m_buckets; /* array of pointers to lists/buckets */
    size_t m_size; /* size of Map */
    size_t m_numItems; /* num of items in HM */
    size_t m_numInserted; /* num if items inserted into HM */
    HashFunction m_hashFunction; /* keys mapped to values */
    EqualityFunction m_keysEqualFunc; /* find a generic ket */
    Map_Stats m_map_stats;
};

/***********************************************************************/
static int Is_Prime(size_t _num)
{
    int i;
    for(i=2; (i*i) <= _num; i++)
    {
        if(0 == _num % i)
        {
            return FALSE;
        }
    }
    return TRUE;
}
/***********************************************************************/
static size_t GetHashMapSize(size_t _capacity)
{
    int num = _capacity;
    while (!Is_Prime(num))
    {
        num++;
    }
    return num;
}

/***********************************************************************/
HashMap* HashMap_Create(size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc)
{
	HashMap* map;
	if (0 == _capacity)
    {
        return NULL;
    }
    map = malloc(sizeof(HashMap));
	if(!map)
	{	
		return NULL;
	}
	map->m_size = GetHashMapSize(_capacity);
	map->m_buckets = calloc(map->m_size, sizeof(List*));
    if(!map->m_buckets)
    {
        free(map);
        return NULL;
    }
	map->m_hashFunction = _hashFunc;
	map->m_keysEqualFunc = _keysEqualFunc;
	map->m_numItems = 0;	
	map->m_numInserted =0;
    map->m_magic = MAGIC;
	return map;
}

/***********************************************************************/
static void BucketDestroy(List *_bucket, void (*_keyDestroy)(void* _key), void (*_valDestroy)(void* _value))
{
    ListItr itr;
    Pair *pair;
    for (itr= ListItr_Begin(_bucket); !ListItr_Equals(itr,ListItr_End(_bucket)); itr = ListItr_Next(itr))
    {
        pair = (Pair*)ListItr_Get(itr);
        if (_keyDestroy)
        {
            _keyDestroy((void *)KEY(pair));
        }
        if (_valDestroy)
        {
            _valDestroy((void *)VALUE(pair));
        }
        free(pair); 
    }
    List_Destroy(&_bucket, NULL);
}

/***********************************************************************/
void HashMap_Destroy(HashMap** _map, void (*_keyDestroy)(void* _key), void (*_valDestroy)(void* _value))
{	
    int i;
    if (!_map || !IS_VALID(*_map))
    {
        return;
    }
	(*_map)->m_magic = DEAD_MAGIC;
    for(i = 0; i < (*_map)->m_size; i++)
    {
        BucketDestroy(BUCKET((*_map),i), _keyDestroy, _valDestroy);
    }
    free((*_map)->m_buckets);
	free(*_map);
    *_map = NULL;
}

/***********************************************************************/
static Map_Result Bucket_InsertPair(List **_bucket, const void* _key, const void* _value)
{
    Pair *pair;
    if(!(*_bucket))
    {
        (*_bucket) = List_Create();
    	if(!(*_bucket))
    	{
     		return MAP_ALLOCATION_ERROR;
     	}
    }
    pair = malloc(sizeof(Pair));
    if (!pair)
    {
        return MAP_ALLOCATION_ERROR;
    }
    pair->m_key = _key;
    pair->m_value = _value;
    List_PushHead((*_bucket), pair);
    return MAP_SUCCESS;
}

/***********************************************************************/
Map_Result HashMap_Insert(HashMap* _map, const void* _key, const void* _value)
{
    Map_Result res;
    void *findValue;
    List *bucket;
    if (!IS_VALID(_map))
    {
        return MAP_UNINITIALIZED_ERROR;
    }
    if (MAP_SUCCESS == HashMap_Find(_map, _key, &findValue))
    {
        return MAP_KEY_DUPLICATE_ERROR;
    }
    bucket = BUCKET_BY_KEY(_map, _key);
    res = Bucket_InsertPair(&BUCKET_BY_KEY(_map, _key), _key, _value);
    if(MAP_SUCCESS == res)
    {
	    _map->m_numItems++;	
	    _map->m_numInserted++;
    }
    return res;
}

/***********************************************************************/
static void Bucket_RemovePair(List *_bucket, const void* _key, EqualityFunction _keysEqualFunc, Pair **_pair)
{
    ListItr itr;
    for (itr= ListItr_Begin(_bucket); !ListItr_Equals(itr,ListItr_End(_bucket)); itr = ListItr_Next(itr))
    {
        *_pair = (Pair *)ListItr_Get(itr);
        if (_keysEqualFunc(_key, KEY(*_pair)))
        {
            ListItr_Remove(itr);
            if (List_IsEmpty(_bucket))
            {
                List_Destroy(&_bucket, NULL);
            }
            return;
        }
    }
    *_pair = NULL;
}

/***********************************************************************/
Map_Result HashMap_Remove(HashMap* _map, const void* _searchKey, void** _pKey, void** _pValue)
{
    Pair* pair;
    if (!IS_VALID(_map) || !_pKey || !_pValue)
    {
        return MAP_UNINITIALIZED_ERROR;
    }
    Bucket_RemovePair(BUCKET_BY_KEY(_map, _searchKey), _searchKey, _map->m_keysEqualFunc, &pair);
    if (!pair)
    {
        return MAP_KEY_NOT_FOUND_ERROR;
    }
    *_pKey = (void *)KEY(pair);
    *_pValue = (void *)VALUE(pair);
    _map->m_numItems--;
    free(pair);
    return MAP_SUCCESS;
}

/***********************************************************************/
static void* Bucket_FindValue(List *_bucket, const void* _searchKey, EqualityFunction _keysEqualFunc)
{
    ListItr itr;
    Pair* pair;
    void *searchKeyValue = NULL;
	itr = ListItr_Begin(_bucket);
    while (itr != ListItr_End(_bucket))
    {
        pair = (Pair*)ListItr_Get(itr);
    	if (_keysEqualFunc(KEY(pair),_searchKey))
        {
        	searchKeyValue = (void *)VALUE(pair);
            break;
        }   
        itr = ListItr_Next(itr);
    }
    return searchKeyValue;
}

/***********************************************************************/
Map_Result HashMap_Find(const HashMap* _map, const void* _searchKey, void** _pValue)
{
    if (!IS_VALID(_map) || !_pValue)
    {
        return MAP_UNINITIALIZED_ERROR;
    }
	*_pValue = Bucket_FindValue(BUCKET_BY_KEY(_map, _searchKey), _searchKey, _map->m_keysEqualFunc);
	if (!(*_pValue))
	{
		return MAP_KEY_NOT_FOUND_ERROR;
	}
	return MAP_SUCCESS;  
}    
 
/***********************************************************************/
static void Migrate_Items(HashMap *_map, List **_newBuckets, size_t _newSize)
{
    size_t i;
    size_t newBucketIndex;
    List *bucket;
    ListItr itr;
    Pair *pair;
    for (i = 0; i < _map->m_size; i++)
    {
        bucket = BUCKET(_map, i);
        itr = ListItr_Begin(bucket);
        while (itr != ListItr_End(bucket))
        {
            pair = (Pair*)ListItr_Get(itr);
            itr = ListItr_Next(itr);
            Bucket_RemovePair(bucket, KEY(pair), _map->m_keysEqualFunc, &pair);
            newBucketIndex = _map->m_hashFunction(KEY(pair)) % _newSize;
            Bucket_InsertPair(&_newBuckets[newBucketIndex], KEY(pair), VALUE(pair));
        }
    }
}

/***********************************************************************/
Map_Result HashMap_Rehash(HashMap *_map, size_t _newCapacity)
{
	List **newBuckets;
    size_t newSize;
    if (0 == _newCapacity)
    {
        return MAP_INVALID_ARGUMENT;
    }
    newSize = GetHashMapSize(_newCapacity);
    newBuckets = calloc(newSize, sizeof(List*));
    if (!newBuckets)
    {
   		return MAP_ALLOCATION_ERROR;
   	}
    /* Migrate old buckets => new buckets */
    Migrate_Items(_map, newBuckets, newSize);

    free(_map->m_buckets);
    _map->m_buckets = newBuckets;    
    _map->m_size = newSize;
    return MAP_SUCCESS;
}

/***********************************************************************/
size_t HashMap_Size(const HashMap* _map)
{
    if (!IS_VALID(_map))
    {
        return 0;
    }
    return _map->m_numItems;
}

/***********************************************************************/
size_t HashMap_ForEach(const HashMap* _map, KeyValueActionFunction _action, void* _context)
{
    int i;
    size_t count = 0;
    ListItr itr;
    Pair *pair;
    if (!IS_VALID(_map))
    {
        return MAP_UNINITIALIZED_ERROR;;
    }
    if(_action)
    {
        for(i = 0; i < _map->m_size; i++)
        {
            for (itr = ListItr_Begin(BUCKET(_map, i)); itr != ListItr_End(BUCKET(_map, i)); itr = ListItr_Next(itr))
            {
                pair = ListItr_Get(itr);
                if(!_action((void *)KEY(pair),(void *)VALUE(pair),_context))
                {
                    break;
                }
                count++;
            }
        }
    }
    return count;
} 

