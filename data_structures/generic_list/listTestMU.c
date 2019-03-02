#include <stdio.h>
#include <stdlib.h>
#include "mu_test.h"
#include "list.h"
#include "list_itr.h"

typedef int* Item;

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

static void PushListIntItemsArray(List *list, int *arr[], int _size)
{
    int i;
    for (i = 0; i < _size; i++)
    {
        List_PushHead(list, arr[i]);
    }
}

UNIT(list_create)
	List* list = List_Create();
	ASSERT_THAT(list);
	List_Destroy(&list, NULL);
END_UNIT

UNIT(list_destroy)
    int *array[5];
    List *list;
    int item = 10;
    SetupIntItemsArray(array, 5);
    list = List_Create();
	List_Destroy(NULL, ElementDestroy);
	List_Destroy(&list, ElementDestroy);
    list = List_Create();
    PushListIntItemsArray(list, array, 5);
	List_Destroy(&list, ElementDestroy);
    ASSERT_THAT(LIST_UNINITIALIZED_ERROR == List_PushHead(list, &item));	
    ASSERT_THAT(NULL == list);
END_UNIT

UNIT(list_push_head)
	int *array[5];
	int *item;
    int i;
    List* list = List_Create();
	SetupIntItemsArray(array, 5);
    for (i=0; i<5; i++)
	{
		ASSERT_THAT(LIST_SUCCESS == List_PushHead(list, array[i]));
	}
    List_PopHead(list, (void **)&item);
    ASSERT_THAT(14 == *item);
    List_PopTail(list, (void **)&item);
    ASSERT_THAT(10 == *item);
    List_Destroy(&list, ElementDestroy);
END_UNIT

UNIT(list_push_tail)
	int *array[5];
	int *item;
    int i;
    List* list = List_Create();
	SetupIntItemsArray(array, 5);
    for (i=0; i<5; i++)
	{
		ASSERT_THAT(LIST_SUCCESS == List_PushTail(list, array[i]));
	}
    List_PopHead(list, (void **)&item);
    ASSERT_THAT(10 == *item);
    List_PopTail(list, (void **)&item);
    ASSERT_THAT(14 == *item);
    List_Destroy(&list, ElementDestroy);
END_UNIT

UNIT(list_push_uninitialized)
	List *list;
    int item = 1;
	list = List_Create();
	List_Destroy(&list, NULL);
	ASSERT_THAT(LIST_UNINITIALIZED_ERROR == List_PushHead(list, &item));
    ASSERT_THAT(LIST_UNINITIALIZED_ERROR == List_PushTail(list, &item));
END_UNIT

UNIT(list_pop_head)
	int *array[5];
	int *item;
    int i;
    List* list = List_Create();
	SetupIntItemsArray(array, 5);
    PushListIntItemsArray(list, array, 5);
    for (i=4; i>=0; i--)
	{
		ASSERT_THAT(LIST_SUCCESS == List_PopHead(list, (void **)&item));
        ASSERT_THAT(10+i == *item);
        ASSERT_THAT(i == List_Size(list));
	}
    List_Destroy(&list, ElementDestroy);
END_UNIT

UNIT(list_pop_tail)
	int *array[5];
	int *item;
    int i;
    List* list = List_Create();
	SetupIntItemsArray(array, 5);
    PushListIntItemsArray(list, array, 5);
    for (i=0; i<5; i++)
	{
		ASSERT_THAT(LIST_SUCCESS == List_PopTail(list, (void **)&item));
        ASSERT_THAT(10+i == *item);
        ASSERT_THAT(4-i == List_Size(list));
	}
    List_Destroy(&list, ElementDestroy);
END_UNIT

UNIT(list_pop_underflow)
    int *array[3];
	int *item;
	int i;
    List* list = List_Create();
    ASSERT_THAT(LIST_UNDERFLOW_ERROR == List_PopHead(list, (void**)&item));
	ASSERT_THAT(LIST_UNDERFLOW_ERROR == List_PopTail(list, (void**)&item));

    SetupIntItemsArray(array, 3);
    PushListIntItemsArray(list, array, 3);
    for (i = 0; i < 3; i++)
    {
        List_PopHead(list, (void **)&item);
    }
    ASSERT_THAT(LIST_UNDERFLOW_ERROR == List_PopHead(list, (void**)&item));
	ASSERT_THAT(LIST_UNDERFLOW_ERROR == List_PopTail(list, (void**)&item));
    List_Destroy(&list, ElementDestroy);
END_UNIT

UNIT(list_pop_uninitialized)
	int *item;
    List* list = List_Create();
	List_Destroy(&list, NULL);
	ASSERT_THAT(LIST_UNINITIALIZED_ERROR == List_PopHead(list, (void**)&item));
	ASSERT_THAT(LIST_UNINITIALIZED_ERROR == List_PopTail(list, (void**)&item));
END_UNIT

UNIT(list_size)
    int i;
    int *item;
    int *array[5];
    List *list = NULL;
	ASSERT_THAT(-1 == List_Size(list));
    list = List_Create();
    ASSERT_THAT(0 == List_Size(list));
    SetupIntItemsArray(array, 5);
    PushListIntItemsArray(list, array, 5);
	ASSERT_THAT(5 == List_Size(list));
    for(i = 0; i < 5; i++)
    {
        List_PopHead(list, (void**)&item);
	    ASSERT_THAT(4-i == List_Size(list));
    }
    List_Destroy(&list, ElementDestroy);
END_UNIT

TEST_SUITE(list container)
	TEST(list_create)
	TEST(list_destroy)

    TEST(list_push_head)
    TEST(list_push_tail)
	TEST(list_push_uninitialized)
    
    TEST(list_pop_head)
    TEST(list_pop_tail)
    TEST(list_pop_underflow)
    TEST(list_pop_uninitialized)

    TEST(list_size)
END_SUITE
