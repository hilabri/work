#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "configuration.h"
#include "zlog4.h"

#define NOT_INIT 0
#define MODULE "configuration"

static void Configuration_SetDefaults(Configuration *_config);
static void ScanFileAndSetConfiguration(FILE *file, Configuration *config);

struct Configuration
{
/* Config file content: */
    char m_inputFileMask[MASK_SIZE]; /* file pattern to read from */
    char m_inputDirectory[DIR_SIZE]; /* directory that contains input files */
    char m_inProgressDirectory[DIR_SIZE]; 
    char m_doneDirectory[DIR_SIZE]; /* directory that contains processed input files */
    char m_outputDirectory[DIR_SIZE]; /* directory to store ouput reports */
	
    size_t m_nDetectThreads;
    size_t m_nReaderThreads;
	size_t m_nParsingThreads;
    size_t m_nReportThreads;
	size_t m_nUiThreads;
	
    size_t m_fileQueueSize;
    size_t m_lineQueueSize;
    size_t m_subscriberHashSize;
    size_t m_operatorHashSize;
    size_t m_requestQueueSize;
};
/*-------------------------------------------------------*/
static void Configuration_SetDefaults(Configuration *_config)
{
    strcpy(_config->m_inputFileMask, DEFAULT_MASK); 
    strcpy(_config->m_inputDirectory, DEFAULT_INPUT_DIR);
    strcpy(_config->m_inProgressDirectory, DEFAULT_PROGRESS_DIR);
    strcpy(_config->m_doneDirectory, DEFAULT_DONE_DIR);
    strcpy(_config->m_outputDirectory, DEFAULT_OUT_DIR);
    
    _config->m_nDetectThreads = DEFAULT_DETECTS;
    _config->m_nReaderThreads = DEFAULT_READERS;
    _config->m_nParsingThreads = DEFAULT_PARSERS;
    _config->m_nReportThreads = DEFAULT_REPORTERS; 
	_config->m_nUiThreads = DEFAULT_UI;
	
    _config->m_fileQueueSize = DEFAULT_FILE_QUEUE_SIZE; 
    _config->m_lineQueueSize = DEFAULT_LINE_QUEUE_SIZE; 
    _config->m_subscriberHashSize = DEFAULT_SUBSCRIBER_HASH_SIZE;
    _config->m_operatorHashSize = DEFAULT_OPERATOR_HASH_SIZE;
    _config->m_requestQueueSize = DEFAULT_REPORTS_QUEUE_SIZE;
}

/*-------------------------------------------------------*/

static void ScanFileAndSetConfiguration(FILE *file, Configuration *config)
{
    int scanVar;
	if(0 == fscanf(file,"mask-%s", config->m_inputFileMask))
    {
        strcpy(config->m_inputFileMask, DEFAULT_MASK);
    }
	if(0 == fscanf(file,"input-%s", config->m_inputDirectory))
    {
        strcpy(config->m_inputDirectory, DEFAULT_INPUT_DIR);
    }
    if(0 == fscanf(file,"progress-%s", config->m_inProgressDirectory))
    {
        strcpy(config->m_inProgressDirectory, DEFAULT_PROGRESS_DIR);
    }
    if(0 == fscanf(file,"done-%s", config->m_doneDirectory))
    {
        strcpy(config->m_doneDirectory, DEFAULT_DONE_DIR);
    }
	if(0 == fscanf(file,"output-%s", config->m_outputDirectory))
    {
        strcpy(config->m_outputDirectory, DEFAULT_OUT_DIR);
    }
   
    config->m_nDetectThreads =      (fscanf(file,"detect-%d", &scanVar)) ? scanVar : DEFAULT_DETECTS;
	config->m_nReaderThreads =      (fscanf(file,"read-%d", &scanVar)) ? scanVar : DEFAULT_READERS; 
	config->m_nParsingThreads =     (fscanf(file,"parser-%d", &scanVar)) ? scanVar : DEFAULT_PARSERS;
    config->m_nReportThreads =      (fscanf(file,"report-%d", &scanVar)) ? scanVar : DEFAULT_REPORTERS;
    config->m_nUiThreads =          (fscanf(file,"ui-%d", &scanVar)) ? scanVar : DEFAULT_UI;

    config->m_fileQueueSize =       (fscanf(file,"file_queue-%d", &scanVar)) ? scanVar : DEFAULT_FILE_QUEUE_SIZE;  
    config->m_lineQueueSize =       (fscanf(file,"line_queue-%d", &scanVar)) ? scanVar : DEFAULT_LINE_QUEUE_SIZE;  
    config->m_subscriberHashSize =  (fscanf(file,"subs_hash-%d", &scanVar)) ? scanVar : DEFAULT_SUBSCRIBER_HASH_SIZE;
    config->m_operatorHashSize =    (fscanf(file,"oper_hash-%d", &scanVar)) ? scanVar : DEFAULT_OPERATOR_HASH_SIZE;
    config->m_requestQueueSize =    (fscanf(file,"request_queue-%d", &scanVar)) ? scanVar : DEFAULT_REPORTS_QUEUE_SIZE;
    
}

