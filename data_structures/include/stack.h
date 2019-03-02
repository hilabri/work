#ifndef __STACK_H__
#define __STACK_H__

/** 
 *  @file stack.h
 *  @brief Create a generic stack data type implemented over generic stack
 * 
 *  @details Implemented as wrapper or policy module over double linked stack
 *  *  Nodes are allocated from heap an store pointer to user proveded element.
 * 
 *  @author Alex Katz (alexanderk@experis.co.il) 
 * 
 *  @bug No known bugs.
 */ 

#include <stddef.h>  /* size_t */

typedef struct Stack Stack;

typedef enum Stack_Result {
	STACK_SUCCESS = 0,
	STACK_UNINITIALIZED_ERROR,			/**< Uninitialized stack 					 	*/
	STACK_ALLOCATION_ERROR,				/**< Node allocation failed due to heap error   */
	STACK_NULL_ELEMENT_ERROR,
	/* Add more as needed by your implementation */
	STACK_OVERFLOW_ERROR,
	STACK_UNDERFLOW_ERROR
} Stack_Result;

/** 
 * @brief Create a stack
 * 
 * @returns a pointer to the created stack.
 * @retval NULL on failure due to allocation failure
 */
Stack* Stack_Create(void);

/** 
 * @brief Destroy stack
 * @details Destroys the stack completely 
 *          optionally destroys elements using user provided function
 *
 * @params[in] _pStack : A pointer to previously created Stack returned via StackCreate
 *					   on completion *_pStack will be null
 * @params[in] _elementDestroy : A function pointer to be used to destroy elements inserted into the stack
 *             or a null if no such destroy is required
 */
void Stack_Destroy(Stack** _pStack, void (*_elementDestroy)(void* _item));

/** 
 * @brief Add element to the of stack
 * @details time complexity: O(1).
 *
 * @params _stack A previously created Stack ADT returned via StackCreate
 * @params _item An item to add to the stack
 * @returns error code
 * @retval STACK_SUCCESS on success
 * @retval STACK_UNINITIALIZED_ERROR if the stack is not initialized
 * @retval STACK_NULL_ELEMENT_ERROR  if _item is NULL
 * @retval STACK_ALLOCATION_ERROR on memory allocation failure
 */
Stack_Result Stack_Push(Stack* _stack, void* _item);

/** @brief Remove element from stack
 *  @details if successfull, stores a pointer to the removed item in _item
 *  time complexity O(1).
 *
 * @params _stack : A previously created Stack ADT returned via StackCreate
 * @params _pItem: To store a pointer to the removed item in
 * @returns 
 * @retval 
 */
Stack_Result Stack_Pop(Stack* _stack, void** _pItem);

/** @brief Fetch element from stack
 *  @details if not empty fetches user data from top of the stack
 *  time complexity O(1).
 *
 * @params _stack : A previously created Stack ADT returned via StackCreate
 * @params _pItem: To store a pointer to the removed item in
 * @returns 
 * @retval 
 */
Stack_Result Stack_Top(Stack* _stack, void** _pItem);

/** @brief check is there are elements in the stack
 *  @details
 *  time complexity O(1).
 *
 * @params _stack : A previously created Stack ADT returned via StackCreate
 * @returns non-zero if there are no elements in the stack
 * @retval 
 */
int Stack_IsEmpty(Stack* _stack);

#endif /* __STACK_H__ */
