#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "vector.h"
#include "mu_test.h"

static int Less(void* _a, void* _b)
{
    return (*(int*)_a < *(int*)_b);
}

static int PrintElement(void *_element, size_t _index, void *_string)
{   
    printf("|%s %lu: %d| \n", (char *)_string, _index, *((int *)_element));
    return 1;
}   

static void ElementDestroy(void* _element)
{  
    free((int*)_element);
}

static Vector* GenerateVector(void)
{
    Vector *vector = Vector_Create(20, 20);
    int i;
    int *item;
    int array[5] = {3,1,16,5,7};
    for (i = 0; i < 5; i++) {
        item = malloc(sizeof(int));
        *item = array[i];
        Vector_Append(vector, item);
    }
    return vector;
}

UNIT(heap_build)
    void* max;
    Heap_Result res;
    Vector *vector = GenerateVector();    
    Heap* heap  = Heap_Build(vector, Less);
    ASSERT_THAT(heap);
    res = Heap_Max(heap, &max);
    ASSERT_THAT(16 == *(int*)max);
    Heap_Destroy(&heap);
    Vector_Destroy(&vector, ElementDestroy);
END_UNIT

UNIT(heap_destroy)
    int item = 1;
    Vector *vector = GenerateVector();    
    Heap* heap = Heap_Build(vector, Less);
    Heap_Destroy(&heap);
    ASSERT_THAT(HEAP_UNINITIALIZED_ERROR == Heap_Insert(heap, &item));
    Vector_Destroy(&vector, ElementDestroy);
END_UNIT

UNIT(heap_insert)
    size_t i;
    void* max;
    int *item;
    Vector *vector = GenerateVector();
    Heap* heap = Heap_Build(vector, Less);
    int array[5] = {12,13,15,14,11};
    for (i=0; i<5; i++)
    {
        item = malloc(sizeof(int));
        *item = array[i];
        ASSERT_THAT(HEAP_SUCCESS == Heap_Insert(heap, item));
    }
    ASSERT_THAT(10 == Heap_ItemsNum(heap));
    Heap_Max(heap, &max);
    ASSERT_THAT(16 == *(int*)max);
    Heap_Destroy(&heap);
    Vector_Destroy(&vector, ElementDestroy);
END_UNIT

UNIT(heap_insert_uninitialized)
    int item = 1;
    Vector *vector = GenerateVector();
    Heap* heap = Heap_Build(vector, Less);
    Heap_Destroy(&heap);
    Vector_Destroy(&vector, ElementDestroy);
    heap = NULL;
    ASSERT_THAT(HEAP_UNINITIALIZED_ERROR == Heap_Insert(heap, &item));
END_UNIT

UNIT(heap_max)
    void* max;
    int result;
    int *item = malloc(sizeof(int));
    Vector *vector = GenerateVector();
    Heap* heap= Heap_Build(vector, Less);
    *item = 5;
    Heap_Insert(heap, item);
    result = Heap_Max(heap, &max);
    ASSERT_THAT(16 == *(int *)max);
    ASSERT_THAT(HEAP_SUCCESS  == result);
    Heap_Destroy(&heap);
    Vector_Destroy(&vector, ElementDestroy);
END_UNIT
     
UNIT(heap_extract_max)
    void* max;
    int result;
    int *item = malloc(sizeof(int));
    Vector *vector = GenerateVector();
    Heap* heap= Heap_Build(vector, Less);
    *item = 11;
    Heap_Insert(heap, item);
    result = Heap_ExtractMax(heap, &max);
    ASSERT_THAT(16 == *(int*)max);
    ASSERT_THAT(HEAP_SUCCESS == result);
    ASSERT_THAT(5 == Heap_ItemsNum(heap));
    Heap_Destroy(&heap);
    Vector_Destroy(&vector, ElementDestroy);
END_UNIT

UNIT(heap_extract_max_uninitialized)
    void* max;
    Vector *vector = GenerateVector();
    Heap* heap = Heap_Build(vector, Less);
    Heap_Destroy(&heap);
    ASSERT_THAT(HEAP_UNINITIALIZED_ERROR == Heap_ExtractMax(heap, &max));
    Vector_Destroy(&vector, ElementDestroy);
END_UNIT
 
UNIT(heap_items_num)
    int *item = malloc(sizeof(int));
    Vector *vector = GenerateVector();
    Heap* heap = Heap_Build(vector, Less);
    *item = 11;
    Heap_Insert(heap, item);
    ASSERT_THAT(6 == Heap_ItemsNum(heap));
    Heap_Destroy(&heap);
    Vector_Destroy(&vector, ElementDestroy);
END_UNIT


UNIT(heap_for_each)
    int count;
    Vector *vector = GenerateVector();
    Heap* heap = Heap_Build(vector, Less);
    count = Vector_ForEach(vector, PrintElement, "Index");
    ASSERT_THAT(5 == count);
    Heap_Destroy(&heap);
    Vector_Destroy(&vector, ElementDestroy);
END_UNIT

TEST_SUITE(heap)
    TEST(heap_build)
    TEST(heap_destroy)
    TEST(heap_insert)
    TEST(heap_insert_uninitialized)
    TEST(heap_max)
    TEST(heap_extract_max)
    TEST(heap_extract_max_uninitialized)
    TEST(heap_items_num)
    TEST(heap_for_each)
END_SUITE
