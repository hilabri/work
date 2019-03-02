#ifndef __AGGREGATOR_H__
#define __AGGREGATOR_H__

#include "cdr.h"
#include <stdio.h>

typedef struct Aggregate Aggregate;

typedef struct CdrSum CdrSum;

typedef enum Aggregate_Result
{
    AGGREGATE_SUCCESS,
    AGGREGATE_FAIL, 
    AGGREGATE_UNINITIALIZED
} Aggregate_Result;

typedef enum Aggregate_TargetOperator
{
    OPERATOR_SAME = 0,
    OPERATOR_OTHER = 1,
    OPERATOR_TARGET_COUNT = 2
} Aggregate_TargetOperator;

typedef enum Aggregate_Direction
{
    INCOMING = 0, /* incoming traffic, including downloads */
    OUTGOING = 1, /* outgoing traffic, including uploads */
    DIRECTION_COUNT = 2
} Aggregate_Direction;

typedef struct
{
    char m_operatorName[OPERATOR_NAME_SIZE];
    unsigned int m_calls[DIRECTION_COUNT][OPERATOR_TARGET_COUNT];
    unsigned int m_sms[DIRECTION_COUNT][OPERATOR_TARGET_COUNT];
    unsigned int m_internetMbs[DIRECTION_COUNT];
} SubscriberInfo;

typedef struct
{
    char m_brandName[OPERATOR_NAME_SIZE];
    unsigned int m_mccMnc;
    unsigned int m_calls[DIRECTION_COUNT];
    unsigned int m_sms[DIRECTION_COUNT];
} OperatorInfo;

Aggregate *Aggregate_Create(size_t _subscriberHashSize, size_t _operatorHashSize);
void Aggregate_Destroy(Aggregate **_aggregate);

Aggregate_Result Aggregate_InsertCdrData(Aggregate *_aggregate, Cdr *_cdr);

/* reports */
typedef int(*FormatFunc)(const void *_ID, void *_Info, void *_Pfile);

Aggregate_Result Aggregate_RetrieveSubscriberData(Aggregate *_aggregate, const char *_subscriberName, SubscriberInfo **_subscriberInfo);
Aggregate_Result Aggregate_RetrieveOperatorData(Aggregate *_aggregate, const char *_operatorName, OperatorInfo **_operatorInfo);
Aggregate_Result Aggregate_RetrieveAllOperators(Aggregate *_aggregate, FormatFunc _format, FILE *fp);
Aggregate_Result Aggregate_RetrieveAllSubscribers(Aggregate *_aggregate, FormatFunc _format, FILE *fp);


#endif
