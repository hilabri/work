#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>         /* size_t */
#include "list.h"
#include "stack.h"


struct Stack {
    List *m_list;
}; 

Stack* Stack_Create(void)
{
    Stack *stack = NULL;
    stack = malloc(sizeof(Stack));
    if (!stack) 
    {
        return NULL;
    }
    stack->m_list = List_Create();
    if (!stack->m_list)
    {
        free(stack);
        return NULL;
    }
    return stack;
}
    

void Stack_Destroy(Stack** _pStack, void (*_elementDestroy)(void* _item))
{
    if (!_pStack)
    {
    	return;
    }
    List_Destroy(&((*_pStack)->m_list), _elementDestroy);
    free(*_pStack);
    *_pStack = NULL;
}

static Stack_Result List_Result_To_Stack_Result(List_Result res)
{
    switch (res)
    {
    case LIST_SUCCESS: return STACK_SUCCESS;
    case LIST_UNINITIALIZED_ERROR: return STACK_UNINITIALIZED_ERROR;
    case LIST_ALLOCATION_ERROR: return STACK_ALLOCATION_ERROR;
    case LIST_NULL_ELEMENT_ERROR: return STACK_NULL_ELEMENT_ERROR;
    case LIST_OVERFLOW_ERROR: return STACK_OVERFLOW_ERROR;
    case LIST_UNDERFLOW_ERROR: return STACK_UNDERFLOW_ERROR;
    default: break;
    }
    return STACK_UNINITIALIZED_ERROR;
}

Stack_Result Stack_Push(Stack* _stack, void* _item)
{
    if (!_stack)
    {
    	return STACK_UNINITIALIZED_ERROR;
    }
    return List_Result_To_Stack_Result(List_PushHead(_stack->m_list, _item));
}

 
Stack_Result Stack_Pop(Stack* _stack, void** _pItem)
{
    if (!_stack)
    {
    	return STACK_UNINITIALIZED_ERROR;
    }
    return List_Result_To_Stack_Result(List_PopHead(_stack->m_list, _pItem));
}


Stack_Result Stack_Top(Stack* _stack, void** _pItem)
{
    if (!_stack)
    {
    	return STACK_UNINITIALIZED_ERROR;
    }
    return List_Result_To_Stack_Result(List_GetHead(_stack->m_list, _pItem));
}

int Stack_IsEmpty(Stack* _stack)
{
    return List_IsEmpty(_stack->m_list);
}

