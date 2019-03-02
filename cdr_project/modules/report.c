#include "report.h"
#include "thread.h"
#include "waitable_queue.h"
#include "aggregate.h"
#include "zlog4.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h> /* mkdir */

#define TRUE 1
#define FALSE 0

#define MAX_FILE_NAME 50

#define MODULE "report"

typedef struct Report_Request
{
    Report_Type m_type;
}Report_Request;

struct Report
{
    Queue *m_requestsQ;
    Aggregate *m_aggregate; 
    char *m_outputDir;
    pthread_t *m_threads;
    size_t m_threadsNum;
    int m_isRunning;
    pthread_cond_t m_cond;
   	pthread_mutex_t m_mutex;
};

Report *Report_Create(Aggregate *_aggregate, char *_outputDirectory, size_t _reportsCapacity, size_t _reportThreadNum)
{
    int res;
    Report *report = malloc(sizeof(Report));
    if(!report)
    {
        ZLOG(LOG_ERROR, MODULE, "Report failed to allocate memory");
        return NULL;
    }
    report->m_outputDir = _outputDirectory;
    report->m_aggregate = _aggregate;
    report->m_threadsNum = _reportThreadNum;
    report->m_requestsQ = Queue_Create(_reportsCapacity);
    if(!report->m_requestsQ)
    {
        free(report);
        ZLOG(LOG_ERROR, MODULE, "Report failed to create files queue");
        return NULL;
    }
    report->m_threads = malloc(_reportThreadNum * sizeof(pthread_t));
    if(!report->m_threads)
    {
        free(report->m_requestsQ);
        free(report);
        ZLOG(LOG_ERROR, MODULE, "Report failed to allocate memory for threads");
        return NULL;
    }
    res = mkdir(_outputDirectory, 0777);
    if (0==res)
    {
        ZLOG(LOG_TRACE, MODULE, "Output dir %s was created\n", _outputDirectory);
    }
    pthread_mutex_init(&report->m_mutex, NULL);
	pthread_cond_init(&report->m_cond,NULL);
    report->m_isRunning = FALSE;
    ZLOG(LOG_INFO, MODULE, "Report created");
    return report;
}

/*--------------------------Reports-------------------------*/

static int ProduceSubscriberFormat(const char *_subscriberID, SubscriberInfo *_subscriberInfo, FILE *_Pfile)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(_Pfile, "%d-%d-%d, %d:%d:%d\n", tm.tm_year + 2000, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    
    fprintf(_Pfile, "%s, ", _subscriberID); /* imsi */
    fprintf(_Pfile, "%d, ", _subscriberInfo->m_calls[OUTGOING][OPERATOR_SAME]);
    fprintf(_Pfile, "%d, ", _subscriberInfo->m_calls[INCOMING][OPERATOR_SAME]);
    fprintf(_Pfile, "%d, ", _subscriberInfo->m_calls[OUTGOING][OPERATOR_OTHER]);
    fprintf(_Pfile, "%d, ", _subscriberInfo->m_calls[INCOMING][OPERATOR_OTHER]);
    fprintf(_Pfile, "%d, ", _subscriberInfo->m_sms[INCOMING][OPERATOR_SAME]);
    fprintf(_Pfile, "%d, ", _subscriberInfo->m_sms[OUTGOING][OPERATOR_SAME]);
    fprintf(_Pfile, "%d, ", _subscriberInfo->m_sms[INCOMING][OPERATOR_OTHER]);
    fprintf(_Pfile, "%d, ", _subscriberInfo->m_sms [OUTGOING][OPERATOR_OTHER]);
    fprintf(_Pfile, "%d, ", _subscriberInfo->m_internetMbs[INCOMING]);
    fprintf(_Pfile, "%d\n", _subscriberInfo->m_internetMbs[OUTGOING]);
    return 1;
}

/*-------------------------------------------------------------*/

