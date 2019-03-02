#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>         /* size_t */
#include "vector.h"

#define REALLOC_REDUCE_FACTOR 2
#define MAGIC 0xDEADBEEF
#define IS_VALID(V)            ((V) && (MAGIC == (V)->m_magic)) 
#define IS_VALID_INDEX(V,X)    ((V)->m_size >= (X) && (X)>0)

#define UNITIALIZED -1;

struct Vector {
    size_t m_magic; /* is initialized*/
    void* *m_items; /*array*/
    size_t m_size; /*number of items*/
    size_t m_capacity; /*max num of items*/
    size_t m_blockSize; /*unit for realloc*/
    size_t m_initialCapacity; /*min cap*/
};

/*************************************************************************/
Vector* Vector_Create(size_t _initialCapacity, size_t _blockSize) 
{
    Vector *vector = NULL;
    if (0 == _initialCapacity && 0 == _blockSize)
    {
        return NULL;
    }
    vector = (Vector *)malloc(sizeof(Vector));
    if (!vector)
    {
        return NULL;
    }
    vector->m_initialCapacity = _initialCapacity;
    vector->m_blockSize = _blockSize;
    vector->m_capacity = _initialCapacity;  
    vector->m_size = 0;
    vector->m_magic = MAGIC;
    if(0 == _initialCapacity)
    {
        vector->m_items = NULL;
        return vector; 
    }
    vector->m_items = (void **)malloc(_initialCapacity*sizeof(void*)); 
    if (!vector->m_items) 
    {
        free(vector);
        return NULL;
    }
    return vector;
}

/*************************************************************************/

void Vector_Destroy(Vector* *_vector, void (*_elementDestroy)(void* _item))
{
    int i;
    if (!_vector || !IS_VALID(*_vector)) 
    {
	    return;
    }
    (*_vector)->m_magic = UNITIALIZED; 
    if (_elementDestroy) /* if user did not send NULL */
    {
		for (i = 0; i < (*_vector)->m_size; i++)
		{
	    	_elementDestroy((*_vector)->m_items[i]);
		}
    }
    free((*_vector)->m_items);
    free(*_vector);
    *_vector = NULL;
}

/*************************************************************************/
Vector_Result Vector_Append(Vector* _vector, void* _item) 
{
    if (!IS_VALID(_vector)) 
    {
        return VECTOR_UNITIALIZED_ERROR;
    }    
    if (_vector->m_size == _vector->m_capacity) 
    {
        if (_vector->m_blockSize == 0)
        {
            return VECTOR_OEVERFLOW;
        }
        if (_vector->m_capacity == 0)
        {
            _vector->m_items = malloc(_vector->m_blockSize*sizeof(void*));
        }
        else
        {
            _vector->m_items = (void**)realloc(_vector->m_items,(_vector->m_capacity+_vector->m_blockSize)*sizeof(void*)); 
        }
        if (!_vector->m_items) 
        {
            return VECTOR_ALLOCATION_ERROR;
        }
        _vector->m_capacity += _vector->m_blockSize;
    }
    _vector->m_items[_vector->m_size] = _item;
    _vector->m_size++;
    return VECTOR_SUCCESS;  
}

/*************************************************************************/
Vector_Result Vector_Remove(Vector* _vector, void** _pValue)
{
    size_t spareCap;
    size_t spareSize;
    if (!IS_VALID(_vector) || !_pValue)
    {
        return  VECTOR_UNITIALIZED_ERROR;
    }
    if (_vector->m_size == 0)
    {
        return VECTOR_UNDERFLOW;
    }
    _vector->m_size--;
    *_pValue = _vector->m_items[_vector->m_size];
    spareCap = _vector->m_capacity - _vector->m_size; 
    spareSize = REALLOC_REDUCE_FACTOR * _vector->m_blockSize;   
    if (spareCap == spareSize)
    {
        _vector->m_items = realloc(_vector->m_items, (_vector->m_capacity - _vector->m_blockSize)*sizeof(void *));
        if (!_vector->m_items)
        {       
            return VECTOR_ALLOCATION_ERROR;;
        }
        _vector->m_capacity -=  _vector->m_blockSize;
    }
    return VECTOR_SUCCESS;
}

/*************************************************************************/
Vector_Result Vector_Get(const Vector* _vector, size_t _index, void** _pValue)
{
    if (!IS_VALID(_vector) || !_pValue)
    {
        return VECTOR_UNITIALIZED_ERROR;
    }
     if(! IS_VALID_INDEX(_vector, _index))
     {
         return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
     }
    *_pValue = _vector->m_items[_index-1];
    return VECTOR_SUCCESS;  
}

/*************************************************************************/
Vector_Result Vector_Set(Vector* _vector, size_t _index, void*  _value)
{
     if (! IS_VALID(_vector) )
     {
         return VECTOR_UNITIALIZED_ERROR;
     }
     if(! IS_VALID_INDEX(_vector, _index))
     {
         return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
     }
    _vector->m_items[_index-1] = _value;
    return VECTOR_SUCCESS;  
}

/*************************************************************************/ 
size_t Vector_Size(const Vector* _vector)
{
    return (IS_VALID(_vector)) ? _vector->m_size : UNITIALIZED;
}
      
/*************************************************************************/
size_t Vector_Capacity(const Vector* _vector)
{
   return (IS_VALID(_vector)) ? _vector->m_capacity : UNITIALIZED;
}

/*************************************************************************/
size_t Vector_ForEach(const Vector* _vector, VectorElementAction _action, void* _context)
{
    int i; 
    void* elem;
    if (!IS_VALID(_vector))
    {
        return  VECTOR_UNITIALIZED_ERROR;
    }
    
	for(i = 1; i <= Vector_Size(_vector); i++)
	{
		Vector_Get(_vector, i, &elem);
		if(_action(elem, i, _context) == 0)
        {
            break;
        }
	}
    return i-1;
}

/*************************************************************************/

