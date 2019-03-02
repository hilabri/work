#ifndef _BS_TREE_H_
#define _BS_TREE_H_

#include <stddef.h>
#include "ADTDefs.h"

typedef struct Tree  Tree;
typedef struct TreeNode TreeNode; 

struct TreeNode
{
    int m_data;
    TreeNode *m_left;
    TreeNode *m_right;
    TreeNode *m_parent;
};

typedef enum
{
    TREE_IN_ORDER,
    TREE_POST_ORDER,
    TREE_PRE_ORDER
} TreeTraversal;

struct Tree
{
    TreeNode *m_root;
    size_t m_magic;
};

/* Create an empty TREE and return it's address, or NULL in case of error */
Tree *TreeCreate(void);

/* Destroy an existing TREE and free it's memory */
void TreeDestroy(Tree *_tree);

/* Add A NODE to the TREE */
/* Return ERR_OK if item was added and specific error code in case of error */
ADTErr TreeInsert(Tree *_tree, int _item);

/* Find if NODE EXIST in TREE */
/* Return -1 if NOT INITIALIZED, 0 if NOT EXIST, positiv number if EXIST*/
int TreeFind(Tree *_tree, int _item); 

/*Print tree according to a given Traversal Order Mode */
void TreePrint(Tree *tree, TreeTraversal _mode);


#endif /* _BS_TREE_H_ */

