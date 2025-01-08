/* 
	Developer: Roy Steiner
	File: Hash_table
	Reviewer: Yakir Asur.
	Date: 14/08/2024.
	Status: Approved.
*/

#include <stdlib.h> /*malloc & free*/
#include <math.h>   /*sqrt*/
#include <assert.h> /*assert*/
#include "hash_table.h" /*typedef*/
#include "dllist.h" /*API*/

#define UNUSED(x) (void)(x)
#define TWO 2

struct hash
{
    hash_func hash_func;
    hash_is_match_func is_match;
    size_t max_table_size;
    dllist_t** buckets;
};

static int GetIndex(hash_t* ht, void* data)
{
    assert(NULL != ht);

    return (ht->hash_func(data) % ht->max_table_size);
}

void HashTDestroy(hash_t* ht)
{   
    size_t i = 0;

    assert(NULL != ht);
    
    for ( ; i < ht->max_table_size ; ++i)
    {
        DLlistDestroy(ht->buckets[i]);
    }

    free(ht->buckets);
    free(ht);
}

hash_t* HashTCreate(hash_func hash, hash_is_match_func is_match
                                                    , size_t max_table_size)
{
    int i = 0;
    hash_t* ht = NULL;

    assert(NULL != hash);
    assert(NULL != is_match);
    
    ht = (hash_t*)malloc(sizeof(hash_t));
    if (NULL == ht)
    {
        return NULL;
    }

    ht->buckets = (dllist_t**)malloc(sizeof(dllist_t*) * max_table_size);
    if (NULL == ht->buckets)
    {
        free(ht);
        return NULL;
    }

    for ( ; (size_t)i < max_table_size ; ++i)
    {
        ht->buckets[i] = DLlistCreate();
        if (NULL == ht->buckets[i])
        {
            for (; i >= 0; --i)
            {
                DLlistDestroy(ht->buckets[i]);
            }
            
            free(ht);

            return NULL;
        }
    }
    
    ht->hash_func = hash;
    ht->is_match = is_match;
    ht->max_table_size = max_table_size;

    return ht;    
}

int HashTInsert(hash_t* ht ,void* data)
{
    size_t index = 0;
    
    assert(NULL != ht);

    index = GetIndex(ht, data);

    return DLlistIsSameIter(DLlistEnd(ht->buckets[index])
                , DLlistInsertBefore(DLlistBegin(ht->buckets[index]), data));
}

static dll_iter_t GetIter(hash_t* ht, size_t index, void* data)
{
    assert(NULL != ht);
    
    return DLlistFind(DLlistBegin(ht->buckets[index])
                        , DLlistEnd(ht->buckets[index]), ht->is_match, data);
}

void *HashTRemove(hash_t* ht, void* data)
{
    size_t index = 0;
    dll_iter_t iter = NULL;
    void* iter_data = NULL;

    assert(NULL != ht);

    index = GetIndex(ht, data);

    iter = GetIter(ht, index, data);
            
    if (!DLlistIsSameIter(DLlistEnd(ht->buckets[index]), iter))
    {
        iter_data = DLlistGetVal(iter);
        DLlistRemove(iter);
    }
    
	return iter_data;
}

void *HashTFind(hash_t* ht, const void* param)
{
    size_t index = 0;
    dll_iter_t iter = NULL;
    void* tmp_data = NULL;

    assert(NULL != ht);

    index = GetIndex(ht, (void*)param);

    iter = GetIter(ht, index, (void*)param);

    if (!DLlistIsSameIter(DLlistEnd(ht->buckets[index]), iter))
    {
        if (!DLlistIsSameIter(DLlistBegin(ht->buckets[index]), iter))
        {
            DLlistSplice(DLlistBegin(ht->buckets[index]), iter
                                                            , DLlistNext(iter));
        }
        
        tmp_data = DLlistGetVal(DLlistBegin(ht->buckets[index]));
    }
    
	return tmp_data;
}

int HashTForeach(hash_t* ht, hash_action_func action, void* param)
{
    size_t i = 0;
    int action_func_status = 0;
    
    assert(NULL != ht);
    assert(NULL != action);

	for ( ; (i < ht->max_table_size) && (0 == action_func_status) ; ++i)
    {
        action_func_status = DLlistForEach(DLlistBegin(ht->buckets[i])
                                    , DLlistEnd(ht->buckets[i]), action, param);
    }
	
	return action_func_status;
}

static int CountingElements(void *data, void *param)
{
	UNUSED(data);
	
	++(*(size_t *)param);
	
	return 0;
}

size_t HashTSize(const hash_t* ht)
{
    size_t counter = 0;
	
	assert(NULL != ht);
	
	HashTForeach((hash_t*)ht, CountingElements, &counter);
	
	return counter;
}

int HashTIsEmpty(const hash_t* ht)
{
    size_t i = 0;

    assert(NULL != ht);

    for ( ; i < ht->max_table_size ; ++i)
    {
        if (!DLlistIsEmpty(ht->buckets[i]))
        {
            return 0;
        }
    }
    
    return 1;
}

double HashTLoad(const hash_t* ht)
{
    assert(NULL != ht);

    return ((double)HashTSize(ht) / ht->max_table_size);
}

double HashTSD(const hash_t* ht)
{
    size_t i = 0;
    double numerator = 0;
    double mean = 0;

    assert(NULL != ht);

    mean = HashTLoad(ht);

    for ( ; i < ht->max_table_size; ++i)
    {
        numerator += pow((DLlistSize(ht->buckets[i]) - mean), TWO);
    }

    return sqrt(numerator/ht->max_table_size);
}