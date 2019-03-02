#include <stdio.h>
#include <stdlib.h>
#include "mu_test.h"
#include "stack.h"

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

static void PushListIntItemsArray(Stack *stack, int *arr[], int _size)
{
    int i;
    for (i = 0; i < _size; i++)
    {
        Stack_Push(stack, arr[i]);
    }
}

UNIT(stack_create)
	Stack* stack = Stack_Create();
	ASSERT_THAT(stack);
	Stack_Destroy(&stack, NULL);
END_UNIT

UNIT(stack_destroy)
    int *array[5];
    Stack* stack;
    int item = 10;
    SetupIntItemsArray(array, 5);
    stack = Stack_Create();
	Stack_Destroy(NULL, ElementDestroy);
	Stack_Destroy(&stack, ElementDestroy);
    stack = Stack_Create();
    PushListIntItemsArray(stack, array, 5);
	Stack_Destroy(&stack, ElementDestroy);
    ASSERT_THAT(STACK_UNINITIALIZED_ERROR == Stack_Push(stack, &item));	
    ASSERT_THAT(NULL == stack);
END_UNIT

UNIT(stack_push)
	int *array[5];
	int *item;
    int i;
    Stack* stack = Stack_Create();
	SetupIntItemsArray(array, 5);
    for (i=0; i<5; i++)
	{
		ASSERT_THAT(STACK_SUCCESS == Stack_Push(stack, array[i]));
	}
    Stack_Pop(stack, (void **)&item);
    ASSERT_THAT(14 == *item);
    Stack_Destroy(&stack, ElementDestroy);
END_UNIT


UNIT(stack_push_uninitialized)
	Stack* stack;
    int item = 1;
	stack = Stack_Create();
	Stack_Destroy(&stack, NULL);
	ASSERT_THAT(STACK_UNINITIALIZED_ERROR == Stack_Push(stack, &item));
END_UNIT


UNIT(stack_pop)
	int *array[5];
	int *item;
    int i;
    Stack* stack = Stack_Create();
	SetupIntItemsArray(array, 5);
    PushListIntItemsArray(stack, array, 5);
    for (i=4; i>=0; i--)
	{
		ASSERT_THAT(STACK_SUCCESS == Stack_Pop(stack, (void **)&item));
        ASSERT_THAT(10+i == *item);
	}
	ASSERT_THAT(Stack_IsEmpty(stack));
    Stack_Destroy(&stack, ElementDestroy);
END_UNIT


UNIT(stack_pop_underflow)
    int *array[3];
	int *item;
	int i;
    Stack* stack = Stack_Create();
    ASSERT_THAT(STACK_UNDERFLOW_ERROR == Stack_Pop(stack, (void**)&item));
    SetupIntItemsArray(array, 3);
    PushListIntItemsArray(stack, array, 3);
    for (i = 0; i < 3; i++)
    {
        Stack_Pop(stack, (void **)&item);
    }
    ASSERT_THAT(STACK_UNDERFLOW_ERROR == Stack_Pop(stack, (void**)&item));
    Stack_Destroy(&stack, ElementDestroy);
END_UNIT

UNIT(stack_pop_uninitialized)
	int *item;
    Stack* stack = Stack_Create();
	Stack_Destroy(&stack, NULL);
	ASSERT_THAT(STACK_UNINITIALIZED_ERROR == Stack_Pop(stack, (void**)&item));
END_UNIT


UNIT(stack_top)
	int *array[5];
	int *item;
    int i;
    Stack* stack = Stack_Create();
	SetupIntItemsArray(array, 5);
    PushListIntItemsArray(stack, array, 5);
    for (i=4; i>=0; i--)
	{
		ASSERT_THAT(STACK_SUCCESS == Stack_Top(stack, (void **)&item));
		ASSERT_THAT(10+i == *item);
		ASSERT_THAT(STACK_SUCCESS == Stack_Pop(stack, (void **)&item));
	}
    Stack_Destroy(&stack, ElementDestroy);
END_UNIT

UNIT(stack_is_empty)
    int i;
    int *item;
    int *array[5];
    Stack* stack = Stack_Create();
    ASSERT_THAT(Stack_IsEmpty(stack));
    SetupIntItemsArray(array, 5);
    PushListIntItemsArray(stack, array, 5);
	ASSERT_THAT(!Stack_IsEmpty(stack));
    for(i = 0; i < 5; i++)
    {
        Stack_Pop(stack, (void**)&item);
    }
    ASSERT_THAT(Stack_IsEmpty(stack));
    Stack_Destroy(&stack, ElementDestroy);
END_UNIT

TEST_SUITE(stack container)
	TEST(stack_create)
	TEST(stack_destroy)

    TEST(stack_push)
	TEST(stack_push_uninitialized)
  
    TEST(stack_pop)
 
    TEST(stack_pop_underflow)
	TEST(stack_pop_uninitialized)

    TEST(stack_top)
    TEST(stack_is_empty)

END_SUITE
