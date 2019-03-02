#ifndef __LOGGER_H__
#define __LOGGER_H__

typedef enum {
    LOG_TRACE,       /* Trace message usually very detailed */
    LOG_DEBUG,       /* A message useful for debugging */
    LOG_INFO,        /* Informative message */ 
    LOG_WARNING,     /* Warning */
    LOG_ERROR,       /* An error occurred */ 
    LOG_CRITICAL,    /* Critical error */
    LOG_SEVERE,      /* Server error requiring immediate intervention */
    LOG_FATAL,       /* Fatal error signaling an emergency */ 
    LOG_NONE,        /* Used only in configuration file */
    ERROR_INVALID
} Severity;


#ifdef DEBUG

#define ZLOG(SEVERITY, MODULE, MESSAGE, ...) (Zlog_Write(SEVERITY,MODULE,__func__,__FILE__,__LINE__,MESSAGE, ##__VA_ARGS__)) 

int Zlog_Write(Severity _level,const char* _module, const char* _func,const char* _file, int _line,const char* _message, ...);
				
#else

#define ZLOG(SEVERITY, MODULE, MESSAGE, ...) (Zlog_Write(SEVERITY,MODULE,MESSAGE, ##__VA_ARGS__)) 
int Zlog_Write(Severity _level,const char* _module,const char* _message, ...);

#endif

int Zlog_Init(const char* _filename);

#endif
