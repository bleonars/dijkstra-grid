#include "minheap.h"

static short parent(short idx)
{
    return (idx - 1) / 2;
}

static short left(short idx)
{
    return (idx * 2) + 1;
}

static short right(short idx)
{
    return (idx * 2) + 2;
}

static void swap(short *x, short *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

minheap_t *minheap_alloc(short capacity)
{
    minheap_t *heap = (minheap_t *) malloc(sizeof(minheap_t));
    heap->m_capacity = capacity;
    heap->m_heap_size = 0;

    heap->m_arr = (short *) malloc(heap->m_capacity * sizeof(short));
    return heap;
}

void minheap_free(minheap_t *heap)
{
    free(heap->m_arr);
    free(heap);
}

void minheap_heapify(minheap_t *heap, short idx)
{
    int smallest = idx;

    int l = left(idx);
    if (l < heap->m_heap_size && heap->m_arr[l] < heap->m_arr[idx])
        smallest = l;

    int r = right(idx);
    if (r < heap->m_heap_size && heap->m_arr[r] < heap->m_arr[smallest])
        smallest = r;

    if (smallest != idx) {
        swap(&(heap->m_arr[idx]), &(heap->m_arr[smallest]));
        minheap_heapify(heap, smallest);
    }
}

short minheap_extract_min(minheap_t *heap)
{
    if (heap->m_heap_size <= 0)
        return SHRT_MAX;

    if (heap->m_heap_size == 1) {
        heap->m_heap_size--;
        return heap->m_arr[0];
    }
  
    int root = heap->m_arr[0];
    heap->m_arr[0] = heap->m_arr[heap->m_heap_size - 1];
    heap->m_heap_size--;
    minheap_heapify(heap, 0);
  
    return root;
}

void minheap_update_key(minheap_t *heap, short idx, short val)
{
    heap->m_arr[idx] = val;

    while (idx != 0 && heap->m_arr[parent(idx)] > heap->m_arr[idx])
    {
        swap(&(heap->m_arr[idx]), &(heap->m_arr[parent(idx)]));
        idx = parent(idx);
    }
}

void minheap_delete_key(minheap_t *heap, short idx)
{
    minheap_update_key(heap, idx, SHRT_MIN);
    minheap_extract_min(heap);
}

void minheap_insert_key(minheap_t *heap, short val)
{
    if (heap->m_heap_size == heap->m_capacity)
        return;

    heap->m_heap_size++;
    int idx = heap->m_heap_size - 1;
    heap->m_arr[idx] = val;

    while (idx != 0 && heap->m_arr[parent(idx)] > heap->m_arr[idx]) {
        swap(&(heap->m_arr[idx]), &(heap->m_arr[parent(idx)]));
        idx = parent(idx);
    }
}
