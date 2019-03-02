#include <stdio.h>
#include <stdlib.h>
#include "mu_test.h"
#include "queue.h"


typedef int* Item;

static int QueuePrintElement(void *_element, size_t _index, void *_string)
{   
    printf("|%s %lu: %d| \n", (char *)_string, _index, *((int *)_element));
    return 1;
}   
    
static void ElementDestroy(void* _element)
{   
    free((int*)_element);
}

static void SetupIntItemsArray(int *arr[], int _size)
{   
    int i;
    int *item;
    for (i = 0; i < _size; i++)
    {
        item = malloc(sizeof(int));
        *item = i + 10;
        arr[i] = item;
    }
}

static void EnqueueIntItemsArray(Queue *queue, int *arr[], int _size)
{
    int i;
    for (i = 0; i < _size; i++)
    {
        Queue_Enqueue(queue, arr[i]);
    }
}

UNIT(queue_create)
	Queue* queue = Queue_Create(5);
	ASSERT_THAT(queue);
	Queue_Destroy(&queue, NULL);
END_UNIT

UNIT(queue_destroy)
    int *array[5];
    Queue *queue;
    int item = 10;
    SetupIntItemsArray(array, 5);
    queue = Queue_Create(5);
	Queue_Destroy(NULL, ElementDestroy);
	Queue_Destroy(&queue, ElementDestroy);
    queue = Queue_Create(5);
    EnqueueIntItemsArray(queue, array, 5);
	Queue_Destroy(&queue, ElementDestroy);
    ASSERT_THAT(QUEUE_UNINITIALIZED_ERROR == Queue_Enqueue(queue, &item));	
    ASSERT_THAT(NULL == queue);
END_UNIT

UNIT(queue_enqueue)
	int *array[6];
	int *item;
    int i;
    Queue* queue = Queue_Create(5);
	SetupIntItemsArray(array, 6);
    for (i=0; i<5; i++)
	{
		ASSERT_THAT(QUEUE_SUCCESS == Queue_Enqueue(queue, array[i]));
	}
    Queue_Dequeue(queue, (void **)&item);
    ASSERT_THAT(QUEUE_SUCCESS == Queue_Enqueue(queue, array[5]));
    Queue_Destroy(&queue, ElementDestroy);
END_UNIT


UNIT(queue_enqueue_overflow)
	int *array[6];
	int i;
    Queue* queue = Queue_Create(5);
	SetupIntItemsArray(array, 5);
    for (i=0; i<5; i++)
	{
		ASSERT_THAT(QUEUE_SUCCESS == Queue_Enqueue(queue, array[i]));
	}
	ASSERT_THAT(QUEUE_OVERFLOW == Queue_Enqueue(queue, array[5]));
	Queue_Destroy(&queue, ElementDestroy);
END_UNIT

UNIT(queue_enqueue_uninitialized)
	Queue *queue;
    int item = 1;
	queue = Queue_Create(5);
	Queue_Destroy(&queue, NULL);
	ASSERT_THAT(QUEUE_UNINITIALIZED_ERROR == Queue_Enqueue(queue, &item));
	/* 0 size queue */
    queue = Queue_Create(0);
	ASSERT_THAT(QUEUE_UNINITIALIZED_ERROR == Queue_Enqueue(queue, &item));
	Queue_Destroy(&queue, NULL);
END_UNIT

UNIT(queue_dequeue)
	int i;
    int *item; 
    int *array[5];
    Queue *queue;
    SetupIntItemsArray(array, 5);
    queue = Queue_Create(5);
    EnqueueIntItemsArray(queue, array, 5);
	for (i=0; i<5; i++)
	{
		ASSERT_THAT(QUEUE_SUCCESS == Queue_Dequeue(queue, (void **)&item));
        ASSERT_THAT((10+i) == *item);
	}
	Queue_Destroy(&queue, ElementDestroy);
END_UNIT

UNIT(queue_dequeue_uninitialized)
	int *item;
    Queue* queue = Queue_Create(5);
	Queue_Destroy(&queue, NULL);
	ASSERT_THAT(QUEUE_UNINITIALIZED_ERROR == Queue_Dequeue(queue, (void**)&item));
END_UNIT

UNIT(queue_dequeue_underflow)
	int *item;
	Queue* queue = Queue_Create(5);
	ASSERT_THAT(QUEUE_UNDERFLOW == Queue_Dequeue(queue, (void**)&item));
	Queue_Destroy(&queue, NULL);
END_UNIT

UNIT(queue_is_empty)
    int i;
    int *item;
    int *array[5];
    Queue *queue = NULL;
	ASSERT_THAT(-1 == Queue_IsEmpty(queue));
    SetupIntItemsArray(array, 5);
    queue = Queue_Create(5);
    ASSERT_THAT(Queue_IsEmpty(queue));
    EnqueueIntItemsArray(queue, array, 5);
	ASSERT_THAT(!Queue_IsEmpty(queue));
    for(i = 0; i < 5; i++)
    {
        Queue_Dequeue(queue, (void**)&item);
    }
	ASSERT_THAT(Queue_IsEmpty(queue));
    Queue_Destroy(&queue, ElementDestroy);
END_UNIT


UNIT(queue_for_each)
    int count;
    int *array[5];
    Queue *queue;
    SetupIntItemsArray(array, 5);
    queue = Queue_Create(5);
    EnqueueIntItemsArray(queue, array, 5);
    count = Queue_ForEach(queue, QueuePrintElement, "Index");
    ASSERT_THAT(5 == count);
    Queue_Destroy(&queue, ElementDestroy);    
END_UNIT

TEST_SUITE(queue container)
	TEST(queue_create)
	TEST(queue_destroy)
	
	TEST(queue_enqueue)
	TEST(queue_enqueue_overflow)
	TEST(queue_enqueue_uninitialized)

	TEST(queue_dequeue)
	TEST(queue_dequeue_uninitialized)
	TEST(queue_dequeue_underflow)

    TEST(queue_is_empty)

    TEST(queue_for_each)
END_SUITE
