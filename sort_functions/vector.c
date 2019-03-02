#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>         /* size_t */
#include "ADTDefs.h"        /* ADTErr */
#include "vector.h"

#define REALLOC_REDUCE_FACTOR 2
#define VECTOR_MAGIC 0xDEADBEEF
#define IS_VECTOR(V)            ((V) && (VECTOR_MAGIC == (V)->m_magic)) 
#define IS_VECTOR_INDEX(V,X)    ((V)->m_size >= (X) && (X))



struct Vector {
    size_t m_magic; /* is initialized*/
    int *m_items; /*array*/
    size_t m_size; /*number of items*/
    size_t m_capacity; /*max num of items*/
    size_t m_extendSize; /*unit for realloc*/
    size_t m_initsize; /*min cap*/
};


Vector* VectorCreate(size_t _initialSize, size_t _extendSize) {
    Vector *v = NULL;
    if (0 == _initialSize && 0 == _extendSize)
    {
        return NULL;
    }
    v = (Vector *)malloc(sizeof(Vector));
    if (!v)
    {
        return NULL;
    }
    v->m_initsize = _initialSize;
    v->m_extendSize = _extendSize;
    v->m_capacity = _initialSize;  
    v->m_size = 0;
    v->m_magic = VECTOR_MAGIC;
    if(0 == _initialSize)
    {
        v->m_items = NULL;
        return v; 
    }
    v->m_items = (int *)malloc(_initialSize*sizeof(int)); 
    if (!v->m_items) 
    {
        free(v);
        return NULL;
    }
    return v;
}


void VectorDestroy (Vector *_vector) {
    if (IS_VECTOR(_vector)) 
    {
        _vector->m_magic = -1; 
        free(_vector->m_items);
        free(_vector);
    }
}


ADTErr VectorAdd  (Vector *_vector, int _item) {
    if (!IS_VECTOR(_vector)) 
    {
        return ERR_NOT_INITIALIZED;
    }    
    if (_vector->m_size == _vector->m_capacity) {
        if (_vector->m_extendSize == 0)
        {
            return ERR_OVERFLOW;
        }
        _vector->m_items = (int *)realloc(_vector->m_items,(_vector->m_capacity+_vector->m_extendSize)*sizeof(int)); 
        if (!_vector->m_items) 
        {
            return ERR_REALLOCATION_FAILED;
        }
        _vector->m_capacity += _vector->m_extendSize;
    }
    _vector->m_items[_vector->m_size] = _item;
    _vector->m_size++;
    return ERR_OK;  
}


ADTErr VectorDelete  (Vector *_vector,  int* _item) {
	size_t spareCap;
	size_t spareSize;
    if (_vector->m_size <= _vector->m_initsize)
    {
        return ERR_GENERAL;
    }
    if (! IS_VECTOR(_vector) || _item == NULL)
    {
        return ERR_NOT_INITIALIZED;
    }
    _vector->m_size--;
    *_item = _vector->m_items[_vector->m_size];
    spareCap = _vector->m_capacity - _vector->m_size; 
    spareSize = REALLOC_REDUCE_FACTOR * _vector->m_extendSize;   
    if (spareCap == spareSize) {
        _vector->m_items = realloc(_vector->m_items, (_vector->m_capacity - _vector->m_extendSize)*sizeof(int));
        if (!_vector->m_items)
        {       
            return ERR_REALLOCATION_FAILED;
        }
    }
    return ERR_OK;
}


ADTErr VectorGet (Vector *_vector, size_t _index, int *_item) {
    if (!IS_VECTOR(_vector) || _item == NULL )
    {
        return ERR_NOT_INITIALIZED;
    }
     if(! IS_VECTOR_INDEX(_vector, _index))
     {
         return ERR_WRONG_INDEX;
     }
    *_item = _vector->m_items[_index-1];
    return ERR_OK;  
}


ADTErr VectorSet (Vector *_vector, size_t _index, int  _item) {
 /* Index from 1 */ 
     if (! IS_VECTOR(_vector) )
     {
         return ERR_NOT_INITIALIZED;
     }
     if(! IS_VECTOR_INDEX(_vector, _index))
     {
         return ERR_WRONG_INDEX;
     }
    _vector->m_items[_index-1] = _item;
    return ERR_OK;  
}

 
int VectorGetSize (Vector *_vector) {
    return (IS_VECTOR(_vector)) ? _vector->m_size : -1;
}
      

int IsVectorEmpty (Vector *_vector) {
    return (IS_VECTOR(_vector)) ? (0 == _vector->m_size) : -1;
}


   /*  Unit-Test functions  */
void VectorPrint (Vector *_vector) {
	int i;
    if (! IS_VECTOR(_vector) )
    {
        printf("NOT INITIALIZED\n");
    }    
    for (i = 0; i < _vector->m_size; i++) 
    {
        printf("%d, ", _vector->m_items[i]);
    }
    printf("\n");
}


