#include "zlog4.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdarg.h>

#define ERR_OPEN_FILE  -1
#define ERR_SUCCESS 0

#define MAX_SIZE 100


static const char* DEFAULT_CONFIG_FILE = "defaultConfig.log";
static const Severity DEFAULT_LEVEL = LOG_NONE;
static const char* DEFAULT_LOG_FILE = "defaultLog.log";

static struct {
	Severity m_level;
	FILE *m_file;
	pid_t m_pid;
} LogFile;

typedef struct {
	Severity	m_level;
	char*		m_name;
	char		m_levelAbbr;
} Severity_Table;

static const Severity_Table s_sevTable[] = {
    { LOG_TRACE,     "Trace",    'T' },
    { LOG_DEBUG,     "Debug",    'D' },    
    { LOG_INFO,      "info",     'I' },       
    { LOG_WARNING,   "Warning",  'W' },     
    { LOG_ERROR,     "Error",    'E' },       
    { LOG_CRITICAL,  "Critical", 'C' },    
    { LOG_SEVERE,    "Severe",   'S' },      
    { LOG_FATAL,     "Fatal",    'F' },       
    { LOG_NONE,      "None",     'N' },             
	{ ERROR_INVALID,  NULL,     '\0' }
};

static const size_t severityTableSize = sizeof(s_sevTable)/sizeof(Severity_Table);

/*******************Look-up Table Info***********/
/**********************************************/
static Severity NameToSeverity(const char *_levelName)
{
   size_t i;
   for (i = 0; i < (severityTableSize-1); ++i)
   {
   		if (0 == strcmp(_levelName, s_sevTable[i].m_name))
        {
            break;
        }
   }
   return s_sevTable[i].m_level;
}

/**********************************************/
static char SeverityToAbbr(Severity _level)
{
	return s_sevTable[_level].m_levelAbbr;
}
/*************************************************/
static void Zlog_Close()
{
    printf("BYE BYE\n");
    fclose(LogFile.m_file);
}
/*************************************************/

int Zlog_Init(const char *_configFile)
{
    FILE *config;
    int numScanned = 0;
    char arg1[MAX_SIZE] = { 0 };
    char arg2[MAX_SIZE] = { 0 };
    LogFile.m_pid = getpid();
    config = (_configFile) ? (fopen(_configFile, "r")) : (fopen(DEFAULT_CONFIG_FILE, "r"));
	if (config)
 	{
 	   	numScanned = fscanf(config,"%s %s", arg1, arg2);
        fclose(config);
	}
    LogFile.m_level = numScanned > 0 ? NameToSeverity(arg1) : DEFAULT_LEVEL;
    LogFile.m_file = numScanned > 1 ? fopen(arg2, "a") : ((LogFile.m_level == ERROR_INVALID) ? fopen(arg1, "a") : NULL);
    
    if(ERROR_INVALID == LogFile.m_level)
    {
        LogFile.m_level = DEFAULT_LEVEL;
    }
    if (!LogFile.m_file)
    {
        LogFile.m_file = fopen(DEFAULT_LOG_FILE, "a");
        if (!LogFile.m_file)
        {
            return ERR_OPEN_FILE;
        }
    }
    atexit(Zlog_Close);
    return ERR_SUCCESS;
}

/******************************************/
#ifdef DEBUG
int Zlog_Write(Severity _level, const char* _module, const char* _func, const char* _file, int _line, const char* _message, ...)
#else
int Zlog_Write(Severity _level, const char* _module, const char* _message, ...)
#endif
{
    int result = ERR_SUCCESS;
    if (_level >= LogFile.m_level)
    {
        va_list args;
        char info[MAX_SIZE];
        char arg_message[MAX_SIZE];
        const char abbr = SeverityToAbbr(_level);
        const pid_t pid = LogFile.m_pid;
        pthread_t thread = pthread_self();
        
        va_start(args, _message);
        vsprintf(arg_message, _message, args);
        va_end(args);
		
        #ifdef DEBUG
        sprintf(info, "%s@%s:%d", _func, _file, _line);
        #endif
		result = fprintf(LogFile.m_file,"%s %s %d %.4ld %c %s %s %s\n", __DATE__, __TIME__, pid, (long)thread, abbr, _module, info, arg_message);
    }
    return result;
}
/**********************************************/
