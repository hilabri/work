#include "malloc.h"

#define MAGIC        0XDEADBEEF
#define DEAD_MAGIC   0xDEADDEAD
#define IS_VALID(A)  ((A) && (MAGIC == (A)->m_magic))

#define TRUE 1
#define FALSE 0

#define IS_ADDRESS_VALID(ALLOCATOR, ADDRESS) \
        (ADDRESS <= (Metadata*)(((char*)(ALLOCATOR)->m_blocks)+(ALLOCATOR)->m_size)   &&   ADDRESS >= (ALLOCATOR)->m_blocks)  

#define NEXT_BLOCK(ADDRESS)  ((char*)(ADDRESS) + ((Metadata*)(ADDRESS))->blockSize + sizeof(Metadata)) 

#define ALIGN_SIZE   (sizeof(Metadata))

/*TODO check valid pointer _blockAddress
 *     check if user asking more than first allocated
 *     Add alignment size per metadata
 */ 
 
struct Allocator
{
    void* m_blocks;
    size_t m_size;
    size_t m_magic;
};

typedef struct Metadata Metadata;

struct Metadata
{
    size_t blockSize : (sizeof(size_t)) - 2; 
    size_t isFree : 1;
    size_t isLast : 1;
};

static void *CreateBlock(void *_address, size_t _size, int _isFree)
{
    ((Metadata*)_address)->blockSize = _size;
    ((Metadata*)_address)->isFree = _isFree;
    return (char*)_address + sizeof(Metadata);    
}

static void MergeFreeBlocks(Allocator *_allocator, Metadata *_address)
{
    Metadata* nextAdd = NEXT_BLOCK(_address);
    while(nextAdd->isFree && IS_ADDRESS_VALID(_allocator, nextAdd))
    {
        _address->blockSize += (sizeof(Metadata) + nextAdd->blockSize);
        nextAdd = NEXT_BLOCK(nextAdd);
    }
}

Allocator *Allocator_Create(size_t _size)
{
    Allocator *allocator = malloc(sizeof(Allocator));
    if (!allocator)
    {
        return NULL;
    }
    allocator->m_size = _size + sizeof(Metadata);
    allocator->m_blocks = malloc(allocator->m_size);
    if (!allocator->m_blocks)
    {
        return NULL;
    }
    CreateBlock(allocator->m_blocks, _size, TRUE);     
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
    
static Metadata *FindNextFreeAdreess(Allocator *_allocator, size_t _size)
{
    Metadata *address = _allocator->m_blocks;
    while(IS_ADDRESS_VALID(_allocator, address))
    {
            
            if(address->isFree && _size <= address->blockSize)
            {
                return address;
            }
            address = NEXT_BLOCK(address);
    }
    return NULL;
}

void *Allocator_BlockAllocate(Allocator* _allocator, size_t _size)
{
    void *block, *address;
    int sizeDiff;
    if(!IS_VALID(_allocator))
    {
        return NULL;
    }
    address = (void*)FindNextFreeAdreess(_allocator, _size);
    if(!address)
    {
        return NULL;
    }
    sizeDiff  = ((Metadata*)address)->blockSize - _size - sizeof(Metadata);
    if(0 < sizeDiff)
    {
        void *spiltBlock = CreateBlock((char*)address + _size + sizeof(Metadata), sizeDiff- sizeof(Metadata), TRUE);
        MergeFreeBlocks(_allocator, (Metadata*)spiltBlock);
    }
    block = CreateBlock(address, _size, FALSE);
    return block; 
}

void Allocator_BlockFree(Allocator* _allocator, void* _blockAddress)
{
    void* dataAddress;
    if(!IS_VALID(_allocator))
    {
        return;
    }
    dataAddress = (char*)_blockAddress - sizeof(Metadata);
    ((Metadata*)dataAddress)->isFree = TRUE;    
    MergeFreeBlocks(_allocator, (Metadata*)dataAddress); 
}

    


