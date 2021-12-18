#ifndef __MIN_HEAP_H__
#define __MIN_HEAP_H__

typedef struct vertex {
    short m_idx;
    short m_dist;
} vertex_t;

typedef struct minheap {
    short *m_arr;
    short m_heap_size;
    short m_capacity;
} minheap_t;

extern minheap_t *minheap_alloc(short capacity);
extern void minheap_free(minheap_t *heap);
extern void minheap_heapify(minheap_t *heap, short i);
extern short minheap_extract_min(minheap_t *heap);
extern void minheap_update_key(minheap_t *heap, short i, short val);
extern void minheap_delete_key(minheap_t *heap, short idx);
extern void minheap_insert_key(minheap_t *heap, short val);

#endif
