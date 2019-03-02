#include <stdio.h>
#include <stdlib.h>
#include "sort_test.h"
#include "sort.h"
#include "vector.h"

static Vector* GenerateVector(void)
{
    Vector *vector = VectorCreate(20, 5);
    int array[15] = {4,1,4,2,5,10,4,8,9,3,7,9,10,2,6};
    int i;
    for (i=0; i < 15; i++)
    {
        VectorAdd(vector, array[i]);
    }
    return vector;
}
  
UNIT(bubble_sort)
    int min, max;
    Vector *vector = GenerateVector();    
    VectorBubbleSort(vector);
	VectorPrint(vector);
    VectorGet(vector, 1, &min);
	ASSERT_THAT(1 == min);
    VectorGet(vector,VectorGetSize(vector),&max);
	ASSERT_THAT(10 == max);
	ASSERT_THAT(vector);
    VectorDestroy(vector);
END_UNIT


UNIT(selection_sort)
    int min, max;
    Vector *vector = GenerateVector();    
    VectorSelectionSort(vector);
	VectorPrint(vector);
    VectorGet(vector, 1, &min);
	ASSERT_THAT(1 == min);
    VectorGet(vector,VectorGetSize(vector),&max);
	ASSERT_THAT(10 == max);
	ASSERT_THAT(vector);
    VectorDestroy(vector);
END_UNIT


UNIT(shake_sort)
    int min, max;
    Vector *vector = GenerateVector();    
    VectorShakeSort(vector);
    VectorGet(vector, 1, &min);
	ASSERT_THAT(1 == min);
    VectorGet(vector,VectorGetSize(vector),&max);
	ASSERT_THAT(10 == max);
	ASSERT_THAT(vector);
	VectorPrint(vector);
    VectorDestroy(vector);
END_UNIT

UNIT(quick_sort)
    int min, max;
    Vector *vector = GenerateVector();    
    VectorQuickSort(vector);
    VectorGet(vector, 1, &min);
	ASSERT_THAT(1 == min);
    VectorGet(vector,VectorGetSize(vector),&max);
	ASSERT_THAT(10 == max);
	ASSERT_THAT(vector);
	VectorPrint(vector);
    VectorDestroy(vector);
END_UNIT

UNIT(merge_sort)
    int min, max;
    Vector *vector = GenerateVector();    
    VectorMergeSort(vector);
	VectorPrint(vector);
    VectorGet(vector, 1, &min);
	ASSERT_THAT(1 == min);
    VectorGet(vector,VectorGetSize(vector),&max);
	ASSERT_THAT(10 == max);
	ASSERT_THAT(vector);
    VectorDestroy(vector);
END_UNIT

UNIT(counting_sort)
    int min, max;
    Vector *vector = GenerateVector();    
    VectorCountingSort(vector, 1, 10);
    VectorGet(vector, 1, &min);
	ASSERT_THAT(1 == min);
    VectorGet(vector,VectorGetSize(vector),&max);
	ASSERT_THAT(10 == max);
	ASSERT_THAT(vector);
	VectorPrint(vector);
    VectorDestroy(vector);
END_UNIT

UNIT(radix_sort)
    int min, max;
    Vector *vector = GenerateVector();    
    VectorRadixSort(vector, 10);
	VectorPrint(vector);
    VectorGet(vector, 1, &min);
	ASSERT_THAT(1 == min);
    VectorGet(vector,VectorGetSize(vector),&max);
	ASSERT_THAT(10 == max);
	ASSERT_THAT(vector);
    VectorDestroy(vector);
END_UNIT

TEST_SUITE(sort_algorithms)
    TEST(bubble_sort)
    TEST(selection_sort)
    TEST(shake_sort)
    TEST(quick_sort)
    TEST(merge_sort)
    TEST(counting_sort)
    TEST(radix_sort)
END_SUITE






