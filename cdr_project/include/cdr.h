#ifndef __CDR_H__
#define __CDR_H__

#include <stdlib.h>

#define OPERATOR_NAME_SIZE 64
#define ID_SIZE 15
#define TIME_SIZE 10

typedef enum
{
    MOC,     /* outgiong call */
    MTC,     /* incoming call */
    SMS_MO,  /* outgoing sms */
    SMS_MT,  /* uncoming sms */
    GPRS,    /* internet */
    IN_VALID
}CallType;

typedef struct
{
    char m_IMSI[ID_SIZE]; /* subs id*/
    char m_MSISDN[ID_SIZE];
    char m_IMEI[ID_SIZE];
    char m_operatorName[OPERATOR_NAME_SIZE];
    int m_operatorMCC;              /* oper id */
    CallType m_callType;
    char m_callDate[TIME_SIZE];
    char m_callTime[TIME_SIZE];
    char m_partyMSISDN[ID_SIZE];
    char m_partyOperator[OPERATOR_NAME_SIZE];
    int m_callDuration;
    int m_mbDownload;
    int m_mbUpload;
}Cdr;

#endif
