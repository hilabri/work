#ifndef __UI_H__
#define __UI_H__

typedef struct UI UI;

typedef enum UI_Result
{
    UI_SUCCESS,
    UI_UNINITIALIZED,
    UI_THREAD_FAIL,
    UI_GET_FAIL
}UI_Result;


UI *Ui_Create(int uiThreadNum, Report *_report, Manager *_manger);
void Ui_Destroy(UI **_ui);

UI_Result Ui_Run(UI *_ui);

/* join ui */
void Ui_DoneRunning(UI *_ui);

#endif /* __UI_H__ */
