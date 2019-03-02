#ifndef __PROTECTED_HASH_H__
#define __PROTECTED_HASH_H__

typedef struct Protected_Hash Protected_Hash;

typedef size_t (*HashFunction)(const void* _key);
typedef int (*KeyEqualityFunction)(const void* _firstKey, const void* _secondKey);
typedef int	(*KeyValueActionFunction)(const void* _key, void* _value, void* _context);
typedef void (*KeyDestroyFunction)(void* _key);
typedef void (*ValDestroyFunction)(void* _value);
typedef void (*ValueUpdateFuncion)(void* _value, const void* _update);

typedef enum 
{
	E_PROHASH_INSERT,
	E_PROHASH_UPDATE
} ProtectedHashUpsertStatus;

/** 
 * @brief create Protected hash
 *
 * @return newly created protect hash
 */
Protected_Hash* ProtectedHash_Create(size_t _capacity, size_t _numOfLocks, HashFunction _hashFunc, KeyEqualityFunction _keysEqualFunc);

/** 
 * @brief destroy protect hash
 *
 * @param[in] _protectH : pointer to the protect hash to destroy
 */
void ProtectedHash_Destroy(Protected_Hash* _protectHM, KeyDestroyFunction _keyDestroy, ValDestroyFunction _valDestroy);

/** 
 * @brief upsert to protect hash
 *
 * @param[in] _proHash : pointer to the protect hash 
 * @param[in] _key : a key to search in the protect hash
 * @param[in] _value : a value to check if exict in protect hash
 * @param[in] _updateSub : a call back function to update new data in the protect hash
 */
ProtectedHashUpsertStatus ProtectedHash_Upsert(Protected_Hash* _proHash, const void* _key, const void* _value , ValueUpdateFuncion _updateFunc);

/** 
 * @brief function that do some action on each value in the protect hash
 *
 * @param[in] _proHash : pointer to the protect hash 
 * @param[in] _action : action function to do on each value in the protect hash
 * @param[in] _context : optional context to the action function
 */
size_t ProtectedHash_ForEach(Protected_Hash* _proHash, KeyValueActionFunction _action, void* _context);


#endif /*__PROTECTED_HASH_H__*/
