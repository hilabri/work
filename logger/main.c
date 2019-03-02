#include "zlog4.h"

#define MODULE "my_program"

/* TODO: Add full tests for log library */

int main(int argc, char *argv[])
{
    const char *config_file = argv[1]; 
    int i;

    Zlog_Init(config_file); /* Startup */

    ZLOG(LOG_INFO, MODULE, "Hey %s, welcome to logger test!", "Hila");

    ZLOG(LOG_WARNING, MODULE, "I'm a warning in debug mode");
    ZLOG(LOG_ERROR, MODULE, "I'm just an error string");

    for (i = 1; i <= 10; i++)
    {
        ZLOG(LOG_TRACE, MODULE, "Trace entry %d out of %d", i, 10);
    }
    ZLOG(LOG_DEBUG, MODULE, "Bye %s ", "Hila"); 

    return 0;
}
