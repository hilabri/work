#include <stdio.h>
#include <stdlib.h>
#include "HashSet.h"
#include "hash_test.h"

static size_t HashFunction(size_t data)
{
    return data;
}

UNIT(hashset_create)
    int item = 534;
    HashSet* set = HashSetCreate(100, 0.8, HashFunction); 
    ASSERT_THAT(set); 
    ASSERT_THAT(ERR_OK == HashSetInsert(set, item)); 
    HashSetDestroy(set); 
END_UNIT 
    
UNIT(hashset_destroy) 
    int item = 432;
    HashSet* set = HashSetCreate(100, 0.8, HashFunction); 
    HashSetDestroy(set); 
    set = NULL; 
    HashSetDestroy(set); 
    ASSERT_THAT(ERR_NOT_INITIALIZED == HashSetInsert(set, item)); 
END_UNIT

UNIT(hashset_insert)
    int i;
    HashSet* set = HashSetCreate(20, 0.5, HashFunction);
     for (i=0; i<5; i++)
     {
         HashSetInsert(set, i);
     }
     ASSERT_THAT(ERR_OK == HashSetInsert(set, i));
     HashSetDestroy(set);
END_UNIT

UNIT(hashset_insert_uninitialized)
    int item = 1;
    HashSet* set = HashSetCreate(10, 0.4, HashFunction);
    HashSetDestroy(set);
    ASSERT_THAT(ERR_NOT_INITIALIZED == HashSetInsert(set, item)); 
END_UNIT

/*
UNIT(hashset_remove)
   HashSetRemove
END_UNIT
     
UNIT(hashset_contains)
    HashSetContains
END_UNIT
 
UNIT(hashset_size)
	HashSetSize
END_UNIT

UNIT(hashset_statistics)
   HashSetStatistics
END_UNIT

UNIT(hashset_for_each)
   HashSetForEach
END_UNIT

UNIT(hashset_print)
   
END_UNIT
*/

TEST_SUITE(hash_set)	
    TEST(hashset_create)
    TEST(hashset_destroy)
    TEST(hashset_insert)
    TEST(hashset_insert_uninitialized)
/*
    TEST(hashset_remove)
    TEST(hashset_size)
    TEST(hashset_contains)
    TEST(hashset_statistics)
    TEST(hashset_for_each)
    TEST(heap_print)
    */
END_SUITE

