#include <stdio.h>
#include <stdlib.h>
#include "allocator_test.h"
#include "allocator.h"

UNIT(allocator_create)
    Allocator* allocator = Allocator_Create(2, 4);
    ASSERT_THAT(allocator);
    Allocator_Destroy(allocator);
END_UNIT

UNIT(allocator_destroy)
    Allocator* allocator = Allocator_Create(2, 4);
    ASSERT_THAT(Allocator_BlockAllocate(allocator));    
    Allocator_Destroy(allocator);
    ASSERT_THAT(!Allocator_BlockAllocate(allocator));    
END_UNIT

UNIT(allocator_block_allocate)
    int i;
    int *block[5];
    Allocator *allocator = Allocator_Create(sizeof(int), 5);
    for (i = 0; i < 5; i++)
    {
        block[i] = Allocator_BlockAllocate(allocator);
        ASSERT_THAT(block[i]);
        *block[i] = i;
        
    }
    for (i = 0; i < 5; i++)
    {
        ASSERT_THAT(*block[i] == i);
    }
    Allocator_Destroy(allocator);
END_UNIT

UNIT(allocator_block_allocate_overflow)
    int i;
    Allocator *allocator = Allocator_Create(sizeof(int), 3);
    for (i = 0; i < 3; i++)
    {
        ASSERT_THAT(Allocator_BlockAllocate(allocator));
    }
    ASSERT_THAT(!Allocator_BlockAllocate(allocator));
    Allocator_Destroy(allocator);
    allocator = Allocator_Create(sizeof(int), 0);
    ASSERT_THAT(!Allocator_BlockAllocate(allocator));
    Allocator_Destroy(allocator);
END_UNIT

UNIT(allocator_block_free)
    int i, j;
    int *block[5];
    Allocator *allocator = Allocator_Create(sizeof(int), 5);
    for (i = 0; i < 5; i++)
    {
        block[i] = Allocator_BlockAllocate(allocator);
        ASSERT_THAT(block[i]);
        *block[i] = i;
    }
    for (i = 0; i < 5; i++)
    {
        Allocator_BlockFree(allocator, block[i]);
        block[i] = Allocator_BlockAllocate(allocator);
        ASSERT_THAT(block[i]);
        *block[i] = 100;
        for(j =i+1; j < 5; j++)
        {
            ASSERT_THAT(*block[j] == j);
        }
    }
    for (i = 0; i < 5; i++)
    {
        ASSERT_THAT(*block[i] == 100);
    }
    Allocator_Destroy(allocator);
END_UNIT

UNIT(allocator_block_free_invalid)
    int myint = 70;
    Allocator *allocator = Allocator_Create(sizeof(int), 1);
    ASSERT_THAT(Allocator_BlockAllocate(allocator));
    Allocator_BlockFree(allocator, &myint);
    ASSERT_THAT(!Allocator_BlockAllocate(allocator));
    Allocator_Destroy(allocator);
END_UNIT

UNIT(allocator_uninitialized)
    Allocator *allocator = NULL;
    ASSERT_THAT(!Allocator_BlockAllocate(allocator));    
END_UNIT    

TEST_SUITE(allocator container)
    TEST(allocator_create)
    TEST(allocator_destroy)
    TEST(allocator_block_allocate)
    TEST(allocator_block_allocate_overflow)
    TEST(allocator_block_free)
    TEST(allocator_block_free_invalid)
    TEST(allocator_uninitialized)
END_SUITE

