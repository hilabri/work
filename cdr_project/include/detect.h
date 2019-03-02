#ifndef __DETECT_H__
#define __DETECT_H__

#define MAX_NAME_SIZE 50
#define MASK_SIZE 10
#define DIR_SIZE 50

typedef struct Detect Detect;

typedef enum Detect_Result
{
    DETECT_SUCCESS,
    DETECT_THREAD_FAIL,
    DETECT_GET_FAIL,
    DETECT_UNINITIALIZED
} Detect_Result;

Detect *Detect_Create(size_t filesCapacity, size_t _detectThreadNum, char* cdrFileMask, char* inputDirectory, char* inProgressDirectory);
void Detect_Destroy(Detect **_detect);

Detect_Result Detect_Run(Detect *_detect);
void Detect_Stop(Detect *_detect);


Detect_Result Detect_Resume(Detect *_detect);
void Detect_Pause(Detect *_detect);

/* get the next file that was stored -  into PFile */
Detect_Result Detect_GetFile(Detect *_detect, char *_Pfile);

#endif /* __DETECT_H__ */

