/* 
	Developer: Roy Steiner.
	File: Heap.
	Reviewer: Shay.
	Date: 19/08/2024.
	Status: APPROVED.
*/
#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc & free*/

#include "heap.h" /*typedef & size_t*/
#include "vector.h" /*API*/

#define INITIATE_ALLOC 2
#define NUM_OF_CHILDREN 2

struct heap
{
   heap_comp_func comp;
   vector_ptr_t vec;
};

static heap_comp_func GetCompFunc(const heap_t* heap)
{
    assert(heap);

    return heap->comp;
}

static vector_ptr_t GetVector(const heap_t* heap)
{
    assert(heap);

    return heap->vec;
}

static size_t GetChildLeft(size_t index)
{
    return (NUM_OF_CHILDREN * index + 1);
}

static size_t GetChildRight(size_t index)
{
    return (NUM_OF_CHILDREN * index + NUM_OF_CHILDREN);
}

static size_t GetParent(size_t index)
{
    return ((index - 1) >> 1);
}

static void SwapData(vector_ptr_t vec, size_t one_index, size_t other_index)
{
    void* one_data = NULL;
    void* other_data = NULL;
    
    assert(vec);

    VectorGetVal(vec, &one_data, one_index);
    VectorGetVal(vec, &other_data, other_index);

    VectorAssign(vec, other_index, &one_data);
    VectorAssign(vec, one_index, &other_data);
}

static void HeapifyUp(heap_t* heap)
{
    size_t child_index = 0;
    size_t parent_index = 0;
    
    void* child_data = NULL;
    void* parent_data = NULL;

    vector_ptr_t vec = NULL;
    heap_comp_func comp = NULL;

    assert(heap);

    vec = GetVector(heap);
    comp = GetCompFunc(heap);

    child_index = (HeapSize(heap) - 1);
    parent_index = GetParent(child_index);
    
    VectorGetVal(vec, &child_data, child_index);

    while (child_index > 0)
    {
        VectorGetVal(vec, &parent_data, parent_index);
        
        if (0 >= comp(parent_data, child_data))
        {
            break;
        }

        VectorAssign(vec, child_index, &parent_data);
        child_index = parent_index;
        parent_index = GetParent(child_index);
    }

    VectorAssign(vec, child_index, &child_data);
}

static void HeapifyDown(heap_t* heap, size_t parent_index)
{
    size_t size = 0;
    size_t optional_parent_index = 0;
    size_t left_child_index = GetChildLeft(parent_index);
    size_t right_child_index = GetChildRight(parent_index); 

    void* left_child_data = NULL;
    void* right_child_data = NULL;
    void* optional_parent_data = NULL;
    
    vector_ptr_t vec = NULL;
    heap_comp_func comp = NULL;

    assert(heap);

    vec = GetVector(heap);
    comp = GetCompFunc(heap);
    size = HeapSize(heap);

    while (1)
    {
        left_child_index = GetChildLeft(parent_index);
        right_child_index = GetChildRight(parent_index); 
        optional_parent_index = parent_index;

        if (left_child_index < size)
        {
            VectorGetVal(vec, &left_child_data, left_child_index);
            VectorGetVal(vec, &optional_parent_data, optional_parent_index);
            
            if (0 >= comp(left_child_data, optional_parent_data))
            {
                optional_parent_index = left_child_index;
            }
        }

        if (right_child_index < size)
        {
            VectorGetVal(vec, &right_child_data, right_child_index);
            VectorGetVal(vec, &optional_parent_data, optional_parent_index);
            
            if (0 >= comp(right_child_data, optional_parent_data))
            {
                optional_parent_index = right_child_index;
            }
        }

        if (optional_parent_index == parent_index)
        {
            return;
        }

        SwapData(vec, parent_index, optional_parent_index);
        parent_index = optional_parent_index;
    }
}

heap_t* HeapCreate(heap_comp_func comp_func)
{
    heap_t* heap = NULL;

    assert(comp_func);

    heap = (heap_t*)malloc(sizeof(heap_t));
    if (NULL == heap)
    {
        return NULL;
    }
    
    heap->vec = VectorCreate(sizeof(void*), INITIATE_ALLOC);
    if (NULL == heap->vec)
    {
        free(heap);
        return NULL;
    }
    
    heap->comp = comp_func;

    return heap;
}

void HeapDestroy(heap_t* heap)
{
    assert(heap);

    VectorDestroy(GetVector(heap));

    free(heap);
}

int HeapPush(heap_t* heap, const void* data)
{
    assert(heap);

    if (VectorAppend(GetVector(heap), &data))
    {
        return 1;
    }
    
    HeapifyUp(heap);
    
    return 0;
}

void* HeapPop(heap_t* heap)
{
    void* data = NULL;

    assert(heap);

    SwapData(GetVector(heap), 0, (HeapSize(heap) - 1));
    VectorPop(GetVector(heap), &data);
    HeapifyDown(heap, 0);

    return data;
}

void* HeapPeek(const heap_t* heap)
{
    void* data = NULL;

    assert(heap);

    VectorGetVal(GetVector(heap), &data, 0);

    return data;
}

size_t HeapSize(const heap_t* heap)
{
    assert(heap);

    return VectorSize(GetVector(heap));
}

int HeapIsEmpty(const heap_t* heap)
{
    assert(heap);

    return VectorIsEmpty(GetVector(heap));
}

void* HeapRemove(heap_t* heap, int (*is_match)(const void* data
                                    , const void* param), const void* param)
{
    size_t i = 0;

    void* element_data = NULL;
    void* removed_data = NULL;

    assert(heap);
    assert(is_match);

    if (!HeapIsEmpty(heap))
    {
        VectorGetVal(GetVector(heap), &element_data, 0);
    }
    
    for (i = 1 ; (i < HeapSize(heap)) && !is_match(param, element_data) ; ++i)
    {
        VectorGetVal(GetVector(heap), &element_data, i);
    }

    if (is_match(param, element_data))
    {
        --i;
        SwapData(GetVector(heap), i, (HeapSize(heap) - 1));
        VectorPop(GetVector(heap), &removed_data);
        HeapifyDown(heap, i);   
    }

    return removed_data;
}
