/* 
	File: AVL (recursive bst balancing).
	Status: APPROVED.
*/

#include <stdlib.h>	/* malloc & free */
#include <assert.h>	/* assert */

#include "avl.h"	/* typedefs */

#define UNUSED(x) (void)(x)

typedef struct avl_node avl_node_t;

typedef enum child_pos
{
	LEFT = 0,
	RIGHT = 1,
	NUM_OF_CHILDREN = 2

}child_pos_t;

struct avl_node
{
	void *data;
	size_t height;
	avl_node_t *children[NUM_OF_CHILDREN];
};

struct avl
{
	avl_node_t *root;
    avl_comp_func cmp;
};

static void *GetData(const avl_node_t *node)
{
	assert(NULL != node);
	
	return node->data;
}

static size_t GetHeight(const avl_node_t *node)
{
	size_t height = 0;

	if(NULL != node)
	{
		height = node->height;
	}
	
	return height;
}

static avl_node_t *GetChild(const avl_node_t *node, child_pos_t pos)
{
	assert(NULL != node);
	
	return node->children[pos];
}

static avl_node_t *GetRoot(const avl_t *tree)
{
	assert(NULL != tree);
	
	return tree->root;
}

static void SetData(avl_node_t *node, void *data)
{
	assert(NULL != node);
	
	node->data = data;
}

static void SetHeight(avl_node_t *node, size_t height)
{
	assert(NULL != node);
	
	node->height = height;
}

static void SetChild(avl_node_t *node, child_pos_t pos
													, const avl_node_t *child)
{
	assert(NULL != node);
	
	node->children[pos] = (avl_node_t *)child;
}

static void SetNewNode(avl_node_t *node, void *data, size_t height
										, avl_node_t *left, avl_node_t *right)
{
	assert(NULL != node);

	SetData(node, data);
	SetHeight(node, height);
	SetChild(node, LEFT, left);
	SetChild(node, RIGHT, right);
}

static int HasRightChild(const avl_node_t *node)
{
	assert(NULL != node);
	
	return (NULL != GetChild(node, RIGHT));
}

static int HasLeftChild(const avl_node_t *node)
{
	assert(NULL != node);
	
	return (NULL != GetChild(node, LEFT));
}

static size_t HowManyChildren(const avl_node_t *node)
{
	assert(NULL != node);
	
	return ((NULL != GetChild(node, LEFT)) 
				+ (NULL != GetChild(node, RIGHT)));
}

static size_t HighestHeightOfChildren(avl_node_t *node)
{
	size_t child_left_height = 0;
	size_t child_right_height = 0;

	assert(NULL != node);

	child_left_height = GetHeight(GetChild(node, LEFT));
	child_right_height = GetHeight(GetChild(node, RIGHT));

	return (((child_left_height >= child_right_height)* child_left_height)
			+ ((child_right_height > child_left_height)* child_right_height));
}

static child_pos_t WhichChildIsHigher(avl_node_t *node)
{
	size_t child_left_height = 0;
	size_t child_right_height = 0;

	assert(NULL != node);

	child_left_height = GetHeight(GetChild(node, LEFT));
	child_right_height = GetHeight(GetChild(node, RIGHT));

	return (((child_left_height >= child_right_height)* LEFT)
			+ ((child_right_height > child_left_height)* RIGHT));
}

static int CheckBalance(avl_node_t *node)
{
	size_t child_left_height = 0;
	size_t child_right_height = 0;

	assert(NULL != node);

	child_left_height = GetHeight(GetChild(node, LEFT));
	child_right_height = GetHeight(GetChild(node, RIGHT));

	return (child_left_height - child_right_height);
}

static int IsBalanced(avl_node_t *node)
{
	assert(NULL != node);

	return ((1 >= CheckBalance(node)) && (-1 <= CheckBalance(node)));
}

static avl_node_t *RotateRight(avl_node_t *node)
{
	avl_node_t *new_root = NULL;
	avl_node_t *nodes_new_subtree = NULL;

	assert(NULL != node);

	new_root = GetChild(node, LEFT);
	nodes_new_subtree = GetChild(new_root, RIGHT);

	SetChild(new_root, RIGHT, node);
	SetChild(node, LEFT, nodes_new_subtree);

	SetHeight(node, (HighestHeightOfChildren(node) + 1));
	SetHeight(new_root, (HighestHeightOfChildren(new_root) + 1));

	return new_root;
}

static avl_node_t *RotateLeft(avl_node_t *node)
{
	avl_node_t *new_root = NULL;
	avl_node_t *nodes_new_subtree = NULL;

	assert(NULL != node);


	new_root = GetChild(node, RIGHT);
	nodes_new_subtree = GetChild(new_root, LEFT);

	SetChild(new_root, LEFT, node);
	SetChild(node, RIGHT, nodes_new_subtree);

	SetHeight(node, (HighestHeightOfChildren(node) + 1));
	SetHeight(new_root, (HighestHeightOfChildren(new_root) + 1));

	return new_root;
}

