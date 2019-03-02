#include "manager.h"
#include "report.h"
#include "read.h"
#include "detect.h"
#include "parser.h"
#include "configuration.h"
#include "aggregate.h"
#include "ui.h"
#include "thread.h"
#include "zlog4.h"
#include <string.h>
#include <errno.h>

#define LOG_CONFIG_FILE "log_config"
#define CONFIG_FILE "configs"
#define MODULE "manager"

#define TRUE 1
#define FALSE 0

static Manager *Manager_Create(char *_configFile);
static Manager_Result Manager_Run(Manager *_manager);
static void Manager_Stop(Manager *_manager);
static void Manager_Destroy(Manager **_manager);

static Manager *sigManager; 

struct Manager
{
    Configuration *m_config;
    Report *m_report;
    Aggregate *m_aggregate;
    Parser *m_parser;
    Read *m_read;
    Detect *m_detect;
    UI *m_ui;
    int m_running;
    pthread_mutex_t m_mutex;
    sig_atomic_t subSignal; 
    sig_atomic_t opSignal;
    struct sigaction saSub;
    struct sigaction saOp;
};


/*----------------------------------------------*/
static void SIG_SubscriberReport(int _signal, siginfo_t *_info, void *_context)
{
    ZLOG(LOG_TRACE, MODULE, "Got signal1 - generating subscribers report file");
    Report_AddRequest(sigManager->m_report, REPORT_CGF_ALL_SUBSCRIBERS, NULL);
}
/*----------------------------------------------*/
static void SIG_OperatorReport(int _signal, siginfo_t *_info, void *_context)
{
    ZLOG(LOG_TRACE, MODULE, "Got signal2 - generating operators report file");
    Report_AddRequest(sigManager->m_report, REPORT_CGF_ALL_OPERATORS, NULL);
}
/*----------------------------------------------*/
static Manager_Result RegisterSignals(Manager *_manager)
{
    _manager->saSub.sa_sigaction = SIG_SubscriberReport;
    _manager->saSub.sa_flags = 0;
    sigemptyset(&_manager->saSub.sa_mask);

    _manager->saOp.sa_sigaction = SIG_OperatorReport;
    _manager->saOp.sa_flags = 0;
    sigemptyset(&_manager->saOp.sa_mask);
    
    if (sigaction(SIGUSR1, &_manager->saSub, NULL) == -1)
    {
        ZLOG(LOG_ERROR, MODULE, "Failed to register SIGUSR1, errno is %s", strerror(errno));
        return MANAGER_SIGNAL_FAIL;
    }
    if (sigaction(SIGUSR2, &_manager->saOp, NULL) == -1)
    {
        ZLOG(LOG_ERROR, MODULE, "Failed to register SIGUSR2, errno is %s", strerror(errno));
        return MANAGER_SIGNAL_FAIL;
    }
    return MANAGER_SUCCESS;
 }

/*----------------------------------------------*/
int System_Run(char *_configFile)
{
    Manager_Result res;
    Manager *manager = Manager_Create(_configFile);
    if(!manager)
    {
        ZLOG(LOG_FATAL, MODULE, "System create failed, cannot allocate memory");
        return 1;
    }
    sigManager = manager;
    res = RegisterSignals(manager);
    if(res != MANAGER_SUCCESS)
    {
        ZLOG(LOG_FATAL, MODULE, "System create failed, cannot register signals");
        return 1;
    }
    res = Manager_Run(manager);
    if(res != MANAGER_SUCCESS)
    {
        ZLOG(LOG_FATAL, MODULE, "System create failed, manager failed to run");
        return 1;
    }
    ZLOG(LOG_INFO, MODULE, "System started successfuly");
    Ui_DoneRunning(manager->m_ui); /* <= join ui */
    ZLOG(LOG_INFO, MODULE, "System shutting down");
    Manager_Shutdown(&manager);
    return 0;
}
/*----------------------------------------------*/
int main()
{   
    Zlog_Init(LOG_CONFIG_FILE);
    return System_Run(CONFIG_FILE);
}
/*----------------------------------------------*/
static Manager_Result Manager_Run(Manager *_manager)
{
    _manager->m_running = TRUE;

    ZLOG(LOG_INFO, MODULE, "Creating threads for all modules");
    if(DETECT_SUCCESS !=  Detect_Run(_manager->m_detect))
    {
        ZLOG(LOG_SEVERE, MODULE, "Detect run failed");
        return MANAGER_RUN_FAIL;
    }
    if(READ_SUCCESS !=  Read_Run(_manager->m_read))
    {
        ZLOG(LOG_SEVERE, MODULE, "Read run failed");
        return MANAGER_RUN_FAIL;
    }
    if(PARSER_SUCCESS != Parser_Run(_manager->m_parser))
    {
        ZLOG(LOG_SEVERE, MODULE, "Parser run failed");
        return MANAGER_RUN_FAIL;
    }
    if(REPORT_SUCCESS != Report_Run(_manager->m_report))
    {
        ZLOG(LOG_SEVERE, MODULE, "Report run failed");
        return MANAGER_RUN_FAIL;
    }
    if(UI_SUCCESS != Ui_Run(_manager->m_ui))
    {
        ZLOG(LOG_SEVERE, MODULE, "UI run failed");
        return MANAGER_RUN_FAIL;
    }
    ZLOG(LOG_INFO, MODULE, "All threads created successfully");
    return MANAGER_SUCCESS;
}

