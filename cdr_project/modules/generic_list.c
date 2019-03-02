#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "list_itr.h"

#define MAGIC 0xDEADBEEF
#define IS_VALID(V)            ((V) && (MAGIC == (V)->m_magic)) 

#define FALSE 0
#define TRUE 1
#define UNITIALIZED -1

#define LIST_HEAD(GDL)	(&((GDL)->m_head))
#define LIST_TAIL(GDL)	(&((GDL)->m_tail))
#define LIST_FIRST(GDL)	(LIST_HEAD(GDL)->m_next)
#define LIST_LAST(GDL)	(LIST_TAIL(GDL)->m_prev)

#define IS_EMPTY(GDL)	(LIST_FIRST(GDL) == LIST_TAIL(GDL)) 


typedef struct Node Node;

struct Node
{
    void* m_data;
    Node *m_next;
    Node *m_prev;
};

struct List
{
    Node m_head;
    Node m_tail;
    size_t m_magic;
};

/*****************************************/
static Node* CreateNode(Node* _before, void* _item)
{
	Node* node = (Node*) malloc(sizeof(Node));
	if (node)
	{		
		node->m_prev		 = _before->m_prev;
		node->m_next		 = _before;

		node->m_next->m_prev = node;
		node->m_prev->m_next = node;

		node->m_data	= _item;		
	}
	return node;
}


static void* DeleteNode(Node* _node)	
{
	void* data = _node->m_data;
	
	_node->m_prev->m_next = _node->m_next;
	_node->m_next->m_prev = _node->m_prev;
		
	free(_node);
	return data;
}	

/*****************************************/

List* List_Create(void)
{
    List *list = malloc(sizeof(List));
    if (!list) 
    {
        return NULL;
    }
    list->m_head.m_prev = NULL;
    list->m_head.m_next = &(list->m_tail);
    list->m_tail.m_next = NULL;
    list->m_tail.m_prev= &(list->m_head);
    list->m_magic =  MAGIC;
    return list;
}


void List_Destroy(List** _pList, void (*_elementDestroy)(void* _item)) 
{
    Node *temp;
    Node *curr;
    if (!_pList || !IS_VALID(*_pList))
    {
        return;
    }
    (*_pList)->m_magic = UNITIALIZED; /* magic in head and tail (DEADDEAD) */
    curr =  (*_pList)->m_head.m_next;
    while (curr != &(*_pList)->m_tail) /* #define - is empty?! */
    {       
        /* DeleteNode function?! */
        if (_elementDestroy)
  		{
        	_elementDestroy(curr->m_data);
   		}
   		temp = curr->m_next;
        free(curr);
        curr = temp;
    }
    free(*_pList);
    *_pList = NULL;
}
 				
/***************************************************************************/
List_Result List_PushHead(List* _list, void* _item) 
{
    if (!_list) 
    {
        return LIST_UNINITIALIZED_ERROR;
    }
    return ((CreateNode(LIST_FIRST(_list), _item)) ? LIST_SUCCESS : LIST_ALLOCATION_ERROR);
}


List_Result List_PushTail(List* _list, void* _item)
{

    if (!_list)
    {
        return LIST_UNINITIALIZED_ERROR;
    }
	return ((CreateNode(LIST_TAIL(_list), _item)) ? LIST_SUCCESS : LIST_ALLOCATION_ERROR);
}


List_Result List_PopHead(List* _list, void** _pItem) 
{
    if (!_list || !_pItem) 
    {
        return LIST_UNINITIALIZED_ERROR;  
    }
    if (IS_EMPTY(_list))
	{
		return LIST_UNDERFLOW_ERROR;
	}
	*_pItem = DeleteNode(LIST_FIRST(_list));
	return LIST_SUCCESS;
}

    
List_Result List_PopTail(List* _list, void** _pItem)
{
    if (!_list || !_pItem) 
    {
        return LIST_UNINITIALIZED_ERROR;
    }
	if (IS_EMPTY(_list))
	{
		return LIST_UNDERFLOW_ERROR;
	}
	*_pItem = DeleteNode(LIST_LAST(_list));
    return LIST_SUCCESS;
}


size_t List_Size(const List* _list)
{
    Node *i;
    int count = 0;
    if (!_list)
    {
        return UNITIALIZED;
    }
    for (i = _list->m_head.m_next; i != &(_list->m_tail); i = i->m_next)
    {
        count++;
    }
    return count;
}

/*****************************************************************/
List_Result List_GetHead(List* _list, void** _pItem) 
{
    if (!_list || !_pItem) 
    {
        return LIST_UNINITIALIZED_ERROR;  
    }
    if (IS_EMPTY(_list))
	{
		return LIST_UNDERFLOW_ERROR;
	}
	*_pItem = LIST_FIRST(_list)->m_data;
	return LIST_SUCCESS;
}


int List_IsEmpty(List* _list)
{
	return (IS_EMPTY(_list));
}

/***************************Itr**************************************/

ListItr ListItr_Begin(const List* _list)
{
	return IS_VALID(_list) ? (ListItr)(LIST_FIRST(_list)) : NULL ;
}

ListItr ListItr_End(const List* _list)
{
	return IS_VALID(_list) ? (ListItr)(LIST_TAIL(_list)) : NULL ;
}

int ListItr_Equals(const ListItr _a, const ListItr _b)
{
	return _a==_b;
}

ListItr ListItr_Next(ListItr _itr)
{
	Node* node = (Node*)_itr;
	return (ListItr) ((node) ? ((node->m_next) ? node->m_next : node) : NULL);
}

ListItr ListItr_Prev(ListItr _itr)
{
	Node* node = (Node*)_itr;
	return (ListItr) ((node) ? ((node->m_prev->m_prev) ? node->m_prev : node) : NULL);
}

void* ListItr_Get(ListItr _itr)
{	
	Node* node = (Node*)_itr;
	return ((node) ? ((node->m_next) ? node->m_data : NULL) : NULL);
}



void* ListItr_Set(ListItr _itr, void* _element)
{
	Node* node = (Node*)_itr;
	void* data = NULL;
	if (_element && node && node->m_next)
	{
		data = node->m_data;
		node->m_data = _element;
	}
	return data;
}


ListItr ListItr_InsertBefore(ListItr _itr, void* _element)
{
	if (!_itr || !_element)
	{
		return NULL;
	}
	 return (ListItr)CreateNode((Node*)_itr, _element);
}

void* ListItr_Remove(ListItr _itr)
{
	if (!_itr  || !((Node*)_itr)->m_next)
	{
		return NULL;
	}
	return DeleteNode((Node*)_itr);
}


