#ifndef __BST_H__
#define __BST_H__

#include <stddef.h> /*for size_t type */

typedef struct bst bst_t;

/* Implementation Detail - might change */
typedef struct bst_node* bst_iter_t;

/* In C File:

typedef struct bst_node bst_node_t;

typedef enum child_pos
{
    LEFT = 0,
    RIGHT = 1,
    NUM_OF_CHILDREN = 2
}

struct bst_node
{
    void *data;
    bst_node_t *parent;
    bst_node_t *children[NUM_OF_CHILDREN];
};

struct bst
{
    bst_node_t dummy;
    bst_comp_func cmp;
}

*/

/* -----------------------------------------------------------------------------
    @ Description: The function format needed to create a bst.
                    The comparison function must return an integer less than,
                    equal to, or greater than zero if the first argument is
                    considered to be respectively less than, equal to,
                    or greater than the second.
                    
    @ Params:       item1, item2: Pointers to the two items to be compared.
    
    @ Return value: an integer.
                    Negative if  item1 < item2.
                    Zero if item1 == item2.
                    Positive item1 > item2.
----------------------------------------------------------------------------- */
typedef int (*bst_comp_func) (const void *item1, const void *item2);

/* -----------------------------------------------------------------------------
    @ Description:   A user defined function to be preforemed on the BST from
                     within the BSTForEach function.
                    
    @ Params:        'data' - an existing element's data that the action 
                     funcion will perform on.
                     'param' - an optional argument, typically used to perform
                     operations on the data.
    
    @ Return value:  an integer representing success or failure of the action 
                     function. 0 represents success, any other number represents failure.
----------------------------------------------------------------------------- */
typedef int (*bst_action)(void *data, void *param);


/* -----------------------------------------------------------------------------
    @description:      This function creates a new Binary Search Tree (BST),
                       where the comparison between keys in the tree is done
                       using the comparison function provided as a parameter.

    @params:           comp: A comparison function of type `bst_comp_func`
                       that compares two keys in the tree. The function should take
                       two parameters: `const void *item` and `const void *item2` and 
                       return a negative value if `item` is less than `item2`, 0 if
                       they are equal, and a positive value if `item` is greater than
                       `item2`.

    @return value:     The function returns a pointer to a `bst_t` structure
                       representing the created tree. If there is a memory
                       allocation problem, the function returns NULL.

    @errors:           If memory allocation fails, the function returns NULL.

    @time complexity:  O(1).

    @space complexity: O(1).
----------------------------------------------------------------------------- */
bst_t *BSTCreate(bst_comp_func comp);

/* -----------------------------------------------------------------------------
    @ description:      Destroys a BST data structure that was initially
                        created with BSTCreate.
    
    @ params:           pointer to BST to destroy.
    
    @ time complexity:  O(n)
    
    @ space complexity: O(1)
----------------------------------------------------------------------------- */
void BSTDestroy(bst_t *bst);

/* -----------------------------------------------------------------------------
    @ description:     inserts new member to the tree in the right place according
                       to its key. 
        
    @ params:          bst - point to the first element of the tree, where the
                       bst_comp_func is.
                       data - point to data will enter the tree.
                   
    @ return value:    iterator pointed to the data.
    
    @ errors:          returns end iterator if memory allocation failed.
                       the key must be unique, otherwise the behavior is undefined.
    
    @ time complexity: worst case - O(n)
                       average case - O(log(n))
                       best case - O(1)
                       
    @ space complexity: O(1)
----------------------------------------------------------------------------- */
bst_iter_t BSTInsert(bst_t *bst, const void *data);


/* -----------------------------------------------------------------------------
    @ description:      removes an element from the bst.
    
    @ params:           iter - the element to be removed.
    
    @ return value:     an iterator to the next element after the removed one.
    
    @ time complexity:  average case: O(logn). worst case: O(n).
    
    @ space complexity: O(1).
----------------------------------------------------------------------------- */
bst_iter_t BSTRemove(bst_iter_t iter);

/* -----------------------------------------------------------------------------
    @ description:       find data in bst.
    
    @ params:           'is match' - matching function suited for user's data.
                        'data' parameter to match with data stored in bst.
                        
    @ return value:     if found => iterator holding data.
                        else => dummy end iterator.
                        
    @ time complexity:  
        - best_case - O(1)
        - average_case - O(log(n))
        - worse_case - O(n)
    @ space complexity: O(1).
----------------------------------------------------------------------------- */
bst_iter_t BSTFind(bst_t *tree, const void *param);

