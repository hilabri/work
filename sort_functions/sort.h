#ifndef __SORT__
#define __SORT__

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>         /* size_t */
#include "ADTDefs.h"        /* ADTErr */
#include "vector.h"
#include "sort.h"

void VectorBubbleSort(Vector *_vector);
void VectorSelectionSort(Vector *_vector);
void VectorShakeSort(Vector *_vector);
void VectorQuickSort(Vector *_vector);
void VectorMergeSort(Vector *_vector);
void VectorCountingSort(Vector *_vector, int _min, int _max);
void VectorRadixSort(Vector *_vector, int _max);
#endif
