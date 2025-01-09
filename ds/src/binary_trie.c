/* 
	File: Trie.
	Status: APPROVED.
*/

#include <stdlib.h> /*malloc & free*/
#include <assert.h> /*assert*/

#include "binary_trie.h"   /*typedef*/

#define MAX_HEIGHT 32

typedef enum child_pos
{
    LEFT,
    RIGHT,
    NUM_OF_CHILDREN
}child_pos_t;

struct trie_node
{
    int is_free;
    trie_node_t* children[NUM_OF_CHILDREN];
};

static void SetChild(trie_node_t* node, trie_node_t* child_node
                                                    , child_pos_t child_pos)
{
    assert(node);

    node->children[child_pos] = child_node;
}

static trie_node_t *GetChild(const trie_node_t *node, child_pos_t pos)
{
	assert(NULL != node);
	
	return node->children[pos];
}

static int IsNodeFree(trie_node_t* node)
{
    assert(node);

    return node->is_free;
}

static int HasChild(trie_node_t* node)
{
    assert(node);

    return (NULL != GetChild(node, LEFT) || NULL != GetChild(node, RIGHT));
}

static uint32_t GetSide(uint32_t route)
{
    return (!!(route & (1 << (MAX_HEIGHT - 1))));
}

static void SetNodeAsUsed(trie_node_t* node)
{
    assert(node);

    node->is_free = 0;
}

static void SetNodeAsUnused(trie_node_t* node)
{
    assert(node);

    node->is_free = 1;
}

/* return 1 for allocation error, otherwise 0 */
static int RecursiveCreateSubtrie(trie_node_t* node, size_t height)
{
    int status = 0;
    
    trie_node_t* left_child = NULL;
    trie_node_t* right_child = NULL;
    
    if (!height)
    {
        return 0;
    }

    left_child = malloc(sizeof(trie_node_t));
    if (NULL == left_child)
    {
        return 1;
    }

    right_child = malloc(sizeof(trie_node_t));
    if (NULL == right_child)
    {
        free(left_child);
        return 1;
    }

    SetChild(node, left_child, LEFT);
    SetChild(node, right_child, RIGHT);

    left_child->is_free = 1;
    SetChild(left_child, NULL, LEFT);
    SetChild(left_child, NULL, RIGHT);

    right_child->is_free = 1;
    SetChild(right_child, NULL, LEFT);
    SetChild(right_child, NULL, RIGHT);

    status += RecursiveCreateSubtrie(left_child, (height - 1));
    status += RecursiveCreateSubtrie(right_child, (height - 1));

    return !!status;
}

trie_node_t* BTrieCreate(size_t height)
{
    trie_node_t* root = malloc(sizeof(trie_node_t));
    if (NULL == root)
    {
        return NULL;
    }
    
    if (RecursiveCreateSubtrie(root, height))
    {
        BTrieDestroy(root);
        return NULL;
    }

    return root;
}

static void RecursiveDestroy(trie_node_t* root)
{
    if (NULL == root)
    {
        return;
    }
    
    RecursiveDestroy(GetChild(root, LEFT));
    RecursiveDestroy(GetChild(root, RIGHT));

    free(root);
}

void BTrieDestroy(trie_node_t* root)
{
    RecursiveDestroy(root);
}

int GetNextNode(trie_node_t* node, uint32_t route, trie_route node_route)
{
    int is_free = 0;
    uint32_t side_pos = 0;
    
    if (!HasChild(node))
    {
        is_free = IsNodeFree(node);
        SetNodeAsUsed(node);
        
        return is_free;
    }

    side_pos = GetSide(route);

    *(uint32_t*)node_route <<= 1;

    if (IsNodeFree(GetChild(node, side_pos)))
    {
        *(uint32_t*)node_route |= side_pos;
        is_free = GetNextNode(GetChild(node, side_pos), (route << 1)
                                                                , node_route);
    }
    
    else
    {
       *(uint32_t*)node_route |= !side_pos;
        is_free = GetNextNode(GetChild(node, !side_pos), 0, node_route);
    }

    if (!IsNodeFree(GetChild(node, LEFT)) && !IsNodeFree(GetChild(node, RIGHT)))
    {
        SetNodeAsUsed(node);
    }

   return is_free;
}

static void RecursiveFindFreeLeaves(trie_node_t* node, size_t* counter)
{
    if (!HasChild(node))
    {
        *counter += IsNodeFree(node);

        return;
    }

    RecursiveFindFreeLeaves(GetChild(node, LEFT), counter);
    RecursiveFindFreeLeaves(GetChild(node, RIGHT), counter);  
}

size_t GetAmountOfFreeLeaves(trie_node_t* root)
{
    size_t counter = 0;

    assert(root);

    RecursiveFindFreeLeaves(root, &counter);

    return counter;
}

int FreeLeaf(trie_node_t* node, uint32_t route)
{
    int is_free = 0;
    
    assert(node);
    
    if (!HasChild(node))
    {
        is_free = IsNodeFree(node);
        SetNodeAsUnused(node);

        return is_free;
    }

    SetNodeAsUnused(node);
    
    return FreeLeaf(GetChild(node, GetSide(route)), (route << 1));
}