static avl_node_t *Rebalance(avl_node_t *node, child_pos_t child_side)
{
	avl_node_t *child_node = NULL;
	
	assert(NULL != node);

	child_node = GetChild(node, child_side);
	
	if (RIGHT == child_side)
	{
		if(LEFT == WhichChildIsHigher(child_node))
		{
			SetChild(node, RIGHT, RotateRight(child_node));
		}

		node = RotateLeft(node);
	}

	else
	{
		if(RIGHT == WhichChildIsHigher(child_node))
		{
			SetChild(node, LEFT, RotateLeft(child_node));
		}

		node = RotateRight(node);
	}

	return node;
}

avl_t *AVLCreate(avl_comp_func comp)
{
	avl_t *tree = NULL;
	
	assert(NULL != comp);
	
	tree = (avl_t *)malloc(sizeof(avl_t));
	if (NULL == tree)
	{
		return NULL;
	}

	tree->cmp = comp;
	tree->root = NULL;

	return tree;
}

static void RecursiveDestroy(avl_node_t *node)
{
	assert(NULL != node);
	
	if (0 != HowManyChildren(node))
	{
		if (HasRightChild(node))
		{
			RecursiveDestroy(GetChild(node, RIGHT));
			SetChild(node, RIGHT, NULL);
		}
	
		if (HasLeftChild(node))
		{
			RecursiveDestroy(GetChild(node, LEFT));
			SetChild(node, LEFT, NULL);
		}
	}
	
	free(node);
}

void AVLDestroy(avl_t *tree)
{
	assert(NULL != tree);
	
	RecursiveDestroy(GetRoot(tree));

	free(tree); 
}

static avl_node_t *RecursiveInsert(avl_comp_func comp, avl_node_t *node
												, avl_node_t *insert_node)
{
	child_pos_t child_side = 0;

	assert(NULL != comp);
	
	if (NULL == node)
	{
		return insert_node;
	}
	
	child_side =  (0 < comp(GetData(insert_node), GetData(node)));
	
	if (NULL != GetChild(node, child_side))
	{
		SetChild(node, child_side, RecursiveInsert(comp
				, GetChild(node, child_side), insert_node));

		SetHeight(node, HighestHeightOfChildren(node) + 1);

		if (!IsBalanced(node))
		{
			node = Rebalance(node, WhichChildIsHigher(node));
		}
	}

	else
	{
		SetChild(node, child_side, insert_node);
		SetHeight(node, HighestHeightOfChildren(node) + 1);
	}

	return node;
}

int AVLInsert(avl_t *tree, void *data)
{
	avl_node_t *insert_node = NULL;
	
	assert(NULL != tree);
	
	insert_node = (avl_node_t *)malloc(sizeof(avl_node_t));
	if (NULL == insert_node)
	{
		return 1;
	}
	
	SetNewNode(insert_node, data, 1, NULL, NULL);
	tree->root = RecursiveInsert(tree->cmp, GetRoot(tree), insert_node);
	
	return 0;
}

static avl_node_t *RecursiveGetLeftNext(avl_node_t *node)
{
	assert(NULL != node);

	if (NULL != GetChild(node, LEFT))
	{
		node = RecursiveGetLeftNext(GetChild(node, LEFT));
	}

	return node;
}

static avl_node_t *RecursiveRemove(avl_comp_func comp, avl_node_t *node
											, void **return_data, void *data)
{
	child_pos_t child_side = 0;

	assert(NULL != comp);
	
	if (NULL == node)
	{
		return node;
	}

	if (0 != comp(data, GetData(node)))
	{
		child_side = (0 < comp(data, GetData(node)));
		SetChild(node, child_side, RecursiveRemove(comp
							, GetChild(node, child_side), return_data, data));
		SetHeight(node, GetHeight(node) + 1);

		if (!IsBalanced(node))
		{
			node = Rebalance(node, WhichChildIsHigher(node));
		}
	}

	else /* found the node to remove */
	{
		avl_node_t *tmp_node = NULL;
		avl_node_t *next_node = NULL;

		if (NULL == *return_data)
		{
			*return_data = GetData(node);
		}

		/*case of 1 child from right or 0 children*/
		if (NULL == GetChild(node, LEFT))
		{
			tmp_node = GetChild(node, RIGHT);
			free(node);

			return tmp_node;
		}
		
		/*case of 1 child from left*/
		if (NULL == GetChild(node, RIGHT))
		{
			tmp_node = GetChild(node, LEFT);
			free(node);

			return tmp_node;
		}

		/*case of 2 children*/
		next_node = RecursiveGetLeftNext(GetChild(node, RIGHT));

		SetData(node, GetData(next_node));
		SetChild(node, RIGHT, RecursiveRemove(comp, GetChild(node, RIGHT)
											, return_data, GetData(next_node)));
	}

	return node;
}

