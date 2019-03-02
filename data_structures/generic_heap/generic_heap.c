#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "vector.h"

#define MAGIC 0XDEADBEEF
#define DEAD_MAGIC 0XDEADDEAD

#define IS_VALID(H)    ((H) && (MAGIC == (H)->m_magic))

#define IS_LESS(H,P,S)  ((H)->m_less)((P),(S))

struct Heap
{
    Vector *m_vec;
    size_t m_heapSize;
    size_t m_magic;
    LessComparator m_less;
};

/*************************************/
static size_t HasRightSon(Heap *_heap, size_t _parentIndex)
{
    return (_heap->m_heapSize > 2*(_parentIndex));
}

/*************************************/

static void HeapifyDown(Heap* _heap, size_t _Index, size_t _lastParent)
{
	size_t i;
	void* parent;
	void* leftSon;
	void* rightSon;
	for(i = _Index; i <= _lastParent; i++)
	{		
		Vector_Get(_heap->m_vec, i, &parent);
		Vector_Get(_heap->m_vec, 2*i, &leftSon); 
		if(IS_LESS(_heap,parent,leftSon))
		{
            Vector_Set(_heap->m_vec, i, leftSon);
			Vector_Set(_heap->m_vec, 2*i, parent);		
		}
		Vector_Get (_heap->m_vec,i, &parent);
        if (!HasRightSon(_heap, i))
        {
            return;
        }
        Vector_Get(_heap->m_vec, 2*i+1, &rightSon);		
		if(IS_LESS(_heap,parent,rightSon))
		{
			Vector_Set(_heap->m_vec, i, rightSon);
			Vector_Set(_heap->m_vec, 2*i+1, parent);
		}
	}
	return;
}
/*************************************/
static void HeapifyUp(Heap* _heap)
{
	size_t i;
	size_t lastParent = _heap->m_heapSize/2;
	void* parent;
	void* leftSon;
	void* rightSon;
    for(i = lastParent; i > 0; i--) /* start from 1, all in index -1 */
	{
        Vector_Get (_heap->m_vec, i, &parent);
		Vector_Get (_heap->m_vec, 2*i, &leftSon);
		if(IS_LESS(_heap, parent, leftSon))
		{
			HeapifyDown( _heap, i, lastParent);	
		}
        if(!HasRightSon(_heap, i))
        {
            return;
        }
		Vector_Get(_heap->m_vec,2*i+1,&rightSon);
		if(IS_LESS(_heap, parent, rightSon))
        {
            HeapifyDown(_heap, i, lastParent);
        }
	}
	return;
}

/*************************************/

Heap* Heap_Build(Vector* _vec, LessComparator _less)
{
	Heap *heap;
	if (!_vec || !_less)
    {
        return NULL;
    }
	heap = malloc(sizeof(Heap));
	if (!heap)
    {
        return NULL;
    }
    heap->m_vec = _vec;
	heap->m_heapSize = Vector_Size(heap->m_vec);
	heap->m_magic = MAGIC;
    heap->m_less = _less;
	HeapifyUp(heap);
	return heap;
}

/*************************************/

Heap_Result Heap_Insert(Heap* _heap, void* _data)
{
	if (!IS_VALID(_heap))
    {
        return HEAP_UNINITIALIZED_ERROR;
    }
    Vector_Append(_heap->m_vec, _data);	
    _heap->m_heapSize++;
    HeapifyUp(_heap);
	return HEAP_SUCCESS;
}

/*************************************/

Heap_Result Heap_Max(Heap* _heap, void* *_data)
{
	if (!IS_VALID(_heap))
    {
        return HEAP_UNINITIALIZED_ERROR;
    }
	Vector_Get(_heap->m_vec, 1, _data);
	return HEAP_SUCCESS;
}
/*************************************/

 Heap_Result Heap_ExtractMax(Heap* _heap, void* *_data)
{
	void* lastItem;	
	void* removed;
    if (!IS_VALID(_heap) || !_data)
    {
        return HEAP_UNINITIALIZED_ERROR;
    }
	Vector_Get(_heap->m_vec, 1, _data);
    Vector_Get(_heap->m_vec, _heap->m_heapSize, &lastItem);
	Vector_Set(_heap->m_vec, 1, lastItem);
	Vector_Set(_heap->m_vec, _heap->m_heapSize, *_data);
    Vector_Remove(_heap->m_vec, &removed);
	_heap->m_heapSize--;
	HeapifyUp(_heap);
	return HEAP_SUCCESS;
}
/*************************************/

int Heap_ItemsNum(const Heap* _heap) 
{
	if (!IS_VALID(_heap))
    {
        return HEAP_UNINITIALIZED_ERROR;
    }
	return _heap->m_heapSize;
}
/*************************************/

void Heap_Destroy(Heap* *_heap)
{
    if (!_heap || !IS_VALID(*_heap))
    {
        return;
    }
	(*_heap)->m_magic = DEAD_MAGIC;
	free(*_heap);
    *_heap = NULL;
}
/*************************************/

size_t Heap_ForEach(const Heap* _heap, HeapElementAction _action, void* _context)
 {   
     size_t i;
     void* element;
     if (!IS_VALID(_heap))
     {
         return  HEAP_UNINITIALIZED_ERROR;
     }
  
     for(i = 1; i <= Heap_ItemsNum(_heap); i++)
     {
         Vector_Get(_heap->m_vec, i, &element);
         if(0 == _action(element, i, _context))
         {
             break;
         }
     }
     return i-1;
 }