/*--------------------------------------------------------------------------*/

Configuration* Configuration_Create(char *_configFile)
{
    FILE *file;
	Configuration *config= malloc(sizeof(Configuration));
	if(!config)
	{
		ZLOG(LOG_ERROR, MODULE, "Configuration failed to allocate memory");
		return NULL;
	}
    file = (_configFile) ? (fopen(_configFile, "r")) : (fopen(DEFAULT_CONFIG_FILE, "r"));
	if (file)
 	{
        ZLOG(LOG_TRACE, MODULE,"Reading configuration file content"); 
        ScanFileAndSetConfiguration(file, config); 
        fclose(file);
    }
    else
    {
        ZLOG(LOG_TRACE, MODULE,"Reading defaults"); 
        Configuration_SetDefaults(config);
    }
    ZLOG(LOG_INFO, MODULE, "Configuration created");
    return config;
}
        
/*--------------------------------------------------------------------------*/

void Configuration_Destroy(Configuration **_config)
{
	if(!_config || !(*_config))
	{
		return;
	}
	free(*_config);
	*_config = NULL;
} 
/*--------------------------------------------------------------------------*/
/*--------------------------------Gets--------------------------------------*/
 
char *Configuration_GetCdrMask(Configuration *_config)
{
	if(!_config)
	{
		return NULL;
	}
    return _config->m_inputFileMask;
}
/*--------------------------------------------------------------------------*/
char *Configuration_GetInputDir(Configuration *_config)
{
	if(!_config)
	{
		return NULL;
	}
    return _config->m_inputDirectory;
}
/*--------------------------------------------------------------------------*/

char *Configuration_GetDoneDir(Configuration *_config)
{
	if(!_config)
	{
		return NULL;
	}
	return _config->m_doneDirectory;
}
/*--------------------------------------------------------------------------*/

char *Configuration_GetProgressDir(Configuration *_config)
{
	if(!_config)
	{
		return NULL;
	}
	return _config->m_inProgressDirectory;
}
/*--------------------------------------------------------------------------*/

char *Configuration_GetOutputDir(Configuration *_config)
{
	if(!_config)
	{
		return NULL;
	}
	return _config->m_outputDirectory;
}
/*--------------------------------------------------------------------------*/
size_t Configuration_GetLineQueueSize(Configuration *_config)
{
	if(!_config)
	{
		return NOT_INIT;
	}
	return _config->m_lineQueueSize;
}
 /*--------------------------------------------------------------------------*/
size_t Configuration_GetFileQueueSize(Configuration *_config)
{
	if(!_config)
	{
		return NOT_INIT;
	}
	return _config->m_fileQueueSize;
}
/*--------------------------------------------------------------------------*/
size_t Configuration_GetSubscriberHashSize(Configuration *_config)
{
	if(!_config)
	{
		return NOT_INIT;
	}
	return _config->m_subscriberHashSize;
}
/*--------------------------------------------------------------------------*/
size_t Configuration_GetOperatorHashSize(Configuration *_config)
{
	if(!_config)
	{
		return NOT_INIT;
	}
	return _config->m_operatorHashSize;
}
/*--------------------------------------------------------------------------*/
size_t Configuration_GetReportQueueSize(Configuration *_config)
{
	if(!_config)
	{
		return NOT_INIT;
	}
	return _config->m_requestQueueSize;
}
/*--------------------------------------------------------------------------*/
size_t Configuration_GetDetectThreadsNum(Configuration *_config)
{
	if(!_config)
	{
		return NOT_INIT;
	}
	return _config->m_nDetectThreads;
}
/*--------------------------------------------------------------------------*/
size_t Configuration_GetReadThreadsNum(Configuration *_config)
{
	if(!_config)
	{
		return NOT_INIT;
	}
	return _config->m_nReaderThreads;
}
/*--------------------------------------------------------------------------*/
size_t Configuration_GetParserThreadsNum(Configuration *_config)
{
	if(!_config)
	{
		return NOT_INIT;
	}
	return _config->m_nParsingThreads;
}
/*--------------------------------------------------------------------------*/
size_t Configuration_GetReportThreadsNum(Configuration *_config)
{
	if(!_config)
	{
		return NOT_INIT;
	}
	return _config->m_nReportThreads;
}

/*--------------------------------------------------------------------------*/
size_t Configuration_GetUiThreadsNum(Configuration *_config)
{
	if(!_config)
	{
		return NOT_INIT;
	}
	return _config->m_nUiThreads;
}