void *AVLRemove(avl_t *tree, const void *data)
{
	void *return_data = NULL;

	assert(NULL != tree);
	
	tree->root = RecursiveRemove(tree->cmp, GetRoot(tree), &return_data
																, (void *)data);
	return return_data;
}

static int ActionSize(void *data, void *param)
{
	UNUSED(data);
	
	++(*(size_t *)param);
	
	return 0;
}

size_t AVLSize(const avl_t *tree)
{
	size_t counter = 0;
	
	assert(NULL != tree);

	AVLForEach((avl_t *)tree, PRE_ORDER, ActionSize, &counter);
	
	return counter;
}

int AVLIsEmpty(const avl_t *tree)
{
	assert(NULL != tree);
	
	return (NULL == GetRoot(tree));
}

static avl_node_t *RecursiveFind(avl_comp_func comp, avl_node_t *node
																, void *data)
{
	child_pos_t child_side = 0;

	assert(NULL != comp);
	
	if (NULL == node)
	{
		return NULL;
	}
	
	child_side =  (0 < comp(data, GetData(node)));
	
	if (0 != comp(GetData(node), data))
	{
		return RecursiveFind(comp, GetChild(node, child_side), data); 
	}
	
	return node;
}

void *AVLFind(const avl_t *tree, const void *param)
{
	avl_node_t *node = NULL;
	void *data = NULL;	

	assert(NULL != tree);

	node = RecursiveFind(tree->cmp, GetRoot(tree), (void *)param);
	UNUSED((NULL != node) && (data = GetData(node)));

	return data;
}

static int RecursivePreOrderTraversal(avl_action_func action, avl_node_t *node
																, void *params)
{
	int action_func_status = 0;
	
	assert(NULL != action);

	action_func_status = action(GetData(node), params);

	UNUSED((NULL != GetChild(node, LEFT)) && (0 == action_func_status) 
			&& (action_func_status  = RecursivePreOrderTraversal(action
											, GetChild(node, LEFT), params)));

	UNUSED((NULL != GetChild(node, RIGHT)) && (0 == action_func_status) 
				&& (action_func_status  = RecursivePreOrderTraversal(action
											, GetChild(node, RIGHT), params)));

	return action_func_status;

}

static int RecursiveInOrderTraversal(avl_action_func action, avl_node_t *node
																, void *params)
{
	int action_func_status = 0;
	
	assert(NULL != action);

	UNUSED((NULL != GetChild(node, LEFT)) && (0 == action_func_status) 
				&& (action_func_status  = RecursivePreOrderTraversal(action
											, GetChild(node, LEFT), params)));

	UNUSED((0 == action_func_status)
				&& (action_func_status = action(GetData(node), params)));

	UNUSED((NULL != GetChild(node, RIGHT)) && (0 == action_func_status) 
				&& (action_func_status  = RecursivePreOrderTraversal(action
											, GetChild(node, RIGHT), params)));

	return action_func_status;
}

static int RecursivePostOrderTraversal(avl_action_func action, avl_node_t *node
																, void *params)
{
	int action_func_status = 0;
	
	assert(NULL != action);

	UNUSED((NULL != GetChild(node, LEFT)) && (0 == action_func_status) 
			&& (action_func_status  = RecursivePreOrderTraversal(action
											, GetChild(node, LEFT), params)));
	
	UNUSED((NULL != GetChild(node, RIGHT)) && (0 == action_func_status) 
			&& (action_func_status  = RecursivePreOrderTraversal(action
											, GetChild(node, RIGHT), params)));

	UNUSED((0 == action_func_status)
				&& (action_func_status = action(GetData(node), params)));

	return action_func_status;
}

int AVLForEach(avl_t *tree, bst_traversal_t order
									, avl_action_func action, void *params)
{
	int action_func_status = 0;

	assert(NULL != tree);
	assert(NULL != action);
	assert(0 <= order);
	assert(2 >= order);
	
	if (!AVLIsEmpty(tree))
	{
		switch (order)
		{
			case PRE_ORDER:
				action_func_status = RecursivePreOrderTraversal(action
													, GetRoot(tree), params);
				break;
			
			case IN_ORDER:
				action_func_status = RecursiveInOrderTraversal(action
													, GetRoot(tree), params);
				break;
			
			case POST_ORDER:
				action_func_status = RecursivePostOrderTraversal(action
													, GetRoot(tree), params);
				break;
		}
	}

	return action_func_status;
}

size_t AVLHeight(const avl_t *tree)
{
	assert(NULL != tree);

	return GetHeight(GetRoot(tree));
}