/*
    @ description:      checks if a BST data structure is empty.
    
    @ params:           a pointer to a BST.
    
    @ return value:     returns 1 if the BST is empty and 0 if not.
    
    @ time complexity:  O(1)
    
    @ space complexity: O(1)
*/ 
int BSTIsEmpty(const bst_t *bst);

/*
    @ description:      checks the size of a BST.
    
    @ params:           a pointer to a BST.
    
    @ return value:     returns the number of elements in the BST.
    
    @ time complexity:  O(n)
    
    @ space complexity: O(1)
*/ 
size_t BSTSize(const bst_t *bst);

/* -----------------------------------------------------------------------------
    @ description:      Get the data from a given Iterator.
    
    @ params:           Iterator to get data from.
    
    @ return value:     A pointer to the data.
    
    @ time complexity:  O(1). 
    
    @ space complexity: O(1).
----------------------------------------------------------------------------- */ 
void *BSTGetData(bst_iter_t iter); 

/* -----------------------------------------------------------------------------
    @ description:      a function that preforms an action on each node within
                        its given range. The function traverses the tree in the [from,to)
                        range in an "in-order" traversal, traversal is stopped if an 
                        action fails.
                        
    @ params:           to, and from, denoting the desired range.
                        param - (optional) a void pointer to a param used by 
                        bst_action.
                        bst_action: a function that preforms an action on a node.
              
    @ return value:     
        - if no action function failed => 0.
        - else => failure status returned from action function failed.
    
    @ errors:           action failed
    
    @ time complexity:  O(n)
    
    @ space complexity: O(1)
----------------------------------------------------------------------------- */ 
int BSTForEach(bst_iter_t from, bst_iter_t to, bst_action action, void *param);

/* -----------------------------------------------------------------------------
    @ description:      a function that returns an iterator to BST's smallest
     					element.
    	
    @ params:           - bst - predefined bst created by BSTCreate function.
    
    @ return value:     - iterator to BST smallest element if BST is empty -  
                        BSTEnd will be returned.
                        
    @ time complexity:  O(logn)
    
    @ space complexity: O(1)
----------------------------------------------------------------------------- */
bst_iter_t BSTBegin(const bst_t *bst);

/* -----------------------------------------------------------------------------
    @ description:      gives the end element of the BST. the end element is not
                        a valid element and is used to mark the end of the 
                        data structure.
                        
    @ params:           a pointer to a BST.
    
    @ return value:     returns an iterator to the end element.
    
    @ time complexity:  O(1)
    
    @ space complexity: O(1)
----------------------------------------------------------------------------- */ 
bst_iter_t BSTEnd(const bst_t *bst);


/* -----------------------------------------------------------------------------
    @ description:      gets the next element in the BST.
    
    @ params:           an iterator to an element to get its next.
    
    @ return value:     returns the next element in the BST.
    
    @ note:             if BSTNext is called on the end element (BSTEnd)
                        behavior is undefined.
    
    @ time complexity:   
        - best_case - O(1)
        - average_case - O(log(n))
        - worse_case - O(n)

    @ space complexity: O(1)
----------------------------------------------------------------------------- */ 
bst_iter_t BSTNext(bst_iter_t iter);


/* -----------------------------------------------------------------------------
    @ description:      gets the previous element in the BST.
    
    @ params:           an iterator to an element to get its previous.
    
    @ return value:     returns the previous element in the BST.
    
    @ note:             if BSTPrev is called on the BSTBegin element, behavior
                        is undefined.
    
    @ time complexity:   
        - best_case - O(1)
        - average_case - O(log(n))
        - worse_case - O(n)
    
    @ space complexity: O(1)
----------------------------------------------------------------------------- */ 
bst_iter_t BSTPrev(bst_iter_t iter);

/* -----------------------------------------------------------------------------
    @ description:      checks if two iterators are the same.
    
    @ params:           two iterators to elements in the BST, to check if they are the same.
    
    @ return value:     returns 1 if both elements are the same, and 0 otherwise.
    
    @ time complexity:  O(1)
    
    @ space complexity: O(1)
----------------------------------------------------------------------------- */ 
int BSTIsSameIter(bst_iter_t one, bst_iter_t other);


#endif /* __BST_H__ */                    
