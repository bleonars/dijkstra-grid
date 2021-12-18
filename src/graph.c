#include "grid.h"
#include "minheap.h"
#include "graph.h"

void djikstra(graph_t *grid, short start_idx)
{
    short num_vertices = grid->m_rows * grid->m_cols;
    minheap_t *heap = minheap_alloc(num_vertices);

    short *dist = (short *) malloc(sizeof(short) * num_vertices);
    short *pred = (short *) malloc(sizeof(short) * num_vertices);

    for (short i = 0; i < num_vertices; ++i) {
        dist[i] = SHRT_MAX; pred[i] = -1;
        minheap_insert_key(
    }

    minheap_free(heap);
}
