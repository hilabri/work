#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "cdr.h"
#include "waitable_queue.h"
#include "parser.h"
#include "thread.h"
#include "zlog4.h"

#define TRUE 1
#define FALSE 0
#define INT_BASE 10

#define MODULE "parser"

static void Parser_CreateCdr(char* _line, Cdr *_cdr);
static void *Parser_StoreCdr(void *_parser);

struct Parser
{
    Aggregate *m_aggregate;
    Read *m_read;
    int m_isRunning;
    size_t m_threadsNum;
    pthread_t *m_threads;
    pthread_cond_t m_cond;
   	pthread_mutex_t m_mutex;
};


Parser *Parser_Create(size_t _parserThreadNum, Read *_read, Aggregate *_aggregate)
{
    Parser *parser = malloc(sizeof(Parser));
    if(!parser)
    {
        ZLOG(LOG_ERROR, MODULE, "Read failed to allocate memory");
        return NULL;
    }
    parser->m_threads = malloc(_parserThreadNum * sizeof(pthread_t));
    if(!parser->m_threads)
    {
        free(parser);
        ZLOG(LOG_ERROR, MODULE, "Read failed to allocate memory for threads");
        return NULL;
    }
    pthread_mutex_init(&parser->m_mutex, NULL);
	pthread_cond_init(&parser->m_cond,NULL);
    parser->m_read = _read;
    parser->m_aggregate = _aggregate;
    parser->m_threadsNum = _parserThreadNum;
    parser->m_isRunning = FALSE;
    ZLOG(LOG_INFO, MODULE, "Parser created");
    return parser; 
}
/*--------------------------------------------------*/
void ElementDestroy(void *_cdr)
{
    free((Cdr*)_cdr);
}

/*--------------------------------------------------*/
void Parser_Stop(Parser *_parser)
{
    if(!_parser)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return; 
    }
    Cancel_Threads(_parser->m_threadsNum, _parser->m_threads);
    Join_Threads(_parser->m_threadsNum, _parser->m_threads);
    ZLOG(LOG_INFO, MODULE, "Parser is now stoped");
}

/*--------------------------------------------------*/

void Parser_Destroy(Parser **_parser)
{
    if(!_parser || !(*_parser))
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return;
    }
    free((*_parser)->m_threads);
    pthread_cond_destroy(&(*_parser)->m_cond);
	pthread_mutex_destroy(&(*_parser)->m_mutex);
    free(*_parser);
    *_parser = NULL;
    ZLOG(LOG_INFO, MODULE, "Parser destroy done");
}

/*---------------------------------------------------------*/

static CallType GetCallType(char *strType)
{
    if(0 == strcmp(strType, "MOC")) return MOC;
    if(0 == strcmp(strType, "MTC")) return MTC;
    if(0 ==  strcmp(strType, "SMS-MO")) return SMS_MO;
    if(0 == strcmp(strType, "SMS-MT")) return SMS_MT;
    if(0 == strcmp(strType, "GPRS")) return GPRS;
    return IN_VALID;
}

/*---------------------------------------------------------*/

static void Parser_CreateCdr(char* _line, Cdr *_cdr)
{
    char *saveptr;
    const char *delim = "|";
 
	strcpy(_cdr->m_IMSI, strtok_r(_line, delim, &saveptr));
	strcpy(_cdr->m_MSISDN, strtok_r(NULL, delim, &saveptr));
	strcpy(_cdr->m_IMEI, strtok_r(NULL, delim, &saveptr));

	strcpy(_cdr->m_operatorName, strtok_r(NULL, delim, &saveptr));

	_cdr->m_operatorMCC = strtol(strtok_r(NULL, delim, &saveptr), NULL, INT_BASE);

    _cdr->m_callType = GetCallType(strtok_r(NULL, delim, &saveptr)); 

    strcpy(_cdr->m_callDate, strtok_r(NULL, delim, &saveptr));
	strcpy(_cdr->m_callTime, strtok_r(NULL, delim, &saveptr));
	strcpy(_cdr->m_partyMSISDN, strtok_r(NULL, delim, &saveptr));
	strcpy(_cdr->m_partyOperator, strtok_r(NULL, delim, &saveptr));

    _cdr->m_callDuration = strtol(strtok_r(NULL, delim, &saveptr), NULL, INT_BASE); 
	_cdr->m_mbDownload = strtol(strtok_r(NULL, delim, &saveptr), NULL, INT_BASE);
	_cdr->m_mbUpload = strtol(strtok_r(NULL, delim, &saveptr), NULL, INT_BASE);
}
/*---------------------------------------------------------*/

