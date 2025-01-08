/* 
	Developer: Roy Steiner.
	File: Binary search tree.
	Reviewer: Alon Brull. 
	Date: 31/07/24
	Status: APPROVED.
*/

#include <stdlib.h>	/* malloc & free */
#include <assert.h>	/* assert */

#include "bst.h"	/* typedef, size_t */

#define UNUSED(x) (void)(x)

typedef struct bst_node bst_node_t;

typedef enum child_pos
{
    LEFT = 0,
    RIGHT = 1,
    NUM_OF_CHILDREN = 2
}child_pos_t;

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
};

static bst_iter_t NodeToIter(const bst_node_t *node)
{
	assert(NULL != node);
	
	return (bst_iter_t)node;
}

static bst_node_t *IterToNode(const bst_iter_t iter)
{
	assert(NULL != (bst_node_t *)iter);
	
	return (bst_node_t *)iter;
}

static bst_node_t *GetParent(const bst_node_t *node)
{
	assert(NULL != node);
	
	return node->parent;
}

static bst_node_t *GetChild(const bst_node_t *node, child_pos_t side)
{
	assert(NULL != node);
	
	return node->children[side];
}


static int IsNodeALeftOrRightChild(const bst_node_t *node)
{
	bst_node_t *parent_node = NULL;
	
	assert(NULL != node);
	
	parent_node = GetParent(node);
	
	return (node == GetChild(parent_node, RIGHT));
}


static int HasRightChild(const bst_node_t *node)
{
	assert(NULL != node);
	
	return (NULL != GetChild(node, RIGHT));
}

static int HasLeftChild(const bst_node_t *node)
{
	assert(NULL != node);
	
	return (NULL != GetChild(node, LEFT));
}

/* Check if node has children 
	0 - none, 
	1 - has 1 child, 
	2 - has 2 child
*/
static size_t HowManyChildren(const bst_node_t *node)
{
	assert(NULL != node);
	
	return ((NULL != GetChild(node, LEFT)) 
				+ (NULL != GetChild(node, RIGHT)));
}

static bst_node_t *GetRoot(const bst_t *bst)
{
	return GetChild(BSTEnd(bst), LEFT);
}

bst_t *BSTCreate(bst_comp_func comp)
{
	bst_t *bst = NULL;
	
	assert(NULL != comp);

	bst = (bst_t *)malloc(sizeof(bst_t));
	if (NULL == bst)
	{
		return NULL;
	}
	
	bst->cmp = comp;
	bst->dummy.data = NULL;
	bst->dummy.parent = NULL;
	bst->dummy.children[LEFT] = NULL;
	bst->dummy.children[RIGHT] = NULL;
	
	return bst;
}

void BSTDestroy(bst_t *bst)
{
	int child_side = 0;
	bst_node_t *n_runner = NULL;
	
	assert(NULL != bst);
	
	n_runner = GetRoot(bst);

	while (!BSTIsEmpty(bst))
	{
		if (HasRightChild(n_runner))
		{ 
			n_runner = GetChild(n_runner, RIGHT);	
		}
		
		else if (HasLeftChild(n_runner))
		{
			n_runner = GetChild(n_runner, LEFT);
		}
			
		else 
		{
			child_side = IsNodeALeftOrRightChild(n_runner);
			n_runner = GetParent(n_runner);
			
			free(GetChild(n_runner, child_side));
			n_runner->children[child_side] = NULL;	
		}
	}
	
	free(bst);
}

bst_iter_t BSTInsert(bst_t *bst, const void *data)
{
	int child_side = 0;
	bst_node_t *node = NULL;
	bst_node_t *new_node = NULL;
	bst_node_t *n_runner = NULL;
	
	assert(NULL != bst);
	
	new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
	if (NULL == new_node)
	{
		return BSTEnd(bst);
	}
	
	n_runner = IterToNode(BSTEnd(bst));
	node = GetChild(n_runner, LEFT);
	
	while (NULL != node)
	{
		assert(0 != bst->cmp(node->data, data));

		child_side = (0 < bst->cmp(data, node->data));
		n_runner = node;
		node = GetChild(n_runner, child_side);
	}
	
	new_node->data = (void *)data;
	new_node->parent = n_runner;
	new_node->children[LEFT] = NULL;
	new_node->children[RIGHT] = NULL;
	
	n_runner->children[child_side] = new_node;
	
	return NodeToIter(new_node);
}


