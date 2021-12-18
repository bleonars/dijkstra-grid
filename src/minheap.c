#include <stdbool.h>
#include <stdlib.h>

#include "minheap.h"

vertex_t *vertex_alloc(short idx, short dist)
{
    vertex_t *v = (vertex_t *) malloc(sizeof(vertex_t));
    v->m_idx = idx;
    v->m_dist = dist;

    return v;
}

static short parent(short i) { return (i - 1) / 2; }
static short left(short i) { return (2 * i) + 1; }
static short right(short i) { return (2 * i) + 2; }
static void exchange(vertex_t **x, vertex_t **y) { vertex_t *t = *x; *x = *y; *y = t; }

minheap_t *minheap_alloc(short capacity)
{
    minheap_t *heap = (minheap_t *) malloc(sizeof(minheap_t));
    heap->m_vertices = (vertex_t **) malloc(sizeof(vertex_t *) * capacity);
    heap->m_indices = (short *) malloc(sizeof(short) * capacity);
    heap->m_heap_size = 0;
    heap->m_capacity = capacity;

    return heap;
}

void minheap_free(minheap_t *heap)
{
    if (heap->m_vertices)
        free(heap->m_vertices);

    if (heap->m_indices)
        free(heap->m_indices);

    if (heap)
        free(heap);
}

void minheap_update(minheap_t *heap, short grid_idx, short dist)
{
    short heap_idx = heap->m_indices[grid_idx];

    heap->m_vertices[heap_idx]->m_dist = dist;

    while (heap_idx &&
            heap->m_vertices[heap_idx]->m_dist < heap->m_vertices[parent(heap_idx)]->m_dist) {
        heap->m_indices[heap->m_vertices[heap_idx]->m_idx] = parent(heap_idx);
        heap->m_indices[heap->m_vertices[parent(heap_idx)]->m_idx] = heap_idx;

        exchange(&heap->m_vertices[heap_idx], &heap->m_vertices[parent(heap_idx)]);
        heap_idx = parent(heap_idx);
    }
}

static void minheap_heapify(minheap_t *heap, short idx)
{
    short smallest = idx;
    short l = left(idx);
    short r = right(idx);

    if (l < heap->m_heap_size && heap->m_vertices[l]->m_dist < 
            heap->m_vertices[smallest]->m_dist)
        smallest = l;

    if (r < heap->m_heap_size && heap->m_vertices[r]->m_dist < 
            heap->m_vertices[smallest]->m_dist)
        smallest = r;

    if (smallest != idx) {
        vertex_t *smallest_vtx = heap->m_vertices[smallest];
        vertex_t *index_vtx = heap->m_vertices[idx];

        heap->m_indices[smallest_vtx->m_idx] = idx;
        heap->m_indices[index_vtx->m_idx] = smallest;

        exchange(&heap->m_vertices[smallest], &heap->m_vertices[idx]);

        minheap_heapify(heap, smallest);
    }
}

vertex_t *minheap_extract_min(minheap_t *heap)
{
    if (minheap_empty(heap))
        return NULL;

    vertex_t *root = heap->m_vertices[0];
    vertex_t *last = heap->m_vertices[heap->m_heap_size - 1];

    heap->m_vertices[0] = last;
    heap->m_indices[root->m_idx] = heap->m_heap_size - 1;
    heap->m_indices[last->m_idx] = 0;

    heap->m_heap_size--;
    minheap_heapify(heap, 0);

    return root;
}

bool minheap_empty(minheap_t *heap)
{
    return heap->m_heap_size == 0;
}

bool minheap_contains(minheap_t *heap, short grid_idx)
{
    return heap->m_indices[grid_idx] < heap->m_heap_size;
}
