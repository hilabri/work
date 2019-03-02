/** @file HashSet.h
 *  @brief HashSet of positive integers (excluding zero)
 *
 *
 *  @author Muhammad Zahalqa  (m@tryfinally)
 *	modified by Alex Katz 2/7/2018 - HashSet functions naming cleanup
 *
 *  @bug No known bugs.
 */

#ifndef _HASHSET_H___
#define _HASHSET_H___

#include "ADTDefs.h"

typedef struct HashSet HashSet;

/** @brief Create a HashSet ADT for managing a set of positive integers
 * @param _capacity: capacity of set
 * @param _loadFactor: max cardinality/capacity
 * @param _hashFunction: hashing function from size_t to size_t
 * @return Created HashSet or NULL on error
 */
HashSet* HashSetCreate(size_t _capacity, float _loadFactor, size_t (*_hashFunction)(size_t));

/** @brief Kaboom
 */
void HashSetDestroy(HashSet* _set);

/** @brief Add element to HashSet
 * Insert data into set if not already there.
 * Average time complexity O(1).
 * Worst: O(n)
 *
 * @params _set : A previously created HashSet ADT returned via HashSetCreate
 * @params _data: an unsigned int > 0 to add to the set.
 * @returns result code
 * @retval ERR_OK on success
 * @retval ERR_ALREADY_EXISTS
 * @retval ERR_OVERFLOW
 */
ADTErr HashSetInsert(HashSet* _set, size_t _data);

/** @brief Remove element from HashSet
 * Average time complexity O(1). Worst: O(n)
 *
 *
 * ERR_OK - if successful, ERR_NOT_FOUND - otherwise
 */
ADTErr HashSetRemove(HashSet* _set, size_t _data);

/** @brief Does HashSet contains element
 * returns 0 if set does not contain data, !0 if it does
 */
int HashSetContains(const HashSet* _set, size_t _data);

/** @brief Get cardinality of set ( number of elements in set )
 */
size_t HashSetSize(const HashSet* _set);

/** @brief Retrieve statistics
 * _averageCollisions == all collisions/all operations
 * _maxCollisionsEver : max colisions in one operation
 */
ADTErr HashSetStatistics(const HashSet* _set, size_t *_maxCollisionsEver, float *_averageCollisions);

/** @brief iterate over all elements
 */
void HashSetForEach(const HashSet* _hash, void (*_userFunction)(size_t _data));

#endif /* _HASHSET_H___ */
