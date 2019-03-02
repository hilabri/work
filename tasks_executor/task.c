#include "task.h"
#include "Time.h"

#define UNINITIALIZED -1

struct Task 
{
    TaskFunction m_func;
    void *m_context;
    size_t m_interval; /*in milliseconds */
    Time *m_runTime;
};

/*******************************/


Task *Task_Create(TaskFunction _func, void *_context, size_t _period_ms, clockid_t _clk_id)
{
	Task* task;
	task = malloc(sizeof(task));
	if (!task)
	{ 
		return NULL;
	}
	task->m_func = _func;
	task->m_context = _context;
	task->m_interval = _period_ms;
	task->m_runTime = Time_Create(_period_ms, _clk_id);
	return task;
}

/*******************************/

void Task_Destroy(Task *_task)
{
	if(!_task)
	{
		return;
	}
    Time_Destroy(_task->m_runTime);
	free (_task);
}

/*******************************/
int Task_Run(Task *_task)
{
	int res;
	if(!_task)
	{
		return UNINITIALIZED;
	}
	Time_Sleep(_task->m_runTime); 
	res = _task->m_func(_task->m_context);
	return res;
}

/*******************************/

int Task_LessComparator(Task *_taskA, Task *_taskB)
{
	if(!_taskA || !_taskB)
	{
		return UNINITIALIZED;
	}
	return Time_IsLess(_taskA->m_runTime, _taskB->m_runTime);
}

/*******************************/

void Task_SetNextRunTime(Task *_task)
{
	if(!_task)
	{
        	return;
	}
	Time_SetNextRunTime(_task->m_runTime, _task->m_interval); 
}

/*******************************/

