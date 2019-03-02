#ifndef MALLOC_H
#define MALLOC_H

#include <stdlib.h>

typedef struct Allocator Allocator;


/** 
* @brief Create a memory allocator 
* @param[in] _blockSize - single allocation size in bytes 
* @param[in] _nBlocks -   total number of blocks
* @return the initialized allocator
*/
Allocator *Allocator_Create(size_t _memorySize);

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
void *Allocator_BlockAllocate(Allocator* _allocator, size_t _size); 

/** 
* @brief free previously allocated block  
* @param[in] _allocator - the allocator to use
* @param[in] __blockAddress - address block to free
*/
void Allocator_BlockFree(Allocator* _allocator, void* _blockAddress); 

#endif
