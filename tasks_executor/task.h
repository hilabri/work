#ifndef __TASK_H__
#define __TASK_H__

#include <time.h>    
#include <stdlib.h>
#include <stdio.h>

typedef struct Task Task;
typedef int (*TaskFunction)(void* _m_context);

/** 
 * @brief Create a task
 * @returns a pointer to the created task
 * @param[in] - _func - the provided task function calls
 * @param[in] - _context - the provided context, to be sent to function
 * @param[in] - _period_ms - the provided period of recurrence in milliseconds.
 * @param[in] - __clk_id - the provided clock id.
 * @retval NULL on failure due to allocation failure
 */
Task *Task_Create(TaskFunction _func, void *_context, size_t _period_ms, clockid_t _clk_id);

/** 
 * @brief Destroy a task
 * @params[in] _task - task to be deallocated. 
 */
void Task_Destroy(Task *_task);

/** 
 * @brief run task 
 * Task will calculate Sleeping time and sleep()if needed 
 * @param[in] _task - the task 
 * @param[out] _res - if 0 task sould run again, alse task finshed runing  
 * @return TaskFunction call result on success, or -1 on error
 */
int Task_Run(Task *_task);

/** 
 * @brief compare _taskA to taskB, and check if taskA sould run before  taskB
 * @param[in] __taskA , _taskB 
 * @return positive number if _timeA < _timeB, 0 otherwise 
 */
int Task_LessComparator(Task *_taskA, Task *_taskB);

/** 
 * @brief calculate and set the next time the task should run
 * @param[in] _task - the task 
 */
void Task_SetNextRunTime(Task *_task);
#endif /* __TASK_H__ */
