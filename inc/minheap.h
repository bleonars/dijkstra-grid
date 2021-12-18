#ifndef __MIN_HEAP_H__
#define __MIN_HEAP_H__

#include <stdbool.h>

typedef struct vertex {
    short m_idx;
    short m_dist;
} vertex_t;

extern vertex_t *vertex_alloc(short idx, short dist);

typedef struct minheap {
    struct vertex **m_vertices;
    short *m_indices;
    short m_heap_size;
    short m_capacity;
} minheap_t;

extern minheap_t *minheap_alloc(short capacity);
extern void minheap_free(minheap_t *heap);
extern void minheap_update(minheap_t *heap, short grid_idx, short dist); 
extern vertex_t *minheap_extract_min(minheap_t *heap);
extern bool minheap_empty(minheap_t *heap);
extern bool minheap_contains(minheap_t *heap, short grid_idx);

#endif
