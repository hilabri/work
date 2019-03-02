#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "protected_hash.h"
#include "list_itr.h"
#include <pthread.h>
#include <unistd.h>

#define FALSE 0
#define TRUE 1

#define MAGIC        0xDEADBEEF
#define DEAD_MAGIC   0XDEADDEAD
#define IS_VALID(H)     ((H) && (MAGIC == (H)->m_magic))

#define INDEX(MAP,KEY)                      ((MAP)->m_hashFunction(KEY)) % (MAP)->m_size
#define BUCKET(MAP,I)                       (MAP)->m_buckets[(I)]
#define BUCKET_BY_KEY(MAP, KEY)             (BUCKET(MAP, INDEX(MAP,KEY)))

#define KEY(PAIR)       (PAIR)->m_key
#define VALUE(PAIR)     (PAIR)->m_value

/* get lock per bucket bucket  */
#define LOCK_INDEX(NUM_LOCKS, BUCKET_INDEX)     (BUCKET_INDEX) % (NUM_LOCKS)
#define LOCK(MAP, I)                            &((MAP)->m_mutexs[(I)])

typedef enum Map_Result {
    MAP_SUCCESS = 0,
    MAP_UNINITIALIZED_ERROR,        /**< Uninitialized map error      */
    MAP_KEY_NULL_ERROR,             /**< Key was null                 */
    MAP_KEY_DUPLICATE_ERROR,        /**< Duplicate key error          */
    MAP_KEY_NOT_FOUND_ERROR,        /**< Key not found                */
    MAP_ALLOCATION_ERROR,           /**< Allocation error             */
    MAP_INVALID_ARGUMENT            /**< Function argument is invalid */
} Map_Result;


typedef struct Pair Pair;

struct Pair
{
    const void* m_key;
    const void* m_value;
};

