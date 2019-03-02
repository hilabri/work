#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "bin_tree.h"

#define MAGIC 0XDEADBEEF
#define DEAD_MAGIC 0XDEADDEAD

#define SUPER(T)       (&((T)->m_super))
#define TREE_MAGIC(T)  ((SUPER(T))->m_data)
 
#define IS_VALID(T)    ((T) && ((void*)MAGIC == TREE_MAGIC(T)))

#define ROOT(T)        (SUPER(T))->m_kid[0]
#define LEFT_SON(N)    (N)->m_kid[0] 
#define RIGHT_SON(N)   (N)->m_kid[1] 

#define IS_LESS(T,L,R)  ((T)->m_less)((L),(R))

typedef struct TreeNode TreeNode;

struct TreeNode
{
    void* m_data;
    TreeNode *m_kid[2];
    TreeNode *m_parent;
};

struct BSTree
{
    TreeNode m_super;
    LessComparator m_less;
};

/********************************************************************/
BSTree* BSTree_Create(LessComparator _less)
{
    BSTree *tree= malloc(sizeof(BSTree));
    if (!tree)
    {
    	return NULL;
    }
    tree->m_less = _less;
    LEFT_SON(SUPER(tree)) = NULL;
    RIGHT_SON(SUPER(tree)) = NULL;
    TREE_MAGIC(tree) = (void*)MAGIC;
    return tree;
}

/********************************************************************/
static TreeNode *FindLocation(BSTree *_tree, TreeNode *_treeNode, void* _item)
{
    if (IS_LESS(_tree,_item, _treeNode->m_data))
    {
        return (!LEFT_SON(_treeNode) ? _treeNode : FindLocation(_tree, LEFT_SON(_treeNode), _item));
    } 
    if (IS_LESS(_tree,_treeNode->m_data,_item))
    {
        return (!RIGHT_SON(_treeNode) ? _treeNode : FindLocation(_tree, RIGHT_SON(_treeNode), _item));
    }
    return _treeNode; /* if equals */      
}

/****************************************************/
TreeNode *CreateTreeNode(BSTree *_tree, void* _item, TreeNode *_parent)
{
    TreeNode *treeNode = malloc(sizeof(TreeNode));
    if (!treeNode)
    {
        return NULL;
    }

    treeNode->m_data = _item;
    LEFT_SON(treeNode) = NULL;
    RIGHT_SON(treeNode) = NULL;
    treeNode->m_parent = _parent;
   
    if ((SUPER(_tree) == _parent) || IS_LESS(_tree, _item, _parent->m_data))
    {
        LEFT_SON(_parent) = treeNode;
    }    
    else
    {
        RIGHT_SON(_parent) = treeNode;
    }
    
    return treeNode;
}

/********************************************************************/
BSTreeItr BSTree_Insert(BSTree* _tree, void* _item)
{   
    TreeNode *parent;

    if(!IS_VALID(_tree)) 
    {
        return NULL; 
    }
    if(!ROOT(_tree)) /* if tree is empty */
    {   
        return (BSTreeItr)CreateTreeNode(_tree, _item, SUPER(_tree));
    }
    
    parent = FindLocation(_tree, ROOT(_tree), _item);
    if(!IS_LESS(_tree ,parent->m_data, _item) && !IS_LESS(_tree, _item, parent->m_data))
    {
        return BSTreeItr_End(_tree); 
    }   
    return (BSTreeItr)CreateTreeNode(_tree,_item,parent);    
}

/********************************************************************/
BSTreeItr BSTree_FindFirst(const BSTree* _tree, PredicateFunction _predicate, void* _context)
{   
    BSTreeItr itr;
    if(!IS_VALID(_tree))
    {
        return NULL;
    }
    for (itr = BSTreeItr_Begin(_tree); !BSTreeItr_Equals(itr, BSTreeItr_End(_tree)); itr = BSTreeItr_Next(itr))
    {
        if(!_predicate(((TreeNode*)itr)->m_data, _context))
        {
            break;
        }    
    }       
    return itr;
}
/********************************************************************/
BSTreeItr BSTreeItr_Begin(const BSTree* _tree)
{
    TreeNode *node;
    if (!IS_VALID(_tree))
    {
        return NULL;
    }
    if (!ROOT(_tree))
    {
        return BSTreeItr_End(_tree);
    }
    for(node = ROOT(_tree); LEFT_SON(node); node = LEFT_SON(node));
    return (BSTreeItr)node;
}

/********************************************************************/
BSTreeItr BSTreeItr_End(const BSTree* _tree)
{
    if (!IS_VALID(_tree))
    {
        return NULL;
    }
    return  (BSTreeItr)SUPER(_tree);
}

/********************************************************************/
int BSTreeItr_Equals(BSTreeItr _a, BSTreeItr _b)
{
    return (_a == _b);    
}
/********************************************************************/