void Parser_Pause(Parser *_parser)
{
    pthread_mutex_lock(&_parser->m_mutex);
    if(_parser->m_isRunning)
    {
        _parser->m_isRunning = FALSE;
    }
    pthread_mutex_unlock(&_parser->m_mutex);
    ZLOG(LOG_INFO, MODULE, "Parser is now paused");
}
/*--------------------------------------------------*/
Parser_Result Parser_Resume(Parser *_parser)
{
     pthread_mutex_lock(&_parser->m_mutex);
    if(!_parser->m_isRunning)
    {
        _parser->m_isRunning = TRUE;
        pthread_cond_broadcast(&_parser->m_cond);
    }
    pthread_mutex_unlock(&_parser->m_mutex);
    ZLOG(LOG_INFO, MODULE, "Parser is now resumed");
    return PARSER_SUCCESS;
}

/*--------------------------------------------------*/
static int ShouldRun(Parser *_parser)
{
    pthread_mutex_lock(&_parser->m_mutex);  
    if(!_parser->m_isRunning)
    {
        pthread_cond_wait(&_parser->m_cond, &_parser->m_mutex);
    }
    pthread_mutex_unlock(&_parser->m_mutex);
    return 1;    
}
/*--------------------------------------------------*/
static void *Parser_StoreCdr(void *_parser)
{
    Cdr cdr;
    Read_Result res;
    char Pline[MAX_LINE_SIZE];
    Parser *parser = _parser;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); 
    while(ShouldRun(parser))
    {
        res = Read_GetLine(parser->m_read, Pline);
	    if(res == READ_SUCCESS)
        {
            ZLOG(LOG_TRACE, MODULE, "Parser creating and storing cdr");
            Parser_CreateCdr(Pline, &cdr); 
            Aggregate_InsertCdrData(parser->m_aggregate, &cdr);
        }
    } 
    return NULL; /* thread work func need to return void* */ 
}

/*---------------------------------------------------------*/

Parser_Result Parser_Run(Parser *_parser)
{
    if (!_parser)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return PARSER_UNINITIALIZED;
    }
    _parser->m_isRunning = TRUE;
    if(THREAD_SUCCESS != Create_Threads(_parser->m_threads, _parser->m_threadsNum, Parser_StoreCdr, _parser))
    {
        ZLOG(LOG_SEVERE, MODULE, "Failed to create threads");
        return PARSER_THREAD_FAIL;
    }
    ZLOG(LOG_INFO, MODULE, "Parser is now running");
    return PARSER_SUCCESS;
}   

/*---------------------------------------------------------*/
void Parser_PrintCdr(Cdr *_cdr)
{
	printf( "m_IMSI is %s \n", _cdr->m_IMSI);
	printf( "m_MSISDN is %s \n", _cdr->m_MSISDN);
	printf( "m_IMEI is %s \n", _cdr->m_IMEI);
	printf( "m_operatorName is %s \n", _cdr->m_operatorName);
	printf( "m_operatorMCC is %d \n", _cdr->m_operatorMCC);
	printf( "m_callType is %d \n", _cdr->m_callType);
	printf( "m_callDate is %s \n", _cdr->m_callDate);
	printf( "m_callTime is %s \n", _cdr-> m_callTime);
	printf( "m_partyMSISDN is %s \n", _cdr->m_partyMSISDN);
	printf( "m_partyOperator is %s \n", _cdr-> m_partyOperator);
	printf( "m_callDuration is %d \n", _cdr->m_callDuration);
	printf( "m_mbDownload is %d \n", _cdr->m_mbDownload);
	printf( "m_mbUpload is %d \n", _cdr->m_mbUpload);
	printf( "--------------------------------------\n");
}





