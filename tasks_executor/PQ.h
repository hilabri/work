#ifndef __PQ_H__
#define __PQ_H__

#include <stddef.h>          
#include <stdlib.h>
#include <stdio.h>

typedef struct PQ PQ;
typedef int (*ItemsComparator)(void* _itemA, void *_itemB);

typedef enum PQ_Result {
	PQ_SUCCESS = 0,
	PQ_UNINITIALIZED_ERROR,			
	PQ_ALLOCATION_ERROR
} PQ_Result;

/** 
 * @brief Create a priority queue
 * @returns a pointer to the created queue
 * @retval NULL on failure due to allocation failure
 */
PQ* PQ_Create(ItemsComparator _func);

/** 
 * @brief Destroy a priority queue
 * @params[in] _pq - queue to be deallocated.
 * @params[in] _elementDestroy - A function pointer to be used to destroy all elements  
*/
void PQ_Destroy(PQ* _pq, void (*_elementDestroy)(void*));

/** 
 * @brief add an item to the priority queue   
 * @param[in] _pq - the queue
 * @param[in] _item - item to add  
 * @return PQ_SUCCESS on success, error code otherwise
 */
PQ_Result PQ_Add(PQ *_pq, void* _item);

/** 
 * @brief retrieve the first item from the queue   
 * @param[in] _pq - the queue
 * @param[out] _item - first item  
 * @return PQ_SUCCESS on success, error code otherwise
 */
PQ_Result PQ_Remove(PQ *_pq, void* *_item);

/** 
 * @brief get the amount of items in queue   
 * @param[in] _pq - the queue  
 * @return item count or -1 on uninitialized error 
 */
int PQ_GetSize(PQ *_pq);


#endif /* __PQ_H__ */
