#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdlib.h>

typedef struct Allocator Allocator;


/** 
* @brief Create a memory allocator 
* @param[in] _blockSize - single allocation size in bytes 
* @param[in] _nBlocks -   total number of blocks
* @return the initialized allocator
*/
Allocator *Allocator_Create(size_t _blockSize, size_t _nBlocks);

/** 
* @brief Destroy an allocator
* @param[in] _allocator -  the allocator to be deallocated
*/
void Allocator_Destroy(Allocator *_allocator);

/** 
* @brief allocate one block 
* @param[in] _allocator - the allocator to use 
* @return the address for the allocated block
*/
void *Allocator_BlockAllocate(Allocator* _allocator); /* O(1) */

/** 
* @brief free previously allocated block  
* @param[in] _allocator - the allocator to use
* @param[in] __blockAddress - address block to free
*/
void Allocator_BlockFree(Allocator* _allocator, void* _blockAddress); /* O(1) */

#endif
