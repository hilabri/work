#include "PQ.h"
#include "min_heap.h"
#include "vector.h"

#define MAGIC 0xDEADBEEF
#define DEAD_MAGIC 0xDEADDEAD
#define IS_VALID(V)            ((V) && (MAGIC == (V)->m_magic)) 
#define UNINITIALIZED -1

#define PQ_INITIAL_SIZE 100
#define PQ_EXTEND_SIZE 50

struct PQ 
{
    Vector *m_vector;
    Heap *m_items;
    size_t m_magic;
};

/***************************/

PQ* PQ_Create(ItemsComparator _func)
{
	PQ *pq = malloc(sizeof(PQ));
	if (!pq) 
    {
        return NULL;
    }
	pq->m_vector = Vector_Create(PQ_INITIAL_SIZE, PQ_EXTEND_SIZE);
	pq->m_items = Heap_Build(pq->m_vector, _func);
	pq->m_magic = MAGIC; 
	return pq;
}

/***************************/

void PQ_Destroy(PQ *_pq, void (*_elementDestroy)(void*))
{
	_pq->m_magic = DEAD_MAGIC;
    Heap_Destroy(&_pq->m_items);
	Vector_Destroy(&_pq->m_vector, _elementDestroy);
	free(_pq);
}

/***************************/

PQ_Result PQ_Add(PQ *_pq, void* _item)
{
	if(!IS_VALID(_pq))
	{
		return PQ_UNINITIALIZED_ERROR;
	}
    Heap_Insert(_pq->m_items, _item);
    return PQ_SUCCESS; 
}
/***************************/

PQ_Result PQ_Remove(PQ *_pq, void* *_item)
{
	if(!IS_VALID(_pq))
	{
	    return PQ_UNINITIALIZED_ERROR;			
	}
	Heap_ExtractMin(_pq->m_items, _item); 
	return PQ_SUCCESS;
}

/***************************/
int PQ_GetSize(PQ *_pq)
{
	if(!IS_VALID(_pq))
	{
		return UNINITIALIZED;
	}
	return Heap_ItemsNum(_pq->m_items);
}
