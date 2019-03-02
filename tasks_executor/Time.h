#ifndef __EXECUTOR_TIME_H__
#define __EXECUTOR_TIME_H__
   
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>


typedef struct timespec timespec; 
typedef struct Time Time; 

 /** 
 * @brief Create a Time instance
 * @returns a pointer to the created Time
 * @retval NULL on failure due to allocation failure
 */
Time *Time_Create( size_t _period_ms, clockid_t _clk_id);

/** 
 * @brief Destroy a Time instance
 * @params[in] _time - time instance to be deallocated.
*/
void Time_Destroy(Time *_time);
/** 
 * @brief compare two time variables
 * @param[in] _timeA,_timeB - times to compare  
 * @return positive number if _timeA < _timeB, 0 otherwise 
 */
int Time_IsLess(Time * _timeA, Time *_timeB);

/** 
 * @brief suspend the process until run time is reached
 */
void Time_Sleep(Time *_time);

/** 
 * @brief Calculate and Update next run time
 * @param[in] _interval -  the period of recurrence in milliseconds.
 * @param[in/out] _runTime -  Time to update
 */
void Time_SetNextRunTime(Time *_time, size_t _period_ms);

#endif /* __EXECUTOR_TIME_H__ */