static int ProduceOperatorFormat(const char *_operatorID, OperatorInfo *_operatorInfo, FILE *_Pfile)
{
	time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(_Pfile, "%d-%d-%d, %d:%d:%d\n", tm.tm_year + 2000, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    
    fprintf(_Pfile, "%d, ", _operatorInfo->m_mccMnc);
    fprintf(_Pfile, "%s, ", _operatorInfo->m_brandName);
    fprintf(_Pfile, "%d, ", _operatorInfo->m_calls[INCOMING]);
    fprintf(_Pfile, "%d, ", _operatorInfo->m_calls[OUTGOING]);
    fprintf(_Pfile, "%d, ", _operatorInfo->m_sms[INCOMING]);
    fprintf(_Pfile, "%d\n", _operatorInfo->m_sms[OUTGOING]);
    return 1;
}

/*-------------------------------------------------------------*/
static int GenericSubscriberFormat(const void *_subscriberID, void *_subscriberInfo, void *_Pfile)
{
	 return ProduceSubscriberFormat((char*)_subscriberID, (SubscriberInfo*)_subscriberInfo, (FILE*)_Pfile);
}

/*-------------------------------------------------------------*/
static int GenericOperatorFormat(const void *_operatorID, void *_operatorInfo, void *_Pfile)
{
	 return ProduceOperatorFormat((char*)_operatorID, (OperatorInfo*)_operatorInfo, (FILE*)_Pfile);
}

/*-------------------------------------------------------------*/
static Report_Result Operator_Report(Aggregate *_aggregate, const char *_id)
{
    OperatorInfo *operatorInfo;
    Aggregate_Result res = Aggregate_RetrieveOperatorData(_aggregate, _id, &operatorInfo);
    if (AGGREGATE_SUCCESS != res)
    {
        ZLOG(LOG_TRACE, MODULE, "Operator %s does not exist!", _id);
        return REPORT_NOT_EXIST;
    }
    ZLOG(LOG_TRACE, MODULE, "Generating operator report");
    ProduceOperatorFormat(_id, operatorInfo, stdout);
    return REPORT_SUCCESS;
}

/*-------------------------------------------------------------*/
static Report_Result Subscriber_Report(Aggregate *_aggregate, const char *_id)
{
    SubscriberInfo *subscriberInfo;
    Aggregate_Result res = Aggregate_RetrieveSubscriberData(_aggregate, _id, &subscriberInfo);
    if (AGGREGATE_SUCCESS != res)
    {
        ZLOG(LOG_TRACE, MODULE, "Subscriber %s does not exist!\n", _id);
        return REPORT_NOT_EXIST;
    }
    ZLOG(LOG_TRACE, MODULE, "Generating subscriber report");
    ProduceSubscriberFormat(_id, subscriberInfo, stdout); 
    return REPORT_SUCCESS;
}

/*-----------------------------------------------------------*/
Report_Result Report_AddRequest(Report*_report, Report_Type type, const char *_identify)
{
    Report_Request *request;
    if(!_report)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return REPORT_UNINITIALIZED;
    }
    switch(type)
    {
    case REPORT_CGF_MSISDN:
        return Subscriber_Report(_report->m_aggregate, _identify);
    case REPORT_CGF_OPERATOR:
        return Operator_Report(_report->m_aggregate, _identify);
    case REPORT_CGF_ALL_SUBSCRIBERS:
    case REPORT_CGF_ALL_OPERATORS:
    case REPORT_CGF_AGGRAGATED_INFO:
        break;
    default:
        ZLOG(LOG_ERROR, MODULE, "Report request type filed");
        return REPORT_STORE_FAIL;
    }
    request = malloc(sizeof(Report_Request));
    if(!request)
    {
        ZLOG(LOG_ERROR, MODULE, "Report failed to allocate request memory");
        return REPORT_STORE_FAIL;
    }
    request->m_type = type;

	ZLOG(LOG_TRACE, MODULE, "Report enqueue request");
    if(QUEUE_SUCCESS != Queue_Enqueue(_report->m_requestsQ, request))
    {
        free(request);
        ZLOG(LOG_ERROR, MODULE, "Report failed to enqueue request");
        return REPORT_STORE_FAIL;
    }
    ZLOG(LOG_TRACE, MODULE, "Report added request");
    return REPORT_SUCCESS;
}

/*-------------------------------------------------------------*/
static void AllOperators_FileReport(Aggregate *_aggregate, char *_outPutDir)
{
    FILE * fp;
    char pathname[MAX_FILE_NAME];
    /* make diff file name */
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(pathname, "%sOperators_Report_%d_%d.txt", _outPutDir, tm.tm_hour, tm.tm_min);
    fp = fopen(pathname, "w");
    ZLOG(LOG_TRACE, MODULE, "Generating all operators report named %s", pathname);
    Aggregate_RetrieveAllOperators(_aggregate, GenericOperatorFormat, fp); 
    fclose(fp);
}
/*-------------------------------------------------------------*/
static void AllSubscribers_FileReport(Aggregate *_aggregate, char *_outPutDir)
{
    FILE * fp;
    char pathname[MAX_FILE_NAME];
    /* make diff file name */
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(pathname, "%sSubscribers_Report_%d_%d.txt", _outPutDir, tm.tm_hour, tm.tm_min);
    fp = fopen(pathname, "w");
    ZLOG(LOG_TRACE, MODULE, "Generating all subscribers report named %s", pathname);
    Aggregate_RetrieveAllSubscribers(_aggregate, GenericSubscriberFormat, fp); 
    fclose(fp);
}
/*-------------------------------------------------------------*/

