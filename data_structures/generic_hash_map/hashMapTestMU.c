#include <stdio.h>
#include <stdlib.h>
#include "mu_test.h"
#include "HashMap.h"

static size_t HashFunc(const void* _key)
{
    return (size_t)_key;
}

static int EqualityFunc(const void* _firstKey, const void* _secondKey)
{
    return (*(int*)_firstKey == *(int*)_secondKey);
}

    
static void KeyDestroy(void* _key)
{   
    free((int*)_key);
}

static void ValDestroy(void* _value)
{   
    free((int*)_value);
}

static int HashMapPrintElement(const void* _key, void* _value, void* _context)
{   
    printf("|%s %d: %d|\n",(char *)_context, *(int *)_key, *(int *)_value);
    return 1;
}   

static void SetupIntItemsArray(int *arr_keys[], int *arr_values[], int _size)
{   
    int i;
    int *item;
    for (i = 0; i < _size; i++)
    {
        item = malloc(sizeof(int));
        *item = i + 10;
        arr_keys[i] = item;
        item = malloc(sizeof(int));
        *item = i + 100;
        arr_values[i] = item;  
    }
}

static void InsertIntItemsArray(HashMap *_map, int *_keys_arr[], int *_values_arr[], int _size)
{
    int i;
    for (i = 0; i < _size; i++)
    {
        HashMap_Insert(_map, _keys_arr[i], _values_arr[i]); /* (map, key, value) */ 
    }
}

UNIT(hashMap_create)
    HashMap* map = HashMap_Create(4, HashFunc, EqualityFunc);
    ASSERT_THAT(map);
    HashMap_Destroy(&map, KeyDestroy, ValDestroy);
    map = HashMap_Create(0, HashFunc, EqualityFunc);
    ASSERT_THAT(!map);
END_UNIT

UNIT(hashMap_destroy)
    int *keys_array[5];
    int *values_array[5];
    HashMap *map;
    int value = 10;
    int key = value + 100;
    SetupIntItemsArray(keys_array, values_array, 5);
    map = HashMap_Create(4, HashFunc, EqualityFunc);
    HashMap_Destroy(NULL, KeyDestroy, ValDestroy);
    HashMap_Destroy(&map, KeyDestroy, ValDestroy);
    InsertIntItemsArray(map, keys_array, values_array, 5);
    HashMap_Destroy(&map, KeyDestroy, ValDestroy);
    ASSERT_THAT(MAP_UNINITIALIZED_ERROR == HashMap_Insert(map, &key ,&value));  
    ASSERT_THAT(NULL == map);
END_UNIT

UNIT(hashMap_destroy_uninitialized_destroy_functions)
    int key = 1;
    int value = 2;
    HashMap *map = HashMap_Create(4, HashFunc, EqualityFunc);
    ASSERT_THAT(MAP_SUCCESS == HashMap_Insert(map, &key ,&value));  
    HashMap_Destroy(&map, NULL, NULL);
    ASSERT_THAT(NULL == map);
END_UNIT

UNIT(hashMap_insert)
    int *keys_array[10];
    int *values_array[10];
    int i;
    HashMap* map = HashMap_Create(4, HashFunc, EqualityFunc);
    SetupIntItemsArray(keys_array, values_array, 10);
    for (i=0; i<10; i++)
    {
        ASSERT_THAT(MAP_SUCCESS == HashMap_Insert(map, keys_array[i], values_array[i]));
    }
    ASSERT_THAT(10 == HashMap_Size(map));
    HashMap_Destroy(&map, KeyDestroy, ValDestroy);
END_UNIT


UNIT(hashMap_insert_uninitialized)
    HashMap *map;
    int value = 10;
    int key = value + 100;
    map = HashMap_Create(4, HashFunc, EqualityFunc);
    HashMap_Destroy(&map, KeyDestroy, ValDestroy);    
    ASSERT_THAT(MAP_UNINITIALIZED_ERROR == HashMap_Insert(map, &key, &value));
    map = HashMap_Create(0, HashFunc, EqualityFunc);
    ASSERT_THAT(MAP_UNINITIALIZED_ERROR == HashMap_Insert(map, &key, &value));
    HashMap_Destroy(&map, KeyDestroy, ValDestroy);    
