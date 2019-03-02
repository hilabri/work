#ifndef __ADTDEFS_H__
#define __ADTDEFS_H__


#define TRUE  1
#define FALSE 0

typedef enum
{
    /* General ADT Errors */
    ERR_OK                    =0,
    ERR_GENERAL,
    ERR_NOT_INITIALIZED,
    ERR_ALLOCATION_FAILED,
    ERR_REALLOCATION_FAILED,
    ERR_UNDERFLOW,
    ERR_OVERFLOW,
    ERR_WRONG_INDEX,

    /* Vector Errors */

    /* Stack  Errors */

    /* LinkedList  Errors */

    /* Tree Errors*/
    ERR_ITEM_EXIST,

    /* maximum number of error returned by ADT implementations */
    ERR_ADT_MAX
} ADTErr;

#endif /*__ADTDEFS_H__*/
