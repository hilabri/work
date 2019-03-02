#include <stdio.h>
#include <stdlib.h>
#include "mu_test.h"
#include "vector.h"

typedef int* Item;

static int VectorPrintElement(void *_element, size_t _index, void *_string)
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

static void AppendIntItemsArray(Vector *vector, int *arr[], int _size)
{
    int i;
    for (i = 0; i < _size; i++)
    {
        Vector_Append(vector, arr[i]);
    }
}

UNIT(vector_create)
	Vector* vector = Vector_Create(5,5);
	ASSERT_THAT(vector);
	Vector_Destroy(&vector, NULL);
END_UNIT

UNIT(vector_destroy)
    int *array[5];
    Vector *vector;
    int item = 10;
    SetupIntItemsArray(array, 5);
    vector = Vector_Create(20,4);
	Vector_Destroy(NULL, ElementDestroy);
	Vector_Destroy(&vector, ElementDestroy);
    AppendIntItemsArray(vector, array, 5);
	Vector_Destroy(&vector, ElementDestroy);
    ASSERT_THAT(VECTOR_UNITIALIZED_ERROR == Vector_Append(vector, &item));	
    ASSERT_THAT(NULL == vector);
END_UNIT

UNIT(vector_append)
	int *array[10];
	int i;
    Vector* vector = Vector_Create(4,2);
	SetupIntItemsArray(array, 10);
    for (i=0; i<10; i++)
	{
		ASSERT_THAT(VECTOR_SUCCESS == Vector_Append(vector, array[i]));
	}
	Vector_Destroy(&vector, ElementDestroy);
    /* initial size == 0 */
	vector = Vector_Create(0,2);
	SetupIntItemsArray(array, 10);
    for (i=0; i<10; i++)
	{
		ASSERT_THAT(VECTOR_SUCCESS == Vector_Append(vector, array[i]));
	}
	Vector_Destroy(&vector, ElementDestroy);
    /* extend size == 0 */
	vector = Vector_Create(10,0);
	SetupIntItemsArray(array, 10);
    for (i=0; i<10; i++)
	{
		ASSERT_THAT(VECTOR_SUCCESS == Vector_Append(vector, array[i]));
	}
	Vector_Destroy(&vector, ElementDestroy);
END_UNIT


UNIT(vector_append_overflow)
	int *array[10];
	int i;
    Vector* vector = Vector_Create(9,0);
	SetupIntItemsArray(array, 10);
    for (i=0; i<9; i++)
	{
		ASSERT_THAT(VECTOR_SUCCESS == Vector_Append(vector, array[i]));
	}
	ASSERT_THAT(VECTOR_OEVERFLOW == Vector_Append(vector, array[9]));
	Vector_Destroy(&vector, ElementDestroy);
END_UNIT

UNIT(vector_append_uninitialized)
	Vector *vector;
    int item = 1;
	vector = Vector_Create(4,2);
	Vector_Destroy(&vector, NULL);
	ASSERT_THAT(VECTOR_UNITIALIZED_ERROR == Vector_Append(vector, &item));
	/* 0 size vector */
    vector = Vector_Create(0,0);
	ASSERT_THAT(VECTOR_UNITIALIZED_ERROR == Vector_Append(vector, &item));
	Vector_Destroy(&vector, NULL);
END_UNIT

UNIT(vector_remove)
	int i;
    int *item; 
    int *array[5];
    Vector *vector;
    SetupIntItemsArray(array, 5);
    vector = Vector_Create(20,4);
    AppendIntItemsArray(vector, array, 5);
	for (i=4; i>=0; i--)
	{
		ASSERT_THAT(VECTOR_SUCCESS == Vector_Remove(vector, (void **)&item));
        ASSERT_THAT((10+i) == *item);
	}
	Vector_Destroy(&vector, ElementDestroy);
END_UNIT

UNIT(vector_remove_uninitialized)
	int *item;
    Vector* vector = Vector_Create(4,2);
	Vector_Destroy(&vector, NULL);
	ASSERT_THAT(VECTOR_UNITIALIZED_ERROR == Vector_Remove(vector, (void**)&item));
END_UNIT

UNIT(vector_remove_underflow)
	int *item;
	Vector* vector = Vector_Create(4,2);
	ASSERT_THAT(VECTOR_UNDERFLOW == Vector_Remove(vector, (void**)&item));
	Vector_Destroy(&vector, NULL);
END_UNIT

UNIT(vector_get)
    int *array[5];
    int *item;
    int i;
    Vector *vector;
    SetupIntItemsArray(array, 5);
    vector = Vector_Create(2,4);
    AppendIntItemsArray(vector, array, 5);
	for (i=0; i<5; i++)
	{
		ASSERT_THAT(VECTOR_SUCCESS == Vector_Get(vector, i+1, (void **)&item));
        ASSERT_THAT((10+i) == *item);
	}
	Vector_Destroy(&vector, ElementDestroy);
