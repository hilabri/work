#include "thread.h"
#include "manager.h"
#include "report.h"
#include "ui.h"
#include "zlog4.h"
#include <string.h>

#define TRUE  1
#define FALSE 0

#define MODULE "ui"

struct UI
{
    Manager *m_manager;
    Report *m_report;
    pthread_t *m_threads;
    size_t m_threadsNum;
};

/*------------------------------------------------*/
UI *Ui_Create(int uiThreadNum, Report *_report, Manager *_manager)
{
	 UI *ui = malloc(sizeof(UI));
	 if(!ui)
	 {
	 	ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
	 	return NULL;
	 }
	 ui->m_manager = _manager;
	 ui->m_report =  _report;
	 ui->m_threadsNum = uiThreadNum;
	 ui->m_threads = malloc(uiThreadNum * sizeof(pthread_t));
	 if(!ui->m_threads)
     {
        free(ui);
        ZLOG(LOG_ERROR, MODULE, "Ui falied to allocate memory for threads");
        return NULL;
     }
     ZLOG(LOG_INFO, MODULE, "Ui created");
     return ui;
}
/*-------------------------------------------------*/

void Ui_DoneRunning(UI *_ui)
{
    if(!_ui)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return;
    }
    Join_Threads(_ui->m_threadsNum, _ui->m_threads);
    ZLOG(LOG_INFO, MODULE, "Ui stoped");
}

/*--------------------------------------------------*/
void Ui_TriggerShutdown(UI *_ui)
{
    if(!_ui)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return;
    }
    Cancel_Threads(_ui->m_threadsNum, _ui->m_threads);
    ZLOG(LOG_INFO, MODULE, "Ui requesed to stop");
}
/*--------------------------------------------------*/
void Ui_Destroy(UI **_ui)
{
    if(!_ui || !(*_ui))
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return;
    }
    free((*_ui)->m_threads);
    free(*_ui);
    *_ui = NULL;
    ZLOG(LOG_INFO, MODULE, "Ui destroy done");
}

/*--------------------------------------------------*/

static void *Ui_Requests(void *_ui)
{
    char identify[REPORT_MAX_IDENTIFIER_SIZE];
    Report_Result res;
    unsigned int choice;
    int souldContinue = TRUE;
    UI *ui = _ui;
    while(souldContinue)
    {
    	/* ui requests */
    	printf("\nChoose option: \n");
    	printf("1: Report- a subscriber\n");
    	printf("2: Report- an operator\n");
    	printf("3: Report file- All subscribers\n");
    	printf("4: Report file- All operators\n");
    	printf("5: Report files- All subscriber + All operator\n");
    	/* system controls */
    	printf("6: System Pause\n");
    	printf("7: System Resume\n");
   		printf("8: System Shutdown Procedure\n");
   	 	printf("Any another number - to stop the system \n");
		scanf("%u", &choice);
		
		ZLOG(LOG_TRACE, MODULE, "Ui got user choice %u", choice);
        switch(choice)
        {
        case 1:
            printf("Please enter subscriber identify\n");
            scanf("%s", identify);
            res = Report_AddRequest(ui->m_report, REPORT_CGF_MSISDN, identify);
            if (REPORT_NOT_EXIST == res)
            {
                printf("Sorry, subscriber %s does not exist!\n", identify);
            }
            break;
        case 2:
            printf("Please enter operators identify\n");
            scanf("%s", identify);
            res = Report_AddRequest(ui->m_report, REPORT_CGF_OPERATOR, identify);
            if (REPORT_NOT_EXIST == res)
            {
                printf("Sorry, operator %s does not exist!\n", identify);
            }
            break;
        case 3:
            Report_AddRequest(ui->m_report, REPORT_CGF_ALL_SUBSCRIBERS, NULL);
            break;
        case 4:
            Report_AddRequest(ui->m_report, REPORT_CGF_ALL_OPERATORS, NULL);
            break;
        case 5:
            Report_AddRequest(ui->m_report, REPORT_CGF_AGGRAGATED_INFO, NULL);
            break;
        case 6:
            Manager_Pause(ui->m_manager);
            break;
        case 7:
            Manager_Resume(ui->m_manager);
            break;
        case 8:
            Ui_TriggerShutdown(ui);
            break;
        default:
            souldContinue = FALSE;
            break;
    	}
    }
    ZLOG(LOG_TRACE, MODULE, "Ui responded to user request");
    return NULL;
}
/*-----------------------------------------------------------*/
UI_Result Ui_Run(UI *_ui)
{
    if (!_ui)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return UI_UNINITIALIZED;
    }
    if (THREAD_SUCCESS != Create_Threads(_ui->m_threads, _ui->m_threadsNum, Ui_Requests, _ui))
    {
        ZLOG(LOG_SEVERE, MODULE, "Failed to create threads");
        return UI_THREAD_FAIL;
    }
    ZLOG(LOG_INFO, MODULE, "Ui is now running");
    return UI_SUCCESS;
}

