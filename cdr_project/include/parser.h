#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdlib.h>
#include "read.h"
#include "aggregate.h"

typedef struct Parser Parser;

typedef enum Parser_Result
{
    PARSER_SUCCESS,
    PARSER_THREAD_FAIL,
    PARSER_UNINITIALIZED
}Parser_Result;

Parser *Parser_Create(size_t _parserThreadNum, Read *_read, Aggregate *_aggregate);
void Parser_Destroy(Parser **_parser);

Parser_Result Parser_Run(Parser *_parser);
void Parser_Stop(Parser *_parser);

void Parser_Pause(Parser *_parser);
Parser_Result Parser_Resume(Parser *_parser);

/* test */
void Parser_PrintCdr(Cdr *_cdr);

#endif /* __PARSER_H__ */

