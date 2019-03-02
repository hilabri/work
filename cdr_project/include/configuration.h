#ifndef __CONFIGIRATION_H__
#define __CONFIGIRATION_H__

#include <stddef.h>

#define MASK_SIZE 10
#define DIR_SIZE 50

#define DEFAULT_CONFIG_FILE        "default_file"

#define DEFAULT_MASK               ".cdr"
#define DEFAULT_INPUT_DIR          "/tmp/cdr/"
#define DEFAULT_PROGRESS_DIR       "/tmp/in_progress/"
#define DEFAULT_DONE_DIR           "/tmp/done/"
#define DEFAULT_OUT_DIR            "/tmp/billing/"
#define DEFAULT_DETECTS            4
#define DEFAULT_READERS            10 /* files concurrency */
#define DEFAULT_PARSERS            1000 /* lines concurrency */
#define DEFAULT_REPORTERS          3
#define DEFAULT_UI                 1

#define DEFAULT_FILE_QUEUE_SIZE    20
#define DEFAULT_LINE_QUEUE_SIZE    10000  
#define DEFAULT_SUBSCRIBER_HASH_SIZE 10000
#define DEFAULT_OPERATOR_HASH_SIZE 20
#define DEFAULT_REPORTS_QUEUE_SIZE 10

typedef struct Configuration  Configuration;


Configuration* Configuration_Create(char *_configFile);
void Configuration_Destroy(Configuration* *_config); 

/* Getters: */
char *Configuration_GetCdrMask(Configuration *_config);
char *Configuration_GetInputDir(Configuration *_config);
char *Configuration_GetProgressDir(Configuration *_config);

char *Configuration_GetDoneDir(Configuration *_config);
char *Configuration_GetOutputDir(Configuration *_config);

size_t Configuration_GetDetectThreadsNum(Configuration *_config);
size_t Configuration_GetReadThreadsNum(Configuration *_config);
size_t Configuration_GetParserThreadsNum(Configuration *_config);
size_t Configuration_GetReportThreadsNum(Configuration *_config);
size_t Configuration_GetUiThreadsNum(Configuration *_config);

size_t Configuration_GetFileQueueSize(Configuration *_config);
size_t Configuration_GetLineQueueSize(Configuration *_config);
size_t Configuration_GetSubscriberHashSize(Configuration *_config);
size_t Configuration_GetOperatorHashSize(Configuration *_config);
size_t Configuration_GetReportQueueSize(Configuration *_config);

#endif /* __CONFIGURATION_H__ */
