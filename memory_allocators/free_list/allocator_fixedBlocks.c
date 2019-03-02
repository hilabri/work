#include "allocator.h"

#define MAGIC        0XDEADBEEF
#define DEAD_MAGIC   0xDEADDEAD
#define IS_VALID(A)  ((A) && (MAGIC == (A)->m_magic))

#define FREE_MAGIC   0xF7EEF7EE
#define ALIGN_SIZE   (sizeof(FreeList))

#define IS_VALID_ADDRESS(ALLOCATOR,ADDRESS) \
 ((ADDRESS) >= (ALLOCATOR)->m_blocks  && \
 ((ADDRESS) <= (void*)((char*)(ALLOCATOR)->m_blocks+(ALLOCATOR)->m_size)) && \
 ((FreeList*)ADDRESS)->m_magic != FREE_MAGIC)

typedef struct FreeList FreeList;

struct Allocator 
{
    void* m_blocks;
    FreeList *m_head;
    size_t m_size;
    size_t m_magic;
};

struct FreeList
{
    FreeList *m_next;
    size_t m_magic; /* check if address is already free? */
};

static void FreeListPush(Allocator *_allocator, FreeList *_address)
{
    _address->m_magic = FREE_MAGIC;
    _address->m_next = _allocator->m_head;
    _allocator->m_head = _address;
}

static FreeList *FreeListPop(Allocator *_allocator)
{
    FreeList *address = _allocator->m_head;
    if (!address)
    {
        return NULL;
    }
    address->m_magic = DEAD_MAGIC;
    _allocator->m_head = _allocator->m_head->m_next;
    return address;
}

Allocator *Allocator_Create(size_t _blockSize, size_t _nBlocks)
{
    int i;
    char *address;
    int alignedBlockSize = _blockSize;
    Allocator *allocator = malloc(sizeof(Allocator));

    if(!allocator)
    {
        return NULL;
    }
    if (0 < _blockSize % ALIGN_SIZE)
    {
        alignedBlockSize = ALIGN_SIZE * (_blockSize / ALIGN_SIZE) + ALIGN_SIZE;    
    }
    allocator->m_blocks = malloc(alignedBlockSize*_nBlocks);
    if(!allocator->m_blocks)
    {
        free(allocator);
        return NULL;
    }
    allocator->m_head = NULL;
    for(i = 0; i < _nBlocks; i++)
    {
        address = (char *)allocator->m_blocks + i * alignedBlockSize;
        FreeListPush(allocator, (FreeList *)address);    
    }
    allocator->m_size = _nBlocks * alignedBlockSize;
    allocator->m_magic = MAGIC;
    return allocator;
}

void Allocator_Destroy(Allocator* _allocator)
{
    if(!IS_VALID(_allocator))
    {
        return;
    }
    _allocator->m_magic = DEAD_MAGIC;
    free(_allocator->m_blocks);
    free(_allocator);
}

void *Allocator_BlockAllocate(Allocator* _allocator)
{
    if(!IS_VALID(_allocator))
    {
        return NULL;
    }
    return (FreeListPop(_allocator));
}

void Allocator_BlockFree(Allocator* _allocator, void* _blockAddress)
{
    if(!IS_VALID(_allocator) || !IS_VALID_ADDRESS(_allocator, _blockAddress))
    {
        return;
    }
    FreeListPush(_allocator, _blockAddress);
}
