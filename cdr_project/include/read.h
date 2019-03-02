#ifndef __READ_H__
#define __READ_H__

#include "detect.h"

#define MAX_LINE_SIZE 256 

typedef struct Read Read;

typedef enum Read_Result
{
    READ_SUCCESS,
    READ_THREAD_FAIL,
    READ_GET_FAIL,
    READ_UNINITIALIZED
}Read_Result;


Read *Read_Create(size_t _linesCapacity, size_t _readThreadNum, char* _doneDirectory, Detect *_detect);
void Read_Destroy(Read **_read);

Read_Result Read_Run(Read *_read); 
void Read_Stop(Read *_read);

Read_Result Read_GetLine(Read *_read, char *_Pline);


void Read_Pause(Read *_read);
Read_Result Read_Resume(Read *_read);


#endif /* __READ_H__ */