/*----------------------------------------------*/
static void Manager_Stop(Manager *_manager)
{
    ZLOG(LOG_INFO, MODULE, "Stopping system");
    Report_Stop(_manager->m_report); 
    Parser_Stop(_manager->m_parser);
    Read_Stop(_manager->m_read);
    Detect_Stop(_manager->m_detect);
    ZLOG(LOG_INFO, MODULE, "System is now stopped");
}

/*----------------------------------------------*/
static void Manager_Destroy(Manager **_manager)
{
    Ui_Destroy(&(*_manager)->m_ui);
    Report_Destroy(&(*_manager)->m_report);
    Parser_Destroy(&(*_manager)->m_parser);
    Read_Destroy(&(*_manager)->m_read);
    Detect_Destroy(&(*_manager)->m_detect);
    Aggregate_Destroy(&(*_manager)->m_aggregate);
    Configuration_Destroy(&(*_manager)->m_config);
    pthread_mutex_destroy(&(*_manager)->m_mutex);
    free(*_manager);
    *_manager = NULL;
}
/*----------------------------------------------*/


void Manager_Shutdown(Manager **_manager)
{
    Report_Result res;
    if (!_manager || !(*_manager))
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return;
    }
    res = Report_GenerateFinal((*_manager)->m_report);
    if (REPORT_SUCCESS != res)
    {
        ZLOG(LOG_ERROR, MODULE, "Failed to generate final report");
    }
    Manager_Stop(*_manager);
    Manager_Destroy(_manager);
    ZLOG(LOG_INFO, MODULE, "Manager is shutting down, bye bye!");
}
/*----------------------------------------------*/

Manager_Result Manager_Pause(Manager *_manager)
{
    ZLOG(LOG_INFO, MODULE, "Pausing system");
    if (!_manager)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return MANAGER_UNINITIALIZED;
    }
    
    pthread_mutex_lock(&_manager->m_mutex);
    if(_manager->m_running)
    {
        _manager->m_running = FALSE;
        Report_Pause(_manager->m_report); 
        Parser_Pause(_manager->m_parser);
        Read_Pause(_manager->m_read);
        Detect_Pause(_manager->m_detect);
    }
    else
    {
        ZLOG(LOG_WARNING, MODULE, "System is already paused");
    }
    pthread_mutex_unlock(&_manager->m_mutex);
    ZLOG(LOG_INFO, MODULE, "System is now paused");
    return MANAGER_SUCCESS;
}

/*----------------------------------------------*/

Manager_Result Manager_Resume(Manager *_manager)
{
    ZLOG(LOG_INFO, MODULE, "Resuming system");
    if (!_manager)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return MANAGER_UNINITIALIZED;
    }
    
    pthread_mutex_lock(&_manager->m_mutex);
    if(!_manager->m_running)
    {
        _manager->m_running = TRUE;
        if(DETECT_SUCCESS !=  Detect_Resume(_manager->m_detect))
        {
            return MANAGER_RUN_FAIL;
        }
        if(READ_SUCCESS != Read_Resume(_manager->m_read))
        {
            return MANAGER_RUN_FAIL;
        }
        if(PARSER_SUCCESS !=  Parser_Resume(_manager->m_parser))
        {
            return MANAGER_RUN_FAIL;
        }
        if(REPORT_SUCCESS !=  Report_Resume(_manager->m_report))
        {
            return MANAGER_RUN_FAIL;
        }
    }
    else
    {
        ZLOG(LOG_WARNING, MODULE, "Resume? system is already running");
    }
    pthread_mutex_unlock(&_manager->m_mutex);
    ZLOG(LOG_INFO, MODULE, "System is now resumed");
    return MANAGER_SUCCESS;
}