bst_iter_t BSTRemove(bst_iter_t iter)
{
	bst_node_t *next_node = NULL;
	bst_node_t *node = NULL;
	bst_node_t *parent_node = NULL;
	bst_node_t *child_node = NULL;
	
	assert(NULL != IterToNode(iter));

	node = IterToNode(iter);
	next_node = IterToNode(BSTNext(iter));
	
	if (1 >= HowManyChildren(node))
	{
		parent_node = GetParent(node);
		child_node = GetChild(node, HasRightChild(node));

		parent_node->children[IsNodeALeftOrRightChild(node)] = child_node;
		UNUSED((NULL != child_node) && (child_node->parent = parent_node));
		
		free(node);
	}

	else
	{
		if (node != GetParent(next_node))
		{	
			child_node = GetChild(next_node, RIGHT);

			if (NULL != child_node)
			{
				child_node->parent = GetParent(next_node);
				next_node->parent->children[LEFT] = child_node;
			
				node->children[LEFT]->parent = next_node;
				next_node->children[LEFT] = GetChild(node, LEFT);
				
				node->children[RIGHT]->parent = next_node;
				next_node->children[RIGHT] = GetChild(node, RIGHT);
					
				node->parent->children[IsNodeALeftOrRightChild(node)] = next_node;
				next_node->parent = GetParent(node);
				
				free(node);
			}
			
			else
			{
				next_node->parent->children[LEFT] = NULL;
				node->data = next_node->data;
				
				free(next_node);
				next_node = node;
			}
		}
		
		else
		{
			next_node->parent = node->parent;
			node->parent->children[IsNodeALeftOrRightChild(node)] = next_node;
				
			node->children[LEFT]->parent = next_node;
			next_node->children[LEFT] = GetChild(node, LEFT);
				
			free(node);
		}
	}
	
	return NodeToIter(next_node);
}

void *BSTGetData(bst_iter_t iter)
{
	assert(NULL != iter);
	
	return IterToNode(iter)->data;
}

bst_iter_t BSTBegin(const bst_t *bst)
{
	bst_node_t *node = NULL;
	
	assert(NULL != bst);
	
	node = (bst_node_t *)&bst->dummy;
	
	while (NULL != GetChild(node, LEFT))
	{
		node = GetChild(node, LEFT);;
	}
	
	return NodeToIter(node);
}

bst_iter_t BSTEnd(const bst_t *bst)
{
	assert(NULL != bst);
	
	return NodeToIter(&bst->dummy);
}

bst_iter_t BSTNext(bst_iter_t iter) 
{	
	bst_node_t *node = {0};
	
	assert(NULL != IterToNode(iter));
	
	node = IterToNode(iter);
	
	if (NULL != GetChild(node, RIGHT))
	{
		node = GetChild(node, RIGHT);
		
		while (NULL != GetChild(node, LEFT))
		{
			node = GetChild(node, LEFT);
		}
	}
	
	else
	{
		while (IsNodeALeftOrRightChild(node))
		{
			node = GetParent(node);
		}
		
		node = GetParent(node);
	}
	
	return NodeToIter(node);
}

bst_iter_t BSTPrev(bst_iter_t iter)
{
	bst_node_t *n_runner = NULL;
	
	assert(NULL != IterToNode(iter));
	
	n_runner = IterToNode(iter);
	
	if (NULL != GetChild(n_runner, LEFT))
	{
		n_runner = GetChild(n_runner, LEFT);
		
		while (NULL != GetChild(n_runner, RIGHT))
		{
			n_runner = GetChild(n_runner, RIGHT);
		}
	}
	
	else
	{
		while (!IsNodeALeftOrRightChild(n_runner))
		{
			n_runner = GetParent(n_runner);
		}
		
		n_runner = GetParent(n_runner);
	}
	
	return NodeToIter(n_runner);
}

bst_iter_t BSTFind(bst_t *bst, const void *param)
{
	int child_side = 0;
	bst_node_t *n_runner = NULL;
	
	assert(NULL != bst);
	
	n_runner = GetRoot(bst);
	
	while (NULL != n_runner 
					&& 0 != bst->cmp((const void *)n_runner->data, param))
	{
		child_side = (0 > bst->cmp((const void *)n_runner->data, param));
		n_runner = GetChild(n_runner, child_side);
	}
	
	UNUSED((NULL == n_runner) && (n_runner = IterToNode(BSTEnd(bst))));  
	
	return n_runner;
}

int BSTForEach(bst_iter_t from, bst_iter_t to, bst_action action, void *param)
{
	int action_func_return_val = 0;
	
	assert(NULL != IterToNode(from));
	assert(NULL != IterToNode(to));
	assert(NULL != action);
	
	while (!BSTIsSameIter(from, to) && 0 == action_func_return_val)
	{
		action_func_return_val = action(IterToNode(from)->data, param);
		from = BSTNext(from);
	}
	
	return action_func_return_val;
}

int BSTIsEmpty(const bst_t *bst)
{
	assert(NULL != bst);
	
	return BSTIsSameIter(BSTBegin(bst), BSTEnd(bst));
}

static int CountingElements(void *data, void *param)
{
	UNUSED(data);
	
	++(*(size_t *)param);
	
	return 0;
}

size_t BSTSize(const bst_t *bst)
{
	size_t counter = 0;
	
	assert(NULL != bst);
	
	BSTForEach(BSTBegin(bst), BSTEnd(bst), CountingElements, &counter);
	
	return counter;
}

int BSTIsSameIter(bst_iter_t one, bst_iter_t other)
{
	assert(NULL != IterToNode(one));
	assert(NULL != IterToNode(other));
	
	return (IterToNode(one) == IterToNode(other));
}
