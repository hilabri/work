#include "queue.h"
#include <stdlib.h>

#define QUEUE_MAGIC 0xDEADBEEF
#define IS_VALID(Q)            ((Q) && (QUEUE_MAGIC == (Q)->m_magic)) 
#define UNINITIALIZED (-1)
#define FALSE 0

struct Queue {
    void **m_items;
    size_t m_size; /* capacity */
    size_t m_head; /* start of the queue */
    size_t m_tail; /* end of the queue */
    size_t m_nitems; /* num of items*/
    size_t m_magic;
};


Queue* Queue_Create(size_t _maxSize)
{
    Queue *queue;
    if (0 == _maxSize)
    {   
        return NULL;
    }
    queue = (Queue *)malloc(sizeof(Queue));
    if (!queue)
    {
        return  NULL;
    }
    queue->m_items = malloc(_maxSize * sizeof(void *));
    if (!queue->m_items) 
    {
        free(queue);
        return NULL;
    }
    queue->m_size = _maxSize;
    queue->m_head = 0;
    queue->m_tail = 0;
    queue->m_nitems = 0;
    queue->m_magic = QUEUE_MAGIC;
    return queue;
}

/**************************************************************************/

void Queue_Destroy(Queue** _queue, void (*_elementDestroy)(void* _item))
{
    int i;
    if (!_queue || !IS_VALID(*_queue))
    {
        return;
    }
    (*_queue)->m_magic = UNINITIALIZED;
    if (_elementDestroy)
    {
        for (i = 0; i < (*_queue)->m_nitems; i++)
        {
            _elementDestroy((*_queue)->m_items[i]);
        }
    }
    free((*_queue)->m_items);
    free(*_queue);
    *_queue = NULL;
}

/**************************************************************************/

Queue_Result Queue_Enqueue(Queue* _queue, void* _item)
{       
    if (!IS_VALID(_queue))
    {
        return QUEUE_UNINITIALIZED_ERROR;
    }   
    if (_queue->m_nitems == _queue->m_size) {
        return QUEUE_OVERFLOW;
    }
        
    _queue->m_items[_queue->m_tail] = _item;
    _queue->m_nitems++;
    _queue->m_tail++;
    _queue->m_tail %= _queue->m_size; 
    
    return QUEUE_SUCCESS; 
}
    
/**************************************************************************/

Queue_Result Queue_Dequeue(Queue* _queue, void** _pValue)
{
    if (!IS_VALID(_queue))
    {
        return QUEUE_UNINITIALIZED_ERROR;
    }
    if (0 == _queue->m_nitems)
    {
        return QUEUE_UNDERFLOW;
    }
    *_pValue = _queue->m_items[_queue->m_head];
    _queue->m_nitems--;
    _queue->m_head++;
    _queue->m_head %= _queue->m_size;
    return QUEUE_SUCCESS;
}

/**************************************************************************/

int Queue_IsEmpty(const Queue* _queue)
{
    return IS_VALID(_queue) ? (0 == _queue->m_nitems) : UNINITIALIZED;
}     

/**************************************************************************/

size_t Queue_ForEach(const Queue* _queue, QueueElementAction _action, void* _context)
{
    int i;
    if (!IS_VALID(_queue))
    {
        return QUEUE_UNINITIALIZED_ERROR;
    }
    for (i = 0; i < _queue->m_nitems; i++)
    {
        if(0 == _action(_queue->m_items[(_queue->m_head+i) % _queue->m_size], i, _context))
        {
            break;
        }
    }
    return i;
}
