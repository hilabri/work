#ifndef __MANAGER_H__
#define __MANAGER_H__

#include <stddef.h> 

typedef struct Manager Manager;

/* System Control: */

typedef enum Manager_Result 
{
    MANAGER_SUCCESS,
    MANAGER_UNINITIALIZED,
    MANAGER_RUN_FAIL,
    MANAGER_SIGNAL_FAIL
} Manager_Result;


Manager_Result Manager_Pause(Manager *_manager);
Manager_Result Manager_Resume(Manager *_manager);
void Manager_Shutdown(Manager **_manager);

int System_Run();

#endif /* __MANAGER_H__ */
