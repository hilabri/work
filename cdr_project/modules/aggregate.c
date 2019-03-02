#include "protected_hash.h"
#include "aggregate.h"
#include "zlog4.h"
#include <string.h>

#define HASH_GET_LOCKS_NUMBER(_hashBucketNumber) (_hashBucketNumber / 2)

#define MODULE "aggregate"

struct Aggregate
{
    Protected_Hash* m_subscribers;
    Protected_Hash *m_operators;
};

/*------------------------------------------------------------*/

static size_t Aggregate_HashFunction(const void* _key)
{
    const char *str = _key;
    size_t hash = 0;
    int c;

    while ((c = *str++))
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

/*------------------------------------------------------------*/

static int Aggregate_KeysEqualFunc(const void* _firstKey, const void* _secondKey)
{
	/* Hash keys are strings: */
	return (0 == strcmp(_firstKey, _secondKey));
}

/*------------------------------------------------------------*/

Aggregate *Aggregate_Create(size_t _subscriberHashSize, size_t _operatorHashSize)
{
    size_t subscribersLocks = HASH_GET_LOCKS_NUMBER(_subscriberHashSize);
    size_t operatorsLocks = HASH_GET_LOCKS_NUMBER(_operatorHashSize);
    Aggregate *aggregate = malloc(sizeof(Aggregate));
    if (!aggregate)
    {
        ZLOG(LOG_ERROR, MODULE, "Aggregate failed to allocate memory");
        return NULL;
    }
    aggregate->m_subscribers = ProtectedHash_Create(_subscriberHashSize, subscribersLocks, Aggregate_HashFunction, Aggregate_KeysEqualFunc);
    if(!aggregate->m_subscribers)
    {
        free(aggregate);
        ZLOG(LOG_ERROR, MODULE, "Aggregate failed to create subscribers hash");
        return NULL;
    }
    aggregate->m_operators = ProtectedHash_Create(_operatorHashSize, operatorsLocks, Aggregate_HashFunction, Aggregate_KeysEqualFunc);
    if(!aggregate->m_operators)
    {
        free(aggregate->m_subscribers);
        free(aggregate);
        ZLOG(LOG_ERROR, MODULE, "Aggregate failed to create operators hash");
        return NULL;
    }
    ZLOG(LOG_INFO, MODULE, "Aggregate created");
    return aggregate;
}

/*------------------------------------------------------------*/

void DestroyElement(void* _element)
{
    free(_element);
}

/*------------------------------------------------------------*/

void Aggregate_Destroy(Aggregate **_aggregate)
{
    if(!_aggregate || !(*_aggregate))
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return;
    }
    ProtectedHash_Destroy((*_aggregate)->m_subscribers, DestroyElement, DestroyElement);
    ProtectedHash_Destroy((*_aggregate)->m_operators, DestroyElement, DestroyElement);
    free(*_aggregate);
    *_aggregate = NULL;
    ZLOG(LOG_INFO, MODULE, "Aggregate destroy done");
}

/*------------------------------------------------------------*/

static void UpdateSubscriberInfo(SubscriberInfo* _subscriber, const Cdr* _cdr)
{
    const enum Aggregate_TargetOperator operator_index = (0 == strcmp(_cdr->m_operatorName, _cdr->m_partyOperator)) ? OPERATOR_SAME : OPERATOR_OTHER;
    switch (_cdr->m_callType)
    {
    case MOC:
        _subscriber->m_calls[OUTGOING][operator_index] += _cdr->m_callDuration;
        break;
    case MTC:
        _subscriber->m_calls[INCOMING][operator_index] += _cdr->m_callDuration;
        break;
    case SMS_MO:
        _subscriber->m_sms[OUTGOING][operator_index]++;
        break;
    case SMS_MT:
        _subscriber->m_sms[INCOMING][operator_index]++;
        break;
    case GPRS:
        _subscriber->m_internetMbs[INCOMING] += _cdr->m_mbDownload;
        _subscriber->m_internetMbs[OUTGOING] += _cdr->m_mbUpload;
        break;
    default: 
        ZLOG(LOG_ERROR, MODULE, "Unexpected call type in cdr: %d", _cdr->m_callType);
        break;
    }
}
/*--------------------------------------------------*/
static void UpdateOperatorInfo(OperatorInfo* _operator, const Cdr* _cdr)
{
    switch (_cdr->m_callType)
    {
    case MOC:
        _operator->m_calls[OUTGOING] += _cdr->m_callDuration;
        break;
    case MTC:
        _operator->m_calls[INCOMING] += _cdr->m_callDuration;
        break;
    case SMS_MO:
        _operator->m_sms[OUTGOING]++;
        break;
    case SMS_MT:
        _operator->m_sms[INCOMING]++;
        break;
    case GPRS: /* not required in operator info */
        break;
    default:
        ZLOG(LOG_ERROR, MODULE, "Unexpected call type in cdr: %d", _cdr->m_callType);
        break;
    }
}
/*------------------------------------------------------------*/
/* Create new functios: */

static void *CreateSubscriber(const void *_cdr)
{
    const Cdr *cdr = _cdr;
    SubscriberInfo *subscriber = calloc(1, sizeof(SubscriberInfo));
    if (!subscriber)
    {
        ZLOG(LOG_ERROR, MODULE, "Aggregate failed to allocate memory for subscriber info");
        return NULL;
    }
    strcpy(subscriber->m_operatorName, cdr->m_operatorName);
    UpdateSubscriberInfo(subscriber, cdr);
    return subscriber;
}
/*--------------------------------------------------*/
static void *CreateOperator(const void *_cdr)
{
    const Cdr *cdr = _cdr;
    OperatorInfo *operator = calloc(1, sizeof(OperatorInfo));
    if (!operator)
    {
        ZLOG(LOG_ERROR, MODULE, "Aggregate failed to allocate memory for operator info");
        return NULL;
    }
    strcpy(operator->m_brandName, cdr->m_operatorName);
    operator->m_mccMnc = cdr->m_operatorMCC;
    UpdateOperatorInfo(operator, cdr);
    return operator;
}
/*--------------------------------------------------*/
/* Update functions: */

