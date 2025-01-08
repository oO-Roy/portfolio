#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <stddef.h> /*for size_t type */

typedef int (*hash_is_match_func) (const void* data, void* param);
typedef int (*hash_action_func) (void* data, void* param);
typedef size_t (*hash_func) (const void* data);

typedef struct hash hash_t;

/*
    in src file:
    struct hash
    {
        *hash_action_func action_func;
        *hash_is_match_func is_match_func;
        dllist * buckets[];
    }
*/
/* -----------------------------------------------------------------------------
    @ description:      This function creates and return a pointer to a new 
                        hash table, where the return value of the hash func
                        is the index of the data.
    
    @ params:           hash - point to hash function of type `hash_func`.
                        is_match - point to match function of type 
                        `hash_is_match_func` that
                        return 1 when the data and the param are the same.
                        max_index - the size of the new table.

    @ errors:           return NULL when allocation fail.
    
    @ time complexity:  O(n)
    
    @ space complexity: O(n)
----------------------------------------------------------------------------- */
hash_t* HashTCreate(hash_func hash, hash_is_match_func is_match
                                                    , size_t max_table_size);


/* -----------------------------------------------------------------------------
    @ description:      Releases the memory allocated for the hash table.
    
    @ params:           ht - a pointer to a hash table created by HashTCreate. 

    @ time complexity:  O(n)
    
    @ space complexity: O(1)
----------------------------------------------------------------------------- */
void HashTDestroy(hash_t* ht);


/* -----------------------------------------------------------------------------
    @ description: Inserts a new element into the hash table.    
    
    @ params: 
            # ht   - A pointer to the hash table where the data should
                     be inserted.
            # data - A pointer to the data to be inserted into the 
                     hash table.
                     
    @ return value: Return 0 on success.
    
    @ errors: malloc failiure
    
    @ time complexity:  - average case: O(1).
                        - worst case: O(n).
                        
    @ space complexity: O(1)
----------------------------------------------------------------------------- */
int HashTInsert(hash_t* ht ,void* data);

/* -----------------------------------------------------------------------------
    @ description:      Removes an element from the hash table containing the 
                        specified data.      
    
    @ params:           hash - The hash table data structure.         
                        data - The data to remove.
    @ return value:     Returns the data element associated with the searched 
                        data.
                        If data isn't found, returns NULL.
    @ errors:           None.
    
    @ time complexity:  - average case: O(1).
                        - worst case: O(n).
                        
    @ space complexity: O(1).
----------------------------------------------------------------------------- */
void *HashTRemove(hash_t* hash, void* data);


/* -----------------------------------------------------------------------------
    @ description:      Find if a Specific data is in the hash table.
    
    @ params:           hash - The hash table.
                        data - the data to be found.

    @ return value:     If found - data, else - NULL.

    @ time complexity:  - Average case: O(1).
                        - Worst case: O(n).
    
    @ space complexity: O(1).
----------------------------------------------------------------------------- */
void *HashTFind(hash_t* hash, const void* param);


/* -----------------------------------------------------------------------------
    @ description:      perform an action on each element in the hash table.
    
    @ params:           - hash table.
                        - action function to call on each element in the hash 
                          table.
                        - parameter passed to the action function.
    
    @ errors:           none.

    @ return value:     if an action function failed => non zero value, 
                        otherwise => 0.
    
    @ time complexity:  O(n).
    
    @ space complexity: O(1).
----------------------------------------------------------------------------- */
int HashTForeach(hash_t* hash, hash_action_func action, void* param);


/* -----------------------------------------------------------------------------
    @ description:      Checks if hash table is empty.
    
    @ params:           hash_table - pointer to hash table that was previously 
                        created by HashTCreate function.

    @ errors:           none.
    
    @ time complexity:  O(max_table_size)
    
    @ space complexity: O(1)
----------------------------------------------------------------------------- */
int HashTIsEmpty(const hash_t* hash_table);

/* -----------------------------------------------------------------------------
    @ description:      checks the size of a hash_table
    
    @ params:           hash_table - pointer to hash table that was previously 
                        created by HashTCreate function.   

    @ return value:     number of elements in hash a table.

    @ errors:           none.

    @ time complexity:  o(max table size).

    @ space complexity: o(1).
----------------------------------------------------------------------------- */
size_t HashTSize(const hash_t* hash_table);

/* -----------------------------------------------------------------------------
    @ description:          The function shall recive the hash table map and 
                            return
                            the load factor ratio of the map.
    
    @ params:               The function recive the hash table map.

    @ errors:               none.
    
    @ time complexity:      O(1)
    
    @ space complexity:     O(1)
----------------------------------------------------------------------------- */
double HashTLoad(const hash_t* hash_table);

double HashTSD(const hash_t* hash_table);

#endif /*__HASH_TABLE_H__*/