struct Protected_Hash
{
    pthread_mutex_t *m_mutexs;                /* array of locks for bucket groups */
    size_t m_numOfLocks;
    List*  *m_buckets;                        /* array of pointers to lists/buckets */
    size_t m_size;                            /* size of Map */
    size_t m_numItems;                        /* num of items in HM */
    size_t m_numInserted;                     /* num if items inserted into HM */
    HashFunction m_hashFunction;              /* keys mapped to values */
    KeyEqualityFunction m_keysEqualFunc;      /* find a generic key */
    size_t m_magic;
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


static size_t GetHashMapSize(size_t _capacity)
{
    int num = _capacity;
    while (!Is_Prime(num))
    {
        num++;
    }
    return num;
}

/*-------------------------------------------------------------------*/
Protected_Hash* ProtectedHash_Create(size_t _capacity, size_t _numOfLocks, HashFunction _hashFunc, KeyEqualityFunction _keysEqualFunc)
{
    int i;
    Protected_Hash* hash;
	if (0 == _capacity)
    {
        return NULL;
    }
    hash = malloc(sizeof(Protected_Hash));
	if(!hash)
	{
		return NULL;
	}
	hash->m_size = GetHashMapSize(_capacity);
	hash->m_buckets = calloc(hash->m_size, sizeof(List*));
    if(!hash->m_buckets)
    {
        free(hash);
        return NULL;
    }

    /* add an arry of init mutexs */
    hash->m_mutexs = calloc(_numOfLocks, sizeof(pthread_mutex_t));
    if(!hash->m_mutexs)
    {
        free(hash->m_buckets);
        free(hash);
        return NULL;
    }
    for(i = 0; i < _numOfLocks; i++)
    {
        pthread_mutex_init(&hash->m_mutexs[i], NULL);
    }
    hash->m_numOfLocks = _numOfLocks;

    hash->m_hashFunction = _hashFunc;
	hash->m_keysEqualFunc = _keysEqualFunc;
	hash->m_numItems = 0;
	hash->m_numInserted =0;
    hash->m_magic = MAGIC;
	return hash;
}

/***********************************************************************/
static void BucketDestroy(List *_bucket, KeyDestroyFunction _keyDestroy, ValDestroyFunction _valDestroy)
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

/*-----------------------------------------------------------------*/
void ProtectedHash_Destroy(Protected_Hash* _protectHM, KeyDestroyFunction _keyDestroy, ValDestroyFunction _valDestroy)
{
    int i;
    if (!IS_VALID(_protectHM))
    {
        return;
    }
	_protectHM->m_magic = DEAD_MAGIC;
    for(i = 0; i < _protectHM->m_size; i++)
    {
        BucketDestroy(BUCKET(_protectHM,i), _keyDestroy, _valDestroy);
    }

    /* destroy all mutexs */
    for(i = 0; i < _protectHM->m_numOfLocks; i++)
    {
        pthread_mutex_destroy(&_protectHM->m_mutexs[i]);
    }
    free(_protectHM->m_mutexs);

    free(_protectHM->m_buckets);
	free(_protectHM);
}

/***********************************************************************/
static void* Bucket_FindValue(List *_bucket, const void* _searchKey, KeyEqualityFunction _keysEqualFunc)
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
/*-----------------------------------------------------------------*/

static Map_Result HashMap_Find(Protected_Hash* _map, const void* _searchKey, void** _pValue)
{
    if (!IS_VALID(_map))
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

/*-----------------------------------------------------------------*/

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

/*-----------------------------------------------------------------*/

static Map_Result HashMap_Insert(Protected_Hash* _map, const void* _key, const void* _value)
{
    Map_Result res;
    void *findValue;
    if (!IS_VALID(_map))
    {
        return MAP_UNINITIALIZED_ERROR;
    }
    if (MAP_SUCCESS == HashMap_Find(_map, _key, &findValue))
    {
        return MAP_KEY_DUPLICATE_ERROR;
    }
    res = Bucket_InsertPair(&BUCKET_BY_KEY(_map, _key), _key, _value);
    if(MAP_SUCCESS == res)
    {
	    _map->m_numItems++;
	    _map->m_numInserted++;
    }
    return res;
}

/*-----------------------------------------------------------------*/
ProtectedHashUpsertStatus ProtectedHash_Upsert(Protected_Hash* _proHash, const void* _key, ValueInsertFunction _insertFunc, ValueUpdateFunction _updateFunc, void *_context)
{
    void* findValue;
    ProtectedHashUpsertStatus upsertStatus;

    /* get lock per bucket */
    int bucketIndex = INDEX(_proHash, _key);
    int lockIndex = LOCK_INDEX(_proHash->m_numOfLocks, bucketIndex);
    pthread_mutex_t *mutex = LOCK(_proHash, lockIndex);

    /* lock relavant bucket grup */
    pthread_mutex_lock(mutex);

    /* check value in bucket and update accordingly */
    if ( MAP_SUCCESS != HashMap_Find( _proHash, _key, &findValue))
    {
        void *newValue = _insertFunc(_context);
        HashMap_Insert(_proHash, _key, newValue);
        upsertStatus =  E_PROHASH_INSERT;
    }
    else
    {
    	_updateFunc(findValue, _context);
        upsertStatus = E_PROHASH_UPDATE;
    }

    /* unlock relavant bucket grup */
    pthread_mutex_unlock(mutex);

    return upsertStatus;
}

/*-----------------------------------------------------------------*/

ProtectedHashRetrieveStatus ProtectedHash_Retrieve(Protected_Hash* _proHash, const void* _key, void** _value)
{
    ProtectedHashRetrieveStatus retrieveStatus;

    /* get lock per bucket */
    int bucketIndex = INDEX(_proHash, _key);
    int lockIndex = LOCK_INDEX(_proHash->m_numOfLocks, bucketIndex);
    pthread_mutex_t *mutex = LOCK(_proHash, lockIndex);

    /* lock relavant bucket group */
    pthread_mutex_lock(mutex);

    /* check value in bucket and update accordingly */
    retrieveStatus = ( MAP_SUCCESS == HashMap_Find( _proHash, _key, _value)) ? E_PROHASH_SUCCESS : E_PROHASH_NOT_FOUND;

    /* unlock relavant bucket grup */
    pthread_mutex_unlock(mutex);

    return retrieveStatus;
}

/***********************************************************************/

size_t ProtectedHash_ForEach(Protected_Hash* _proHash, KeyValueActionFunction _action, void* _context)
{
    int i = 0;
    size_t count = 0;
    ListItr itr;
    Pair *pair;
    pthread_mutex_t *mutex;
    int lockIndex;
    if (!IS_VALID(_proHash))
    {
        return MAP_UNINITIALIZED_ERROR;;
    }
    if(_action)
    {
        lockIndex = LOCK_INDEX(_proHash->m_numOfLocks, i);
        mutex = LOCK(_proHash, lockIndex);

        for(i = 0; i < _proHash->m_size; i++)
        {
            /* if bucket group changed - update lock */
            if (lockIndex != LOCK_INDEX(_proHash->m_numOfLocks, i))
            {
                pthread_mutex_unlock(mutex);
                mutex = LOCK(_proHash, lockIndex);
                pthread_mutex_lock(mutex);
            }

            for (itr = ListItr_Begin(BUCKET(_proHash, i)); itr != ListItr_End(BUCKET(_proHash, i)); itr = ListItr_Next(itr))
            {
                pair = ListItr_Get(itr);
                if(!_action((void *)KEY(pair),(void *)VALUE(pair),_context))
                {
                    break;
                }
                count++;
            }
        }
        pthread_mutex_unlock(mutex);
    }
    return count;
}