void Report_Pause(Report *_report)
{
    pthread_mutex_lock(&_report->m_mutex);
    if(_report->m_isRunning)
    {
        _report->m_isRunning = FALSE;
    }
    pthread_mutex_unlock(&_report->m_mutex);
	ZLOG(LOG_INFO, MODULE, "Report is now stoped");
}
/*--------------------------------------------------*/
Report_Result Report_Resume(Report *_report)
{
     pthread_mutex_lock(&_report->m_mutex);
    if(!_report->m_isRunning)
    {
        _report->m_isRunning = TRUE;
        pthread_cond_broadcast(&_report->m_cond);
    }
    pthread_mutex_unlock(&_report->m_mutex);
    ZLOG(LOG_INFO, MODULE, "Report is now resumed");
    return REPORT_SUCCESS;
}

/*--------------------------------------------------*/
static int ShouldRun(Report *_report)
{
    pthread_mutex_lock(&_report->m_mutex);  
    if(!_report->m_isRunning)
    {
        pthread_cond_wait(&_report->m_cond, &_report->m_mutex);
    }
    pthread_mutex_unlock(&_report->m_mutex);
    return 1;    
}
/*--------------------------------------------------*/
static void* Report_Generate(void *_report)
{
    Report *report = (Report*)_report;
    Report_Request *request;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); 
    while(ShouldRun(report))
    {
        /* threads waiting on the Queue */
        if(QUEUE_SUCCESS == Queue_Dequeue(report->m_requestsQ, (void**)&request)) 
        {          
            ZLOG(LOG_TRACE, MODULE, "Report got new request");
            char *outputDir = report->m_outputDir;
            switch(request->m_type)
            {
                case REPORT_CGF_MSISDN:
                case REPORT_CGF_OPERATOR:
                    ZLOG(LOG_ERROR, MODULE, "Report of single operator/subscriber should be synchronous");
                    break;  
                case REPORT_CGF_ALL_SUBSCRIBERS:
                    AllSubscribers_FileReport(report->m_aggregate, outputDir);
                    break;                  
                case REPORT_CGF_ALL_OPERATORS:
                    AllOperators_FileReport(report->m_aggregate, outputDir);            
                    break;                  
                case  REPORT_CGF_AGGRAGATED_INFO:
                    AllSubscribers_FileReport(report->m_aggregate, outputDir);
                    AllOperators_FileReport(report->m_aggregate, outputDir);            
                    break;                  
                default:
                    break;
            }
            free(request);
        }
    } 
    ZLOG(LOG_TRACE, MODULE, "Report generated");
    return NULL;           
}

/*--------------------------------------------------*/

Report_Result Report_GenerateFinal(Report *_report)
{
    if (!_report)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return REPORT_UNINITIALIZED; 
    }
    ZLOG(LOG_TRACE, MODULE, "Generating final reports");
    AllSubscribers_FileReport(_report->m_aggregate, _report->m_outputDir);
    AllOperators_FileReport(_report->m_aggregate, _report->m_outputDir);
    return REPORT_SUCCESS;
}

/*--------------------------------------------------*/

Report_Result Report_Run(Report *_report)
{
    if (!_report)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return REPORT_UNINITIALIZED; 
    }
    _report->m_isRunning = TRUE;
    if(THREAD_SUCCESS != Create_Threads(_report->m_threads, _report->m_threadsNum, Report_Generate, _report))
    {
        ZLOG(LOG_SEVERE, MODULE, "Failed to create threads");
        return REPORT_THREAD_FAIL;
    }
    ZLOG(LOG_INFO, MODULE, "Report is now running");
    return REPORT_SUCCESS;
}

/*-----------------------------------------------------------*/

void Report_Stop(Report *_report)
{
    if(!_report)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return;
    }
    Cancel_Threads(_report->m_threadsNum, _report->m_threads);
    Join_Threads(_report->m_threadsNum, _report->m_threads);
    ZLOG(LOG_INFO, MODULE, "Report is now stoped");
}

/*--------------------------------------------------*/
static void ElementDestroy(void *_reportRequest)
{
    free((Report_Request*)_reportRequest); 
}

/*--------------------------------------------------*/

void Report_Destroy(Report **_report)
{
    if(!_report || !(*_report))
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return;
    }
    free((*_report)->m_threads);
    Queue_Destroy(&((*_report)->m_requestsQ), ElementDestroy);
    pthread_cond_destroy(&(*_report)->m_cond);
	pthread_mutex_destroy(&(*_report)->m_mutex);
    free(*_report);
    *_report = NULL;
    ZLOG(LOG_INFO, MODULE, "Report destroy done");
}
/*--------------------------------------------------*/

