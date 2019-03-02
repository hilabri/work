#ifndef __HEAP_H__
#define __HEAP_H__

#include <stddef.h>
#include "heap.h"
#include "vector.h"

typedef int (*LessComparator)(void* _parent, void* _son);
typedef int (*HeapElementAction)(void* _element, size_t _index, void* _context);
typedef struct Heap Heap;

typedef enum 
{
    HEAP_SUCCESS = 0,                /**< On success      */
    HEAP_UNINITIALIZED_ERROR,        /**< Uninitialized map error      */
    HEAP_ALLOCATION_ERROR            /**< Allocation error             */
} Heap_Result;

/**  
 * @brief Build a max double heap using heap DS 
 * @param[in] _vec - pointer to heap which will be used as base for the heap.
 * @return Heap * - on success / NULL on fail of heap building or sorting.
 *
 *sComparator
 */
Heap* Heap_Build(Vector* _vec,LessComparator _less);


/**  
 * @brief  deallocate of previously allocated heap. vector remains untouched.  
 * @param[in] _heap - heap to be deallocated.
 * @return void
 */
void Heap_Destroy(Heap* *_heap);

/**  
 * @brief Enqueue an Item to the heap and sorting by max.  
 * @param[in] _heap - Heap to operate on.
 * @param[in] _data - Item to inseart.
 * @return success or error code 
 */
Heap_Result Heap_Insert(Heap* _heap, void* _data);


/**  
 * @brief retrive the max value in the Heap.  
 * @param[in] _heap - Heap to operate on.
 * @param[out] _pItem - pointer to variable that will receive the Max item
 * @return success or error code 
 */
Heap_Result Heap_Max(Heap* _heap, void* *_data);


/**  
 * @brief retrive and remove the max value in the Heap.  
 * @param[in] _heap - Heap to operate on.
 * @param[out] _pItem - pointer to variable that will receive the Max item
 * @return success or error code 
 */
Heap_Result Heap_ExtractMax(Heap* _heap, void* *_data);

/**  
 * @brief retrive the size of the Heap.  
 * @param[in] _heap - Heap to operate on.
 * @return value of the heap size 
 */
int Heap_ItemsNum(const Heap* _heap);

/*  
 * @brief Iterate over all elements in the heap.
 * @details The user provided _action function will be called for each element
 *          if _action return a zero for an element the iteration will stop.  
 * @param[in] _heap - heap to iterate over.
 * @param[in] _action - User provided function pointer to be invoked for each element
 * @param[in] _context - User provided context, will be sent to _action
 * @returns number of times the user functions was invoked
 */
size_t Heap_ForEach(const Heap* _heap, HeapElementAction _action, void* _context);

#endif /*__HEAP_H__*/
