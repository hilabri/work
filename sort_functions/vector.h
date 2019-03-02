#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h>         /* size_t */

#include "ADTDefs.h"        /* ADTErr */

typedef struct Vector Vector;

/* initialSize  - The initial allocation size.
   extendSize   - The extention block size.
   *     */
Vector* VectorCreate  (size_t _initialSize, size_t _extendSize);
void    VectorDestroy (Vector *_vector);

/* Add item to end. */
ADTErr  VectorAdd     (Vector *_vector,  int  _item);
/* Delete item from the end. */
ADTErr  VectorDelete  (Vector *_vector,  int* _item);

ADTErr  VectorGet     (Vector *_vector, size_t _index, int *_item);  /* Index from 1 */
ADTErr  VectorSet     (Vector *_vector, size_t _index, int  _item);  /* Index from 1 */

int     VectorGetSize (Vector *_vector);
int     IsVectorEmpty (Vector *_vector);

/*  Unit-Test functions  */
void    VectorPrint   (Vector *_vector);

#endif /*__VECTOR_H__*/