/*----------------------------------------------*/
static Manager *Manager_Create(char *_configFile)
{
    size_t filesQueueSize, linesQueueSize, detectThreadNum, readThreadNum, parserThreadNum, uiThreadNum, reportThreadNum, reportsCapacity, operatorHashCapacity, SubscriberHashCapacity;
    char *cdrFileMask, *inputDirectory, *inProgressDirectory, *doneDirectory, *outputDirectory;
    Manager *manager = malloc(sizeof(Manager));
    if (!manager)
    {
        ZLOG(LOG_SEVERE, MODULE, "Failed to allocate memory for manager");
        goto FREE_MANAGER;
    }
    manager->m_running = FALSE;
    pthread_mutex_init(&manager->m_mutex, NULL);
    /* config */
    manager->m_config = Configuration_Create(_configFile);
    if (!manager->m_config)
    {
        ZLOG(LOG_SEVERE, MODULE, "Failed to create configuration");
        goto FREE_CONFIG;
    }
    /* detect */
    filesQueueSize = Configuration_GetFileQueueSize(manager->m_config);
    detectThreadNum = Configuration_GetDetectThreadsNum(manager->m_config);
    cdrFileMask = Configuration_GetCdrMask(manager->m_config);
    inputDirectory = Configuration_GetInputDir(manager->m_config);
    inProgressDirectory = Configuration_GetProgressDir(manager->m_config);
    manager->m_detect = Detect_Create(filesQueueSize, detectThreadNum, cdrFileMask, inputDirectory, inProgressDirectory);
    if (!manager->m_detect)
    {
       ZLOG(LOG_SEVERE, MODULE, "Failed to create file detector");
       goto FREE_DETECT;
    }
    /* read */
    linesQueueSize = Configuration_GetLineQueueSize(manager->m_config);
    readThreadNum = Configuration_GetReadThreadsNum(manager->m_config); 
    doneDirectory = Configuration_GetDoneDir(manager->m_config); 
    manager->m_read = Read_Create(linesQueueSize, readThreadNum, doneDirectory, manager->m_detect);
    if (!manager->m_read)
    {
        ZLOG(LOG_SEVERE, MODULE, "Failed to create readers");
		goto FREE_READ;
    }
    /* aggregate */
    operatorHashCapacity = Configuration_GetOperatorHashSize(manager->m_config);
    SubscriberHashCapacity = Configuration_GetSubscriberHashSize(manager->m_config);
    manager->m_aggregate = Aggregate_Create(SubscriberHashCapacity, operatorHashCapacity);
    if (!manager->m_aggregate)
    {
        ZLOG(LOG_SEVERE, MODULE, "Failed to create data aggregator");
		goto FREE_AGGREGATE;
    }
    /* parser */
    parserThreadNum = Configuration_GetParserThreadsNum(manager->m_config);
    manager->m_parser = Parser_Create(parserThreadNum, manager->m_read, manager->m_aggregate);
    if (!manager->m_parser)
    {
        ZLOG(LOG_SEVERE, MODULE, "Failed to create parsers");
		goto FREE_PARSER;
    }
    /* report */
    outputDirectory = Configuration_GetOutputDir(manager->m_config);
    reportsCapacity = Configuration_GetReportQueueSize(manager->m_config);
    reportThreadNum = Configuration_GetReportThreadsNum(manager->m_config); 
    manager->m_report = Report_Create(manager->m_aggregate, outputDirectory, reportsCapacity, reportThreadNum);
    if(!manager->m_report)
    {
        ZLOG(LOG_SEVERE, MODULE, "Failed to create reporters");
        goto FREE_REPORT;
    }
    /* ui */
    uiThreadNum = Configuration_GetUiThreadsNum(manager->m_config); 
    manager->m_ui = Ui_Create(uiThreadNum, manager->m_report, manager);
    if(!manager->m_ui)
    {
        ZLOG(LOG_SEVERE, MODULE, "Failed to create UI");
		goto FREE_UI;
    }
    return manager;
    
    FREE_UI:
    Ui_Destroy(&manager->m_ui);
    FREE_REPORT:
    Report_Destroy(&manager->m_report);
    FREE_AGGREGATE:
    Aggregate_Destroy(&manager->m_aggregate);
    FREE_PARSER:
    Parser_Destroy(&manager->m_parser);
    FREE_READ:
    Read_Destroy(&manager->m_read);
    FREE_DETECT:
    Detect_Destroy(&manager->m_detect);
    FREE_CONFIG:
    Configuration_Destroy(&manager->m_config);
    FREE_MANAGER:
    free(manager);
    return NULL;
}


    
    
    
    
    
    
    


