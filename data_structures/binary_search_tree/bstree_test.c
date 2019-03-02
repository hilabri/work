#include <stdio.h>
#include <stdlib.h>
#include "bs_tree.h"
#include "bstree_test.h"

  
UNIT(tree_create)
    Tree* tree = TreeCreate();
    int item = 6; 
    ASSERT_THAT(tree);
    ASSERT_THAT(ERR_OK == TreeInsert(tree, item));
    TreeDestroy(tree);
END_UNIT

UNIT(tree_destroy)
    int item = 1;
    Tree* tree = TreeCreate();
    TreeDestroy(tree);
    tree = NULL;
    TreeDestroy(tree);
    ASSERT_THAT(ERR_NOT_INITIALIZED == TreeInsert(tree, item));
END_UNIT

UNIT(tree_insert)
    int i;
    Tree* tree = TreeCreate();
    for (i=0; i<5; i++)
    {
        TreeInsert(tree, i);
    }
    ASSERT_THAT(ERR_OK == TreeInsert(tree, i));
    TreeDestroy(tree);
END_UNIT

UNIT(tree_insert_uninitialized)
    int item = 1;
    Tree* tree = TreeCreate();
    TreeDestroy(tree);
    ASSERT_THAT(ERR_NOT_INITIALIZED == TreeInsert(tree, item));
END_UNIT

UNIT(tree_find)
    Tree* tree = TreeCreate();
    int array[10] = {4,6,2,5,8,6,0,3,7,9};
    int result;
    int i;
    for (i=0; i<10; i++)
    {
        TreeInsert(tree, array[i]);
    }
    for (i=0; i<10; i++)
    {
       result = TreeFind(tree, array[i]);
       ASSERT_THAT(result);
    }
    TreeDestroy(tree);
END_UNIT
     
UNIT(tree_find_uninitialized)
    int i = 2;
    Tree* tree = TreeCreate();
    TreeDestroy(tree);    
    tree = NULL; 
    ASSERT_THAT(-1 == TreeFind(tree, i));
END_UNIT
 
UNIT(tree_find_not_in_tree)
    int i = 2;
    Tree* tree = TreeCreate();
    ASSERT_THAT(0 == TreeFind(tree, i));
    TreeInsert(tree,4);
    ASSERT_THAT(0 == TreeFind(tree, i));
    TreeDestroy(tree);    
END_UNIT

UNIT(tree_print)
    Tree *tree = TreeCreate();
    int array[10] = {1,4,2,6,7,3,10,8,5,9};
    int i;
    for (i=0; i<10; i++)
    {
        TreeInsert(tree, array[i]);
    }
    TreePrint(tree, TREE_IN_ORDER);
    TreePrint(tree, TREE_PRE_ORDER);
    TreePrint(tree, TREE_POST_ORDER);
    TreeDestroy(tree);
END_UNIT


TEST_SUITE(binary_search_tree)
    TEST(tree_create)
    TEST(tree_destroy)
    TEST(tree_insert)
    TEST(tree_insert_uninitialized)
    TEST(tree_find)
    TEST(tree_find_uninitialized)
    TEST(tree_find_not_in_tree)
    TEST(tree_print)
END_SUITE