static void UpdateSubscriber(void* _subscriber, const void* _cdr)
{
    UpdateSubscriberInfo((SubscriberInfo *)_subscriber, (const Cdr *)_cdr);
}

static void UpdateOperator(void* _operator, const void* _cdr)
{
    UpdateOperatorInfo((OperatorInfo *)_operator, (const Cdr *)_cdr);
}

/*------------------------------------------------------------*/

Aggregate_Result Aggregate_InsertCdrData(Aggregate *_aggregate, Cdr *_cdr)
{
    ProtectedHashUpsertStatus status;
    char *subscriber_key;
    char *operator_key;
    if(!_aggregate)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return  AGGREGATE_UNINITIALIZED;
    }
    subscriber_key = malloc(sizeof(char) * ID_SIZE);
    if (!subscriber_key)
    {
        ZLOG(LOG_ERROR, MODULE, "Aggregate failed to allocate memory for subscriber key");
        return AGGREGATE_FAIL;
    }
    strcpy(subscriber_key, _cdr->m_IMSI);
    operator_key = malloc(sizeof(char) * OPERATOR_NAME_SIZE);
    if (!operator_key)
    {
        free(subscriber_key);
        ZLOG(LOG_ERROR, MODULE, "Aggregate failed to allocate memory for operator key");
        return AGGREGATE_FAIL;
    }
    strcpy(operator_key, _cdr->m_operatorName);
	
    status = ProtectedHash_Upsert(_aggregate->m_subscribers, subscriber_key, CreateSubscriber, UpdateSubscriber, _cdr);
    if(E_PROHASH_UPDATE == status)
    {
         free(subscriber_key);
    } 
    status = ProtectedHash_Upsert(_aggregate->m_operators, operator_key, CreateOperator, UpdateOperator, _cdr);
    if(E_PROHASH_UPDATE == status)
    {
         free(operator_key);
    } 
    ZLOG(LOG_TRACE, MODULE, "Aggregate inserted Cdr data");
    return AGGREGATE_SUCCESS;
}

/*------------------------------------------------------------*/

Aggregate_Result Aggregate_RetrieveSubscriberData(Aggregate *_aggregate, const char *_subscriberName, SubscriberInfo **_subscriberInfo)
{
    ProtectedHashRetrieveStatus status;
    if(!_aggregate)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return  AGGREGATE_UNINITIALIZED;
    }
    status = ProtectedHash_Retrieve(_aggregate->m_subscribers, _subscriberName, (void **)_subscriberInfo);
    switch (status)
    {
		case E_PROHASH_SUCCESS:
		{

    		ZLOG(LOG_TRACE, MODULE, "Aggregate Retrieve Subscriber %s Data", _subscriberName);
		    return AGGREGATE_SUCCESS;
		}
		case E_PROHASH_NOT_FOUND:
		{
		    ZLOG(LOG_TRACE, MODULE, "Aggregate subscriber not found");
		    return AGGREGATE_FAIL;
		}
		default: 
		{
			ZLOG(LOG_ERROR, MODULE, "Aggregate subscriber retrieve data failed");
			return AGGREGATE_FAIL;
		} 
    }
}

/*------------------------------------------------------------*/

Aggregate_Result Aggregate_RetrieveOperatorData(Aggregate *_aggregate, const char *_operatorName, OperatorInfo **_operatorInfo)
{
    ProtectedHashRetrieveStatus status;
    if(!_aggregate)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return  AGGREGATE_UNINITIALIZED;
    }
    status = ProtectedHash_Retrieve(_aggregate->m_operators, _operatorName, (void **)_operatorInfo);
    switch (status)
    {
		case E_PROHASH_SUCCESS:
		{
    		ZLOG(LOG_TRACE, MODULE, "Aggregate Retrieve Operator %s Data", _operatorName);
		    return AGGREGATE_SUCCESS;
		}
		case E_PROHASH_NOT_FOUND:
		{
		    ZLOG(LOG_TRACE, MODULE, "Aggregate operator not found");
		    return AGGREGATE_FAIL;
		}
		default: 
		{
			ZLOG(LOG_ERROR, MODULE, "Aggregate operator retrieve data failed");
			return AGGREGATE_FAIL; 
		}
    }
}

/*------------------------------------------------------------*/

Aggregate_Result Aggregate_RetrieveAllSubscribers(Aggregate *_aggregate, FormatFunc _format, FILE *fp)
{
    size_t count;
    if(!_aggregate)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return  AGGREGATE_UNINITIALIZED;
    }
    count = ProtectedHash_ForEach(_aggregate->m_subscribers, _format, fp);
    ZLOG(LOG_TRACE, MODULE, "Done retrieving %lu subscribers\n", count);
    return AGGREGATE_SUCCESS;
}
/*------------------------------------------------------------*/
Aggregate_Result Aggregate_RetrieveAllOperators(Aggregate *_aggregate, FormatFunc _format, FILE *fp)
{
    size_t count;
    if(!_aggregate)
    {
        ZLOG(LOG_ERROR, MODULE, "Uninitialized parameter");
        return  AGGREGATE_UNINITIALIZED;
    }
    count = ProtectedHash_ForEach(_aggregate->m_operators, _format, fp);
    ZLOG(LOG_TRACE, MODULE,"Done retrieving %lu operators\n", count);
    return AGGREGATE_SUCCESS;
}

