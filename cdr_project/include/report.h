#ifndef __REPORT_H__
#define __REPORT_H__

#include "aggregate.h"
#include <stdlib.h>
#include <sys/param.h> /* MAX macro */

typedef struct Report Report;

#define REPORT_MAX_IDENTIFIER_SIZE MAX(OPERATOR_NAME_SIZE, ID_SIZE)

typedef enum Report_Type
{
    REPORT_CGF_MSISDN,
    REPORT_CGF_OPERATOR,
    REPORT_CGF_ALL_SUBSCRIBERS,
    REPORT_CGF_ALL_OPERATORS,
    REPORT_CGF_AGGRAGATED_INFO
}Report_Type;

typedef enum Report_Result
{
    REPORT_SUCCESS,
    REPORT_UNINITIALIZED,
    REPORT_STORE_FAIL,
    REPORT_THREAD_FAIL,
    REPORT_NOT_EXIST
}Report_Result;

Report *Report_Create(Aggregate *_aggregate, char *_outputDirectory, size_t _reportsCapacity, size_t _reportThreadNum);
void Report_Destroy(Report **_report);

Report_Result Report_Run(Report *_report);
void Report_Stop(Report *_report);

Report_Result Report_AddRequest(Report*_report, Report_Type _type, const char *_identify);
Report_Result Report_GenerateFinal(Report *_report);

void Report_Pause(Report *_report);
Report_Result Report_Resume(Report *_report);

#endif /* __REPORT_H__ */