BSTreeItr BSTreeItr_Next(BSTreeItr _itr)
{
    TreeNode *node = (TreeNode*)_itr;
    if(RIGHT_SON(node))
    {
        for(node = RIGHT_SON(node); LEFT_SON(node); node = LEFT_SON(node));    
    }
    else
    {
        for(; node->m_parent && (RIGHT_SON(node->m_parent) == node); node = node->m_parent);
        node = (LEFT_SON(node->m_parent) == node) ? node->m_parent : node;
    }
    return (BSTreeItr)node;
}

/********************************************************************/
BSTreeItr BSTreeItr_Prev(BSTreeItr _itr)
{
    TreeNode *node = (TreeNode*)_itr;
    if(LEFT_SON(node))
    {
        for(node = LEFT_SON(node); RIGHT_SON(node); node = RIGHT_SON(node));    
    }
    else
    {
        for(; node->m_parent && (LEFT_SON(node->m_parent) == node); node = node->m_parent);
        node = (RIGHT_SON(node->m_parent) == node) ? node->m_parent : node;
    }
    return (BSTreeItr)node;
    
}
/********************************************************************/
void* BSTreeItr_Get(BSTreeItr _itr)
{
    if (!_itr)
    {
        return NULL;
    }
    return ((TreeNode*)_itr)->m_data; 
}


/********************************************************************/
void  BSTree_Destroy(BSTree** _tree, void (*_destroyer)(void*))
{
    BSTreeItr itr, itrNext;
    void *data;
    if(!_tree || !IS_VALID(*_tree))
    {
        return;
    }
    TREE_MAGIC(*_tree) = (void *)DEAD_MAGIC;
    itr = BSTreeItr_Begin(*_tree);
    while (!BSTreeItr_Equals(itr, BSTreeItr_End(*_tree)))
    {
        itrNext = BSTreeItr_Next(itr);
        data = BSTreeItr_Remove(itr);
        if (_destroyer)
        {  
            _destroyer(data);
        }
        itr = itrNext;
    }
    free(*_tree);
    *_tree = NULL;
}   


/********************************************************************/
BSTreeItr BSTree_ForEach(const BSTree* _tree, TreeTraversalMode _mode, ActionFunction _action, void* _context)
{
    BSTreeItr itr;
    if (!IS_VALID(_tree))
    {
        return NULL;
    }
    for (itr = BSTreeItr_Begin(_tree); !BSTreeItr_Equals(itr, BSTreeItr_End(_tree)); itr = BSTreeItr_Next(itr))
    {
        if (!_action(BSTreeItr_Get(itr), _context))
        {
            break;
        }
    }
    return itr;
}

/***************************************************************/

static void* Remove_Leaf(TreeNode* _node)
{
    void* data = _node->m_data;
    if (_node->m_parent && (RIGHT_SON(_node->m_parent) == _node))
    {
        RIGHT_SON(_node->m_parent) = NULL;
    }
    else
    {
        LEFT_SON(_node->m_parent) = NULL;
    }
    _node->m_data = NULL;
    free(_node);
    return data;
}

static void* Remove_Parent_To_One(TreeNode* _node)
{
    TreeNode *son;
    void* data = _node->m_data;

    son = LEFT_SON(_node) ? LEFT_SON(_node) : RIGHT_SON(_node);
    son->m_parent = _node->m_parent;
    
    if (RIGHT_SON(_node->m_parent) == _node)
    {
        RIGHT_SON(_node->m_parent) = son;
    }
    else
    {
        LEFT_SON(_node->m_parent) = son;
    }
    _node->m_data = NULL;
    free(_node);
    return data;
}



static void* Remove_Parent_To_Two(TreeNode* _node)
{
    void* data = _node->m_data;
    TreeNode* swapNode;

    for(swapNode = RIGHT_SON(_node); LEFT_SON(swapNode); swapNode = LEFT_SON(swapNode));

    if (LEFT_SON(swapNode->m_parent) == swapNode)
    {
        LEFT_SON(swapNode->m_parent) = NULL;
    }
    else
    {
        RIGHT_SON(swapNode->m_parent) = NULL;
    }
    swapNode->m_parent = _node->m_parent;
    LEFT_SON(swapNode) = LEFT_SON(_node);
    RIGHT_SON(swapNode) = RIGHT_SON(_node);
    if (RIGHT_SON(_node->m_parent) == _node)
    {
        RIGHT_SON(_node->m_parent) = swapNode;
    }
    else
    {
        LEFT_SON(_node->m_parent) = swapNode;
    }   
    _node->m_data = NULL;
    free(_node);
    return data;
}


void* BSTreeItr_Remove(BSTreeItr _itr)
{
    TreeNode *node;
    if (!_itr)
    {
        return NULL;
    }
    node = (TreeNode*)_itr;
    if (!LEFT_SON(node) && !RIGHT_SON(node))
    {
        return Remove_Leaf(node);
    }
    if (LEFT_SON(node) && RIGHT_SON(node))
    {
        return Remove_Parent_To_Two(node);
    }
    return Remove_Parent_To_One(node);
}