END_UNIT


UNIT(hashMap_insert_key_duplicate)
    int i;
    int *keys_array[10];
    int *values_array[10];
    HashMap* map = HashMap_Create(4, HashFunc, EqualityFunc);
    SetupIntItemsArray(keys_array, values_array, 10);
    for (i=0; i<9; i++)
    {
        ASSERT_THAT(MAP_SUCCESS == HashMap_Insert(map, keys_array[i], values_array[i]));
        ASSERT_THAT(MAP_KEY_DUPLICATE_ERROR == HashMap_Insert(map, keys_array[i], values_array[i]));
    }
    ASSERT_THAT(MAP_SUCCESS == HashMap_Insert(map, keys_array[i], values_array[i]));
    HashMap_Destroy(&map, KeyDestroy, ValDestroy);    
END_UNIT

UNIT(hashMap_remove)
    int i;
    int *removed_key; 
    int *removed_value;
    int *keys_array[5];
    int *values_array[5];
    HashMap *map = HashMap_Create(4, HashFunc, EqualityFunc);
    SetupIntItemsArray(keys_array, values_array, 5);
    InsertIntItemsArray(map, keys_array, values_array, 5);
    for (i=4; i>=0; i--)
    {
        ASSERT_THAT(MAP_SUCCESS == HashMap_Remove(map, keys_array[i], (void **)&removed_key, (void **)&removed_value));
        ASSERT_THAT(*keys_array[i] == *removed_key);
        ASSERT_THAT(*values_array[i] == *removed_value);
    }
    ASSERT_THAT(0 == HashMap_Size(map));
    HashMap_Destroy(&map, KeyDestroy, ValDestroy);    
END_UNIT

UNIT(hashMap_remove_key_not_found)
    int *removed_key;
    int *removed_value;
    int key = 10;
    HashMap *map = HashMap_Create(4, HashFunc, EqualityFunc);
    ASSERT_THAT(MAP_KEY_NOT_FOUND_ERROR == HashMap_Remove(map, &key, (void **)&removed_key, (void **)&removed_value));
    HashMap_Destroy(&map, KeyDestroy, ValDestroy);
END_UNIT

UNIT(hashMap_remove_uninitialized)
    int *removed_key;
    int *removed_value;
    int key = 10;
    HashMap *map = HashMap_Create(4, HashFunc, EqualityFunc);
    HashMap_Destroy(&map, NULL, NULL);
    ASSERT_THAT(MAP_UNINITIALIZED_ERROR == HashMap_Remove(map, &key, (void**)&removed_key, (void **)&removed_value));
END_UNIT

UNIT(hashMap_find)
    int *keys_array[5];
    int *values_array[5];
    int *removed_key;
    int *removed_value;
    int *value;
    int i;
    HashMap *map;
    SetupIntItemsArray(keys_array, values_array, 5);
    map = HashMap_Create(4, HashFunc, EqualityFunc);
    InsertIntItemsArray(map, keys_array, values_array, 5);
    for (i=0; i<5; i++)
    {
        ASSERT_THAT(MAP_SUCCESS == HashMap_Find(map, keys_array[i], (void **)&value));
        ASSERT_THAT(*values_array[i] == *value);
        ASSERT_THAT(MAP_SUCCESS == HashMap_Remove(map, keys_array[i], (void **)&removed_key, (void **)&removed_value));
        ASSERT_THAT(MAP_KEY_NOT_FOUND_ERROR == HashMap_Find(map, keys_array[i], (void **)&value));
    }
    HashMap_Destroy(&map, KeyDestroy, ValDestroy);
END_UNIT

UNIT(hashMap_find_uninitialized)
    int key = 5;
    int value = 10;
    int *removed_value;
    HashMap* map = HashMap_Create(4, HashFunc, EqualityFunc);
    ASSERT_THAT(MAP_SUCCESS == HashMap_Insert(map, (void *)&key, (void *)&value));
    HashMap_Destroy(&map, NULL, NULL);
    ASSERT_THAT(MAP_UNINITIALIZED_ERROR == HashMap_Find(map, (void *)&key, (void**)&removed_value));