END_UNIT

UNIT(vector_get_uninitialized)
	Item item;
	Vector* vector = Vector_Create(4,2);
	Vector_Destroy(&vector, NULL);
	ASSERT_THAT(VECTOR_UNITIALIZED_ERROR == Vector_Get(vector, 2, (void**)&item));
END_UNIT

UNIT(vector_get_bad_index)
    int *array[5];
    int *item;
    Vector *vector;
    SetupIntItemsArray(array, 5);
    vector = Vector_Create(2,4);
    AppendIntItemsArray(vector, array, 5);
    ASSERT_THAT(VECTOR_INDEX_OUT_OF_BOUNDS_ERROR == Vector_Get(vector, 7, (void**)&item));
	ASSERT_THAT(VECTOR_INDEX_OUT_OF_BOUNDS_ERROR == Vector_Get(vector, 0, (void**)&item));
	Vector_Destroy(&vector, ElementDestroy);
END_UNIT

UNIT(vector_set)
	int *array[10];
    int *item;
	int i;
    Vector* vector = Vector_Create(4,2);
	SetupIntItemsArray(array, 10);
    AppendIntItemsArray(vector, array, 5);
    for (i=0; i<5; i++)
	{
		ASSERT_THAT(VECTOR_SUCCESS == Vector_Set(vector, i+1, array[i+5]));
        ASSERT_THAT(VECTOR_SUCCESS == Vector_Get(vector, i+1, (void **)&item));
        ASSERT_THAT(*array[i+5] == *item);
	}
	Vector_Destroy(&vector, ElementDestroy);
END_UNIT

UNIT(vector_set_uninitialized)
	int item;
	Vector* vector = Vector_Create(4,2);
	Vector_Destroy(&vector, NULL);
	item = 10;
    ASSERT_THAT(VECTOR_UNITIALIZED_ERROR == Vector_Set(vector, 2, &item));
END_UNIT

UNIT(vector_set_bad_index)
    int *array[5];
    int item;
    Vector *vector;
    SetupIntItemsArray(array, 5);
    vector = Vector_Create(2,4);
    AppendIntItemsArray(vector, array, 5);
    
    item = 10;
    ASSERT_THAT(VECTOR_INDEX_OUT_OF_BOUNDS_ERROR == Vector_Set(vector, 7, &item));
	ASSERT_THAT(VECTOR_INDEX_OUT_OF_BOUNDS_ERROR == Vector_Set(vector, 0, &item));
	Vector_Destroy(&vector, ElementDestroy);
END_UNIT

UNIT(vector_size)
    int *array[5];
    Vector *vector;
    SetupIntItemsArray(array, 5);
    vector = Vector_Create(2,4);
    ASSERT_THAT(0 == Vector_Size(vector));
    AppendIntItemsArray(vector, array, 5);
	ASSERT_THAT(5 == Vector_Size(vector));
	Vector_Destroy(&vector, ElementDestroy);
END_UNIT


UNIT(vector_capacity)
    int *array[5];
    Vector *vector;
    SetupIntItemsArray(array, 5);
    vector = Vector_Create(2,4);
    ASSERT_THAT(2 == Vector_Capacity(vector));
    AppendIntItemsArray(vector, array, 5);
	ASSERT_THAT(6 == Vector_Capacity(vector));
	Vector_Destroy(&vector, ElementDestroy);
END_UNIT

UNIT(vector_for_each)
    int count;
    int *array[5];
    Vector *vector;
    SetupIntItemsArray(array, 5);
    vector = Vector_Create(2,4);
    AppendIntItemsArray(vector, array, 5);
    count = Vector_ForEach(vector, VectorPrintElement, "Index");
    ASSERT_THAT(5 == count);
    Vector_Destroy(&vector, ElementDestroy);    
END_UNIT

TEST_SUITE(vector container)
	TEST(vector_create)
	TEST(vector_destroy)
	
	TEST(vector_append)
	TEST(vector_append_overflow)
	TEST(vector_append_uninitialized)

	TEST(vector_remove)
	TEST(vector_remove_uninitialized)
	TEST(vector_remove_underflow)

	TEST(vector_get)
	TEST(vector_get_uninitialized)
	TEST(vector_get_bad_index)

	TEST(vector_set)
	TEST(vector_set_uninitialized)
	TEST(vector_set_bad_index)

	TEST(vector_size)
    TEST(vector_capacity)

    TEST(vector_for_each)
END_SUITE

