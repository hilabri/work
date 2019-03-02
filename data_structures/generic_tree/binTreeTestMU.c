#include <stdio.h>
#include <stdlib.h>
#include "bin_tree.h"
#include "mu_test.h"

static int LessComparatorFunc(void* _left, void* _right)
{
    return (*(int*)_left < *(int*)_right);
}

static int PredicateFunc(void * _element, void* _context)
{
    return ( *(int*)_element < *(int*)_context);  
}

static int PrintNode(void* _element, void* _context)
{
    printf("|%s: %d|\n", (char *)_context, *(int *)_element);
    return 1;
}   
    
static void SetupIntItemsArray(int *arr[], int _size)
{   
    int i;
    int *item;
    for (i = 0; i < _size; i++)
    {
        item = malloc(sizeof(int));
        *item = i ;
        arr[i] = item;
    }
}

static void AppendIntItemsArray(BSTree* _tree, int *arr[], int _size)
{   
    int i;
    for (i = 0; i < _size; i++)
    {
        BSTree_Insert(_tree, arr[i]);
    }
}   

static void DestroyerFunc(void* _value)
{
    free((int*)_value);
}

UNIT(tree_create)
    BSTreeItr itr;
    BSTree* tree = BSTree_Create(LessComparatorFunc);
    int item = 5; 
    ASSERT_THAT(tree);
    itr = BSTree_Insert(tree, &item);
    ASSERT_THAT(item == *(int*)(BSTreeItr_Get(itr)));
    BSTree_Destroy(&tree, NULL);
END_UNIT


UNIT(tree_destroy)
    int *arr[10];
    int item = 1;
    BSTree* tree = BSTree_Create(LessComparatorFunc);
    BSTree_Destroy(&tree,NULL);
    BSTree_Destroy(&tree,NULL);
    ASSERT_THAT(NULL == BSTree_Insert(tree, &item));
    SetupIntItemsArray(arr, 10);
    AppendIntItemsArray(tree, arr, 10);
    BSTree_Destroy(&tree,DestroyerFunc);
    ASSERT_THAT(NULL == BSTree_Insert(tree, &item));
END_UNIT


UNIT(tree_insert)
    BSTreeItr itr;
    int i;
    int *arr[10];
    BSTree* tree = BSTree_Create(LessComparatorFunc);
    SetupIntItemsArray(arr, 10);
    for (i=0; i < 10; i++)
    {
        itr = BSTree_Insert(tree, arr[i]);
        ASSERT_THAT(*arr[i] == *(int*)(BSTreeItr_Get(itr)));
    }
    BSTree_Destroy(&tree, DestroyerFunc);
END_UNIT


UNIT(tree_insert_uninitialized)
    int item = 1;
    BSTree* tree = BSTree_Create(LessComparatorFunc);
    BSTree_Destroy(&tree, DestroyerFunc);
    ASSERT_THAT(NULL == BSTree_Insert(tree, &item));
END_UNIT


UNIT(tree_insert_alredy_exist)
    BSTreeItr itr;
    int i;
    int *arr[10];
    BSTree* tree = BSTree_Create(LessComparatorFunc);
    SetupIntItemsArray(arr, 10);
    for (i=0; i < 10; i++)
    {
        itr = BSTree_Insert(tree, arr[i]);
        ASSERT_THAT(BSTreeItr_Equals(BSTreeItr_End(tree), BSTree_Insert(tree, arr[i])));
    }
    BSTree_Destroy(&tree, DestroyerFunc);
END_UNIT


UNIT(tree_find)
    BSTreeItr itr;
    int *arr[10];
    int context = 5;
    BSTree* tree = BSTree_Create(LessComparatorFunc);
    SetupIntItemsArray(arr, 10);
    AppendIntItemsArray(tree, arr, 10);
    itr = BSTree_FindFirst(tree, PredicateFunc, &context);
    ASSERT_THAT(5 == *(int*)(BSTreeItr_Get(itr)));
    context = -1;
    itr = BSTree_FindFirst(tree, PredicateFunc, &context);
    ASSERT_THAT(BSTreeItr_Equals(BSTreeItr_Begin(tree),itr));
    BSTree_Destroy(&tree, DestroyerFunc);    
END_UNIT
 
UNIT(tree_find_not_in_tree)
    BSTreeItr itr;
    int *arr[10];
    int context = 100;
    BSTree* tree = BSTree_Create(LessComparatorFunc);
    SetupIntItemsArray(arr, 10);
    AppendIntItemsArray(tree, arr, 10);
    itr = BSTree_FindFirst(tree, PredicateFunc, &context);
    ASSERT_THAT(BSTreeItr_Equals(BSTreeItr_End(tree), itr));
    BSTree_Destroy(&tree, DestroyerFunc);    
END_UNIT


UNIT(tree_find_uninitialized)
    BSTreeItr itr;
    int context = 1000;
    BSTree* tree = BSTree_Create(LessComparatorFunc);
    BSTree_Destroy(&tree, DestroyerFunc);
    itr = BSTree_FindFirst(tree, PredicateFunc, &context);
    ASSERT_THAT(!itr);
END_UNIT
 
UNIT(tree_for_each)
    BSTreeItr itr;
    int *arr[10];
    BSTree* tree = BSTree_Create(LessComparatorFunc);
    SetupIntItemsArray(arr, 10);
    AppendIntItemsArray(tree, arr, 10);
    itr = BSTree_ForEach(tree, BSTREE_TRAVERSAL_PREORDER, PrintNode, "Node");
    ASSERT_THAT(BSTreeItr_Equals(BSTreeItr_End(tree), itr));
    BSTree_Destroy(&tree, DestroyerFunc);
END_UNIT

UNIT(tree_remove)
    BSTreeItr itr;
    int *arr[10];
    int *item;
    int i;
    int context = 0;
    BSTree* tree = BSTree_Create(LessComparatorFunc);
    SetupIntItemsArray(arr, 10);
    AppendIntItemsArray(tree, arr, 10);
    for (i = 0; i < 10; i++)
    {
        itr = BSTree_FindFirst(tree, PredicateFunc, &context);
        item = (int *)BSTreeItr_Remove(itr);
        ASSERT_THAT(*item == *arr[i]);
    }
    BSTree_Destroy(&tree, DestroyerFunc);    
END_UNIT

UNIT(tree_remove_uninitialized)
    void *item;
    BSTreeItr itr = NULL;
    item = BSTreeItr_Remove(itr);
    ASSERT_THAT(NULL == item);
END_UNIT

TEST_SUITE(binary_search_tree)
    TEST(tree_create)
    TEST(tree_destroy)
    TEST(tree_insert)
    TEST(tree_insert_uninitialized)
    TEST(tree_insert_alredy_exist)
    TEST(tree_find)
    TEST(tree_find_not_in_tree)
    TEST(tree_find_uninitialized)
    TEST(tree_remove)
    TEST(tree_remove_uninitialized)    
    TEST(tree_for_each)
END_SUITE