END_UNIT

UNIT(hashMap_find_key_not_found)
    int *find_value;
    int *removed_key;
    HashMap *map;
    int search_key = 10;
    int search_value = 100;
    
    map = HashMap_Create(4, HashFunc, EqualityFunc);
    ASSERT_THAT(MAP_KEY_NOT_FOUND_ERROR == HashMap_Find(map, &search_key, (void **)&find_value));
    
    ASSERT_THAT(MAP_SUCCESS == HashMap_Insert(map, (void *)&search_key, (void *)&search_value));
    ASSERT_THAT(MAP_SUCCESS == HashMap_Find(map, (void *)&search_key, (void **)&find_value));
    ASSERT_THAT(MAP_SUCCESS == HashMap_Remove(map, (void *)&search_key, (void **)&removed_key, (void **)&find_value));
    ASSERT_THAT(MAP_KEY_NOT_FOUND_ERROR == HashMap_Find(map, (void *)&search_key, (void **)&find_value));
    HashMap_Destroy(&map, NULL, NULL);
END_UNIT


UNIT(hashMap_size)
    int *keys_array[10];
    int *values_array[10];
    void *removed_key;
    void *removed_value;
    HashMap* map = HashMap_Create(4, HashFunc, EqualityFunc);
    ASSERT_THAT(0 == HashMap_Size(map));
    SetupIntItemsArray(keys_array, values_array, 10);
    InsertIntItemsArray(map, keys_array, values_array, 10);
    ASSERT_THAT(10 == HashMap_Size(map));
    ASSERT_THAT(MAP_SUCCESS == HashMap_Remove(map, keys_array[0], &removed_key, &removed_value));
    ASSERT_THAT(9 == HashMap_Size(map));
    HashMap_Destroy(&map, KeyDestroy, ValDestroy);
    ASSERT_THAT(0 == HashMap_Size(map));
END_UNIT

UNIT(hashMap_for_each)
    int *keys_array[10];
    int *values_array[10];
    int count;
    HashMap* map = HashMap_Create(4, HashFunc, EqualityFunc);
    SetupIntItemsArray(keys_array, values_array, 10);
    InsertIntItemsArray(map, keys_array, values_array, 10);
    count = HashMap_ForEach(map, HashMapPrintElement, "Key ");
    ASSERT_THAT(10 == count);
    HashMap_Destroy(&map, KeyDestroy, ValDestroy);    
END_UNIT

UNIT(hashMap_rehash)
    int *keys_array[5];
    int *values_array[5];
    int i;
    int *value;
    HashMap *map;
    SetupIntItemsArray(keys_array, values_array, 5);
    map = HashMap_Create(4, HashFunc, EqualityFunc);
    InsertIntItemsArray(map, keys_array, values_array, 5);
    ASSERT_THAT(MAP_SUCCESS == HashMap_Rehash(map, 10));
    ASSERT_THAT(5 == HashMap_Size(map));
    for (i=0; i<5; i++)
    {
        ASSERT_THAT(MAP_SUCCESS == HashMap_Find(map, keys_array[i], (void **)&value));
        ASSERT_THAT(*values_array[i] == *value);
    }
    HashMap_Destroy(&map, KeyDestroy, ValDestroy);
END_UNIT

TEST_SUITE(hashMap container)
    TEST(hashMap_create)
    TEST(hashMap_destroy)
    TEST(hashMap_destroy_uninitialized_destroy_functions)
    
    TEST(hashMap_insert)
    TEST(hashMap_insert_uninitialized)
    TEST(hashMap_insert_key_duplicate)

    TEST(hashMap_remove)
    TEST(hashMap_remove_key_not_found)
    TEST(hashMap_remove_uninitialized)

    TEST(hashMap_find)
    TEST(hashMap_find_uninitialized)
    TEST(hashMap_find_key_not_found)

    TEST(hashMap_rehash)

    TEST(hashMap_size)
    TEST(hashMap_for_each)
END_SUITE
