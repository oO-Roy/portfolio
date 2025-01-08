#ifndef __AVL_H__
#define __AVL_H__

#include <stddef.h> /* for size_t type */

/*#include "dllist.h"*/ /* */

typedef enum bst_traversal
{
    PRE_ORDER = 0,
    IN_ORDER = 1,
    POST_ORDER = 2
} bst_traversal_t;


typedef struct avl avl_t;

/* -----------------------------------------------------------------------------
    @ Description:  The function format needed to create a avl.
                    The comparison function must return an integer less than,
                    equal to, or greater than zero if the first argument is
                    considered to be respectively less than, equal to,
                    or greater than the second.
                    
    @ Params:       first, second: Pointers to the two items to be compared.
    
    @ Return value: an integer.
                    Negative if  first < second.
                    Zero if first == second.
                    Positive first > second.
----------------------------------------------------------------------------- */
typedef int (*avl_comp_func) (const void *first, const void *second);

/* -----------------------------------------------------------------------------
    @ Description:   A user defined function to be preforemed on the AVL from
                     within the AVLForEach function.
                    
    @ Params:        'data' - an existing element's data that the action 
                     funcion will perform on.
                     'param' - an optional argument, typically used to perform
                     operations on the data.
    
    @ Return value:  an integer representing success or failure of the action 
                     function. 0 represents success, any other number represents failure.
----------------------------------------------------------------------------- */
typedef int (*avl_action_func)(void *data, void *param);

/* -----------------------------------------------------------------------------
   @description:       This function creates a new AVL Tree, where the comparison
                       between keys in the tree is done using the comparison function
                       provided as a parameter.

    @params:           comp: A comparison function of type `avl_comp_func` that
                       compares two keys in the tree. The function should take two
                       parameters: `const void *first` and `const void *second` and 
                       return a negative value if `first` is less than `second`, 0 if
                       they are equal, and a positive value if `first` is greater than
                       `second`.

    @return value:     The function returns a pointer to a `avl_t` structure
                       representing the created tree. If there is a memory
                       allocation problem, the function returns NULL.

    @errors:           If memory allocation fails, the function returns NULL.

    @time complexity:  O(1).

    @space complexity: O(1).
----------------------------------------------------------------------------- */
avl_t *AVLCreate(avl_comp_func comp);

/* -----------------------------------------------------------------------------
    @ description:      Function that frees all allocated memory of a AVL tree
    
    @ params:           an AVL tree memory allocation to be released

    @errors: 			none
    
    @ time complexity:  O(n)
    
    @ space complexity: O(1)
----------------------------------------------------------------------------- */
void AVLDestroy(avl_t *tree);

/* -----------------------------------------------------------------------------
    @ description:      Adds a new element to the AVL tree while maintaining the balance of the tree.
       
    @ params:           tree - A pointer to the AVL tree where the element will be inserted.
                        The tree should be initialized using AVLCreate function.
                      
                        const void *data - A pointer to the data that needs to be inserted into
                        the tree. 
                           
    @ time complexity:  O(log n) 
    
    @ space complexity: O(log n) 
----------------------------------------------------------------------------- */
int AVLInsert(avl_t *tree, void *data);

/* -----------------------------------------------------------------------------
    @ description:      removes the element form the tree containing the data
                        provided by the user.      
    
    @ params:           tree - the tree data structure.
                        data - the data to be removed.
      
    @return value:      returns the removed data. if data isn't found in the tree, return NULL.      

    @errors: returns    NULL if data isn't found in the tree.
    
    @ time complexity:  O(log(n)). 
    
    @ space complexity: O(1).
----------------------------------------------------------------------------- */
void *AVLRemove(avl_t *tree, const void *data); 

/*
    @ description:      checks the size of a tree.
    
    @ params:           a pointer to a tree.
    
    @ return value:     returns the number of nodes in the tree.
    
    @ time complexity:  O(n)
    
    @ space complexity: O(1)
*/ 
size_t AVLSize(const avl_t *tree); 

/* -----------------------------------------------------------------------------
    @ description:      checks if tree has no elements.
    
    @ params:           a pointer to tree.
      
    @return value:      returns 1 if the tree is empty and 0 if not.   
    
    @ time complexity:  O(1)
    
    @ space complexity: O(1)
----------------------------------------------------------------------------- */
int AVLIsEmpty(const avl_t *tree); 

/* -----------------------------------------------------------------------------
    @ description: 		find data in avl tree
    
    @ params: 			avl tree, parameter holding a search key.
    
    @ return value: 	if found - data found, else - NULL.
    
    @ time complexity: 	O(log(n)).
    
    @ space complexity: O(log(n)).
----------------------------------------------------------------------------- */
void *AVLFind(const avl_t *tree, const void *param);

/* -----------------------------------------------------------------------------
    @ description:      Performs a specified action on each element in the AVL tree.
    
    @ params:           - tree: Apointer to the AVL tree.
                        - oreder: The traversal order (PRE_ORDER, IN_ORDER, or POST_ORDER).
                        - action: A function to be pcalled for each element.
                        - param: A parameter to be passed to the action function.
      
    @return value:      0 on success, non-zero if the action function returns a non-zero value.   

    @errors:            action fault.
    
    @ time complexity:  O(n) 
    
    @ space complexity: O(log(n)) for height
----------------------------------------------------------------------------- */
int AVLForEach(avl_t *tree, bst_traversal_t order, avl_action_func action, void *params);

/* -----------------------------------------------------------------------------
    @ description:      check the height of the tree, which is defined as the
                        number of levels the tree has. for example, a tree with
                        only a root node has a height of 1.
                        a tree with no nodes at all, has a height of 0.
    
    @ params:           an AVL data structure.
      
    @return value:      returns the height of the tree.    

    @ time complexity:  O(1)
    
    @ space complexity: O(1)
----------------------------------------------------------------------------- */
size_t AVLHeight(const avl_t *tree);

/* ADVANCED 

int AVLFindMultiple(const avl_t *tree, int (*is_match)(void *data, void *arg), void *arg, dllist_t *list);
void AVLRemoveMultiple(avl_t *tree, int (*should_remove)(void *data, void *arg), void *arg);
*/

#endif /* __AVL_H__ */
