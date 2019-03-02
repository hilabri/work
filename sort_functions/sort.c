#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>         /* size_t */
#include "ADTDefs.h"        /* ADTErr */
#include "vector.h"
#include "sort.h"

void VectorBubbleSort(Vector *_vector)
{
	int i,j;
	int a1,a2;
	int len = VectorGetSize(_vector);
	assert(_vector);
	for(i = 0; i < len-1; ++i)
	{
		for(j = 1; j < len-i; ++j)
		{		
		    VectorGet(_vector, j, &a1);
		    VectorGet(_vector, j+1, &a2);
		    if( a1 > a2)
		    {
			    VectorSet(_vector,j,a2);
			    VectorSet(_vector,j+1,a1);
		    }
        }
	}
}

void VectorSelectionSort(Vector *_vector)
{
	int i,j;
	int arri,arrj;
	int minIndex;
    int minValue;
	int len = VectorGetSize(_vector);
	assert(_vector);
    for (i=1; i <= len; i++)
    {
        VectorGet(_vector, i, &arri);
        minIndex = i;
        minValue = arri;
        for (j = i+1; j <=len; j++)
        {
            VectorGet(_vector, j, &arrj);
            if (arrj < minValue)
            {
                minIndex = j;
                minValue = arrj;
            }
        }
        if (minIndex != i)
        {
            VectorSet(_vector, i, minValue);
            VectorSet(_vector, minIndex, arri);
        }
    }
}

void VectorShakeSort(Vector *_vector)
{
    int i,j;
    int a1,a2;
	int len = VectorGetSize(_vector);
	assert(_vector);
    for(i=0; i < len/2; i++){
    
        for(j=i+1; j< len-i; j++)
        {
            VectorGet(_vector, j, &a1);
            VectorGet(_vector, j+1, &a2);
            if (a1 > a2)
            {
            VectorSet(_vector, j, a2);
            VectorSet(_vector, j+1, a1);
            }
        }
        for(j= len-1-i; j > i-1; j --)
        {
            VectorGet(_vector, j, &a1);
            VectorGet(_vector, j-1, &a2);
            if (a1 < a2)
            {
            VectorSet(_vector, j, a2);
            VectorSet(_vector, j-1, a1);
            }
        }
    }
}

static void PartionSort(Vector *vector, int start, int end)
{
    int i = end;
    int j = start;
    int pivotIndex = start;
    int pivot;
    int arri, arrj;
    if ( end <= start ) return;
    VectorGet(vector, pivotIndex, &pivot);
    while ( i != j)
    {
        VectorGet(vector, i, &arri);
        VectorGet(vector, j, &arrj);
        while((pivot <= arri) && (i != j))
        {
            i--;
            VectorGet(vector, i, &arri);
        }
        if(pivot > arri)
        {
            VectorSet(vector,i,pivot);
            VectorSet(vector,pivotIndex,arri);
            pivotIndex = i;
        }
        while((pivot >= arrj) && (i != j))
        {
            j++;
            VectorGet(vector, j, &arrj);
        }
        if(pivot < arrj)
        {
            VectorSet(vector,j,pivot);
            VectorSet(vector,pivotIndex,arrj);
            pivotIndex = j;
        }
    } 
    PartionSort(vector, start, pivotIndex-1);
    PartionSort(vector, pivotIndex+1, end);
}
    
void VectorQuickSort(Vector *_vector)
{
	int len = VectorGetSize(_vector);
	assert(_vector);
    PartionSort(_vector,1,len);
}
    
static void Merge(Vector *vector, int start1, int end1, int start2, int end2, int  *tempArr)
{
    int i;
    int len = end2 - start1 +1;
    int i1 = start1;
    int i2 = start2;
    int arri1,arri2;
    for(i = 0; i < len; i++)
    {
        if(i1 > end1)
        {
            VectorGet(vector, i2, &arri2);
            tempArr[i] =  arri2;
            i2++;
            continue;
        }
        if(i2 > end2)
        {
            VectorGet(vector, i1, &arri1);
            tempArr[i] = arri1;
            i1++;
            continue;
        } 
        VectorGet(vector, i1, &arri1);
        VectorGet(vector, i2, &arri2);
        if( arri1 < arri2)
        {
            tempArr[i] = arri1;
            i1++;
        } else {
            tempArr[i] = arri2;
            i2++;
        }
    }

    /* Copy output array to vector */
    for (i = 0; i < len; i++)
    {
        VectorSet(vector, start1+i, tempArr[i]);        
    }
}

static void MergeSort(Vector *vector, int start, int end, int *tempArr)
{
    int diff;    
    if(start >= end ) return;
    diff = end - start;
    MergeSort(vector, start, start+diff/2, tempArr);
    MergeSort(vector, start+diff/2+1, end, tempArr);
    Merge(vector, start, start + diff/2, start + diff/2+1, end, tempArr);
}
    
void VectorMergeSort(Vector *_vector)
{
    int  *tempArr;
	int len = VectorGetSize(_vector);
    assert(_vector);
    tempArr = malloc(sizeof(int)*len);
    assert(tempArr);
    MergeSort(_vector, 1, len, tempArr);
}

void VectorCountingSort(Vector *_vector, int _min, int _max)
{ 
    int i, j, value, range, len, vectorIndex;
    int *countArr;
    assert(_vector);
    
    range = _max - _min + 1;   
    len = VectorGetSize(_vector);
    countArr = calloc(range,sizeof(int));
    assert(countArr);
    for(i = 1; i <= len; i++)
    {
        VectorGet(_vector, i, &value);
        countArr[value - _min]++;
    } 
    vectorIndex = 1;
    for(i = 0; i < range; i++)
    {
        for(j = 0; j < countArr[i]; j++)
        {
            VectorSet(_vector, vectorIndex, (i+_min));
            vectorIndex++;        
        }
    }
    free(countArr);
}

void RadixSort(Vector *vector, int div, int tempArray[], int len)
{
    int i;
    int value;
    int digitCounter[10] = { 0 };
    /* Counter array: */
    for (i = 1; i <= len; i++)
    {
        VectorGet(vector, i, &value);
        digitCounter[(value/div)%10]++;
    }
    /* Count to sum: */
    for (i = 1; i <= 9; i++)
    {
        digitCounter[i] += digitCounter[i-1];
    }
    /* Prepare temp array: */
    for (i = len; i > 0; i--)
    {
        VectorGet(vector, i, &value);
        tempArray[digitCounter[(value/div)%10] - 1] = value;
        digitCounter[(value/div)%10]--;
    }
    /* Update vector: */
    for (i = 0; i < len; i++)
    {
        VectorSet(vector, i+1, tempArray[i]);
    }
}

void VectorRadixSort(Vector *_vector, int _max)
{
    int *tempArray;
    int len;
    int div;

    len = VectorGetSize(_vector);
    tempArray = malloc(len * sizeof(int));

    for (div = 1; _max/div > 0; div*=10)
    {
        RadixSort(_vector, div, tempArray, len);
    }

    free(tempArray);
}

