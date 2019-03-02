#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__
#include <stddef.h>
#include <time.h>

typedef struct PeriodicExecutor PeriodicExecutor;


/** 
 * @brief Create a Periodic Executor
 * @returns a pointer to the created executor
 * @retval NULL on failure due to allocation failure
 */
PeriodicExecutor* PeriodicExecutor_Create(const char* _name, clockid_t _clk_id );


/** 
 * @brief Destroy a Periodic Executor
 * @params[in] _executor - executer to be deallocated. 
 * @params[in] _elementDestroy - A function pointer to be used to destroy all elements 
 */
void PeriodicExecutor_Destroy(PeriodicExecutor* _executor);

/*
 * @brief Add elemnt to the executor.
 * @params[in] _executor - the executer. 
 * @param[in] - _func - function calls to be invoked for each element
 * @param[in] - _context - context to be sent to function
 * @param[in] - _period_ms - the period of recurrence in milliseconds.
 */
int PeriodicExecutor_Add(PeriodicExecutor* _executor, int (*_taskFunction)(void *), void* _context, size_t _period_ms);
 /** 
 * @brief run executer 
 * @param[in] _executor - the executor to run 
 * @return the number of execution cycles (user function calls) performed, or -1 on error
 */
int PeriodicExecutor_Run(PeriodicExecutor* _executor);

/** 
* @brief Pause the execution of the executor. 
* @param[in] _executor - the executor to pause 
* @return the number of tasks remaining inside the executor, or -1 on error
*/
int PeriodicExecutor_Pause(PeriodicExecutor* _executor);

#endif /* __EXECUTOR_H__ */
