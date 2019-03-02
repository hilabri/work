#include "executor.h"
#include "PQ.h"
#include "task.h"

#define MAGIC 0xDEADBEEF
#define DEAD_MAGIC 0xDEADDEAD
#define IS_VALID(V)            ((V) && (MAGIC == (V)->m_magic)) 

#define FALSE 0
#define TRUE 1
#define EXECUTER_ERROR -1

struct PeriodicExecutor {
    PQ* m_priorityQueue;
    size_t m_magic; 
    size_t m_runCount;
    int m_shouldPause;
    const char* m_name;
	clockid_t m_clk_id;
};

/************************************/

static int TaskComparator(void *_taskA, void *_taskB)
{
    return Task_LessComparator((Task *)_taskA, (Task *)_taskB);
}
/************************************/

PeriodicExecutor* PeriodicExecutor_Create(const char* _name, clockid_t _clk_id )
{
	PeriodicExecutor *executor = malloc(sizeof(PeriodicExecutor));
	if (!executor)
	{
		return NULL;
	}
	executor->m_priorityQueue = PQ_Create(TaskComparator);
	executor->m_magic = MAGIC;
	executor->m_shouldPause = FALSE;
	executor->m_clk_id = _clk_id;
	executor->m_name = _name;
	return executor;
}

/*******************************/

static void TaskDestroy(void *_task)
{
    Task_Destroy((Task *)_task);
}

/*******************************/

void PeriodicExecutor_Destroy(PeriodicExecutor *_executor)
{
	if(!IS_VALID(_executor))
	{
		return;
	}
	PQ_Destroy(_executor->m_priorityQueue, TaskDestroy);
	_executor->m_magic = DEAD_MAGIC;
	free(_executor);
}

/*******************************/
int PeriodicExecutor_Add(PeriodicExecutor* _executor, int (*_taskFunction)(void *), void* _context, size_t _period_ms)
{
	int res;
	Task *task;
	if(!IS_VALID(_executor))
	{
		return  EXECUTER_ERROR;
	}
	task = Task_Create(_taskFunction, _context, _period_ms, _executor->m_clk_id);
	if (!task)
	{ 
		return EXECUTER_ERROR;
	}
	res = PQ_Add(_executor->m_priorityQueue, (void*)task);
	if (!res) 
	{
		return EXECUTER_ERROR;
	}
	return 0;
}

/*******************************/
int PeriodicExecutor_Run(PeriodicExecutor* _executor)
{
	int completed;
	Task *task;
	if(!IS_VALID(_executor))
	{
		return EXECUTER_ERROR;
	}
	_executor->m_runCount = 0;
	_executor->m_shouldPause = FALSE;
	while (!_executor->m_shouldPause && (0 < PQ_GetSize(_executor->m_priorityQueue)))
	{
    	PQ_Remove(_executor->m_priorityQueue, (void **)&task);
    	if (!task)
    	{
    		return EXECUTER_ERROR;
    	}
    	completed = Task_Run(task);
    	_executor->m_runCount++;
    	if (!completed)
    	{
			Task_SetNextRunTime(task);
        	PQ_Add(_executor->m_priorityQueue, (void*)task);
		}
        else
        {
            Task_Destroy(task);
        }
	}
	return _executor->m_runCount;
}

/*******************************/
int PeriodicExecutor_Pause(PeriodicExecutor* _executor)
{
	if(!IS_VALID(_executor))
	{
		return EXECUTER_ERROR;
	}
	_executor->m_shouldPause = TRUE;
	return PQ_GetSize(_executor->m_priorityQueue);
}

