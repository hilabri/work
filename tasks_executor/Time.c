#include "Time.h"
#include <math.h>
 
#define MILLIS_IN_SEC 1000
#define MICROS_IN_SEC 1000000
#define NANOS_IN_SEC 1000000000
#define NANOS_IN_MILLI 1000000
#define NANOS_IN_MICRO 1000

struct Time
{
	timespec m_nextRunTime;
	clockid_t m_clockId;  
};

/***************************************/

static timespec Time_MsecToSpec(size_t _msec)
{
    timespec spec;
    spec.tv_sec = _msec / MILLIS_IN_SEC;
    spec.tv_nsec = (_msec % MILLIS_IN_SEC) * NANOS_IN_MILLI;
    return spec;
}

/***************************************/

static timespec Time_Add(timespec _timeA, timespec _timeB)
{
    timespec addition;
    addition.tv_sec = _timeA.tv_sec + _timeB.tv_sec + (_timeA.tv_nsec + _timeB.tv_nsec) / NANOS_IN_SEC;
    addition.tv_nsec = (_timeA.tv_nsec + _timeB.tv_nsec) % NANOS_IN_SEC; 
    return addition;
}

/***************************************/

static useconds_t SpecToMicroseconds(timespec timeInSpec)
{
    return (timeInSpec.tv_sec * MICROS_IN_SEC) + round(timeInSpec.tv_nsec / NANOS_IN_MICRO);
}

/***************************************/
static useconds_t Time_Sub(timespec _timeA, timespec _timeB)
{
    const useconds_t microA = SpecToMicroseconds(_timeA);
    const useconds_t microB = SpecToMicroseconds(_timeB);
	return (microB < microA) ? (microA - microB) : 0;
}

/***************************************/

void Time_SetNextRunTime(Time *_time, size_t _period_ms)
{
    timespec currentTime;
    timespec interval = Time_MsecToSpec(_period_ms);
    clock_gettime(_time->m_clockId, &currentTime);
    _time->m_nextRunTime = Time_Add(currentTime, interval);
}

/***************************************/

Time *Time_Create(size_t _period_ms, clockid_t _clk_id)
{
    Time *time = malloc(sizeof(Time));
    if(!time)
    {
        return NULL;
    }
    time->m_clockId = _clk_id;
    Time_SetNextRunTime(time, _period_ms);
    return time;
}
/***************************************/

void Time_Destroy(Time *_time)
{
    if(!_time)
    {
        return;
    }
    free(_time);
}

/***************************************/

int Time_IsLess(Time *_timeA, Time *_timeB)
{
	timespec runTimeA = _timeA->m_nextRunTime;
	timespec runTimeB = _timeB->m_nextRunTime;
    if (runTimeA.tv_sec != runTimeB.tv_sec) 
    {
        return (runTimeA.tv_sec < runTimeB.tv_sec);
    }
    return (runTimeA.tv_nsec < runTimeB.tv_nsec); 
}


/***************************************/

void Time_Sleep(Time *_time)
{
    timespec currentTime;
    useconds_t sleepPeriod;
    timespec runTime = _time->m_nextRunTime;
    clock_gettime(_time->m_clockId, &currentTime);
    sleepPeriod = Time_Sub(runTime,currentTime);
    if (0 < sleepPeriod)
    {
        usleep(sleepPeriod);
    }
}

