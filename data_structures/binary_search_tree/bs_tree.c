#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "bs_tree.h"
#include <assert.h>
#define MAGIC 0XBEEFBEEF
#define IS_TREE(T)  ((T) && (MAGIC == (T)->m_magic))
#define FALSE 0
#define TRUE 1
#define INIT_ERR -1

/********************************************************************/
Tree *TreeCreate(void)
{
    Tree *tree= malloc(sizeof(Tree));
    assert(tree);
    tree->m_root = NULL;
    tree->m_magic = MAGIC;
    return tree;
}

/********************************************************************/
static TreeNode *FindLocation(TreeNode *_treeNode, int _item)
{
    if (_treeNode->m_data > _item)
    {
        return (!_treeNode->m_left) ? _treeNode : FindLocation(_treeNode->m_left, _item);
    } 
    if (_treeNode->m_data < _item) 
    {
        return (!_treeNode->m_right) ? _treeNode : FindLocation(_treeNode->m_right, _item);
    }
    return _treeNode; /* if equals */      
}

/********************************************************************/
ADTErr TreeInsert(Tree *_tree, int _item)
{   
    TreeNode *treeNode;
    TreeNode *parent;

    if(!IS_TREE(_tree)) 
    {
        return ERR_NOT_INITIALIZED; 
    }
    
    treeNode = malloc(sizeof(TreeNode));
    if (!treeNode)
    {
        return ERR_ALLOCATION_FAILED;
    }
    treeNode->m_data = _item;
    treeNode->m_left = NULL;
    treeNode->m_right = NULL; 
    treeNode->m_parent = NULL;

    if(!_tree->m_root) /* if tree is empty */
    {   
        _tree->m_root = treeNode;
        return ERR_OK; 
    }
    
    parent = FindLocation(_tree->m_root, _item);
    if(parent->m_data == _item)
    {
        free(treeNode);
        return ERR_ITEM_EXIST; 
    }   
    if(parent->m_data > _item)
    {
        parent->m_left = treeNode;
        treeNode->m_parent = parent;
    }
    if(parent->m_data < _item)
    {
        parent->m_right = treeNode;
        treeNode->m_parent = parent;
    } 
    return ERR_OK;
}

/********************************************************************/
int TreeFind(Tree *_tree, int _item)
{   
    TreeNode *node;
    if(!IS_TREE(_tree))
    {
        return INIT_ERR;
    }
    if (!_tree->m_root)
    {
        return FALSE;
    }
    node = FindLocation(_tree->m_root, _item);
    return (node->m_data == _item);
}

/********************************************************************/
/*left->root->right*/    
static void InOrderPrint(TreeNode *_treeNode)  
{   
    if(!_treeNode) 
    {
        return;
    }
    InOrderPrint(_treeNode->m_left);
    printf("%d ",_treeNode->m_data);
    InOrderPrint(_treeNode->m_right);
}   

/********************************************************************/
/*left->right->root*/ 
static void PostOrderPrint(TreeNode *_treeNode) 
{   
    if(!_treeNode) 
    {
        return;
    }
    PostOrderPrint(_treeNode->m_left);
    PostOrderPrint(_treeNode->m_right);
    printf("%d ",_treeNode->m_data);
}  

/********************************************************************/
/*root->left->rigth*/
static void PreOrderPrint(TreeNode *_treeNode)
{
    if(!_treeNode) 
    {
        return;
    }
    printf("%d ",_treeNode->m_data);
    PreOrderPrint(_treeNode->m_left);
    PreOrderPrint(_treeNode->m_right);
}

/********************************************************************/
void TreePrint(Tree *_tree, TreeTraversal _mode)
{
    assert(_tree);
    switch(_mode)
    {
        case TREE_IN_ORDER: 
                        InOrderPrint(_tree->m_root);
                        printf("\n");
                        break;

        case TREE_POST_ORDER: 
                        PostOrderPrint(_tree->m_root);                       
                        printf("\n");
                        break;

        case TREE_PRE_ORDER:
                        PreOrderPrint(_tree->m_root);    
                        printf("\n");
                        break;
        default:
                break;
    }
}

/********************************************************************/
static void FreeTreeNodes(TreeNode *_treeNode)
{
    if (!_treeNode)
    {
        return;
    }
    FreeTreeNodes(_treeNode->m_left);
    FreeTreeNodes(_treeNode->m_right);
    free(_treeNode);
}

/********************************************************************/
void TreeDestroy(Tree *_tree)
{
    if(!IS_TREE(_tree))
    {
        return;
    }
    FreeTreeNodes(_tree->m_root);
    _tree->m_magic = 0xDEADBEEF;
    free(_tree);
}   










