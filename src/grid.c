#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "grid.h"
#include "minheap.h"

int parse_grid_file(char *filename, grid_t **grid)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
        return 0;

    short rows, cols;
    fread(&rows, sizeof(short), 1, file);
    fread(&cols, sizeof(short), 1, file);

    *grid = (grid_t *) malloc(sizeof(grid_t));
    (*grid)->m_grid = (short **) malloc(rows * sizeof(short *));

    for (short i = 0; i < rows; ++i) {
        (*grid)->m_grid[i] = (short *) malloc(cols * sizeof(short));
        fread((*grid)->m_grid[i], sizeof(short), cols, file);
    }

    (*grid)->m_rows = rows;
    (*grid)->m_cols = cols;

    fclose(file);
    return 1;
}

void print_grid(char *filename, grid_t *grid)
{
    FILE *file = fopen(filename, "w");

    fprintf(file, "%hd %hd\n", grid->m_rows, grid->m_cols);

    for (short i = 0; i < grid->m_rows; ++i) {
        for (short j = 0; j < grid->m_cols; ++j) {
            fprintf(file, "%hd", grid->m_grid[i][j]);

            if (j != grid->m_cols - 1) {
                fprintf(file, " ");
            }
        }

        fprintf(file, "\n");
    }

    fclose(file);
}

static void path_recv(FILE *file, short *pred, int idx, short cols)
{
    short x, y;

    if (pred[idx] == -1) {
        x = idx / cols;
        y = idx % cols;
        fwrite(&x, sizeof(short), 1, file); 
        fwrite(&y, sizeof(short), 1, file);
        return;
    }

    path_recv(file, pred, pred[idx], cols);

    x = idx / cols;
    y = idx % cols;

    fwrite(&x, sizeof(short), 1, file); 
    fwrite(&y, sizeof(short), 1, file);
}

static void count_path_recv(short *pred, int idx, int *path_count)
{
    if (pred[idx] == -1) {
        (*path_count)++;
        return;
    }

    count_path_recv(pred, pred[idx], path_count);

    (*path_count)++;
}

static void print_paths(char *filename, int fastest_time, short fastest_idx, short *pred, short cols)
{
    FILE *file = fopen(filename, "wb");
    fwrite(&fastest_time, sizeof(int), 1, file);

    int path_count = 0;
    count_path_recv(pred, fastest_idx, &path_count);
    fwrite(&path_count, sizeof(int), 1, file);

    path_recv(file, pred, fastest_idx, cols);

    fclose(file);
}

void print_dijkstra(char *filename_times, char *filename_paths, grid_t *grid)
{
    FILE *file = fopen(filename_times, "wb");
    fwrite(&grid->m_cols, sizeof(short), 1, file);

    short num_vertices = grid->m_cols * grid->m_rows;

    short *overall_dist = NULL;
    short *overall_pred = NULL;
    int overall_time = INT_MAX;
    short overall_idx = -1;

    for (short i = 0; i < grid->m_cols; ++i) {
        short *dist = NULL;
        short *pred = NULL;

        dijkstra(grid, i, &dist, &pred);

        short *last_col = (short *) malloc(sizeof(short) * grid->m_cols);
        memcpy(last_col, dist + num_vertices - grid->m_cols, grid->m_cols * sizeof(short));

        int fastest_time = INT_MAX;
        short fastest_idx = -1;
        for (short j = 0; j < grid->m_cols; ++j) {
            if (last_col[j] < fastest_time) {
                fastest_time = (int) last_col[j];
                fastest_idx = num_vertices - grid->m_cols + j;
            }
        }
        fwrite(&fastest_time, sizeof(int), 1, file);

        if (fastest_time < overall_time) {
            if (overall_dist) {
                free(overall_dist);
            } 

            if (overall_pred) {
                free(overall_pred);
            }

            overall_time = fastest_time;
            overall_idx = fastest_idx;

            overall_dist = dist;
            overall_pred = pred;
        }
        else {
            free(pred);
            free(dist);
        }

        free(last_col);
    }

    print_paths(filename_paths, overall_time, overall_idx, overall_pred, grid->m_cols);
    free(overall_dist);
    free(overall_pred);

    fclose(file);
}

void free_grid(grid_t *grid)
{
    for (int i = 0; i < grid->m_rows; ++i)
        free(grid->m_grid[i]);

    free(grid->m_grid);
    free(grid);
}

void dijkstra(grid_t *grid, short start_idx, short **dist_ret, short **pred_ret)
{
    short num_vertices = grid->m_rows * grid->m_cols;
    minheap_t *heap = minheap_alloc(num_vertices);

    short *dist = (short *) malloc(sizeof(short) * num_vertices);
    short *pred = (short *) malloc(sizeof(short) * num_vertices);

    for (short i = 0; i < num_vertices; ++i) {
        dist[i] = SHRT_MAX; pred[i] = -1;
        heap->m_vertices[i] = vertex_alloc(i, dist[i]);
        heap->m_indices[i] = i;
    }

    heap->m_heap_size = num_vertices;

    dist[start_idx] = grid->m_grid[start_idx / grid->m_cols][start_idx % grid->m_cols];
    minheap_update(heap, start_idx, dist[start_idx]);

    while (!minheap_empty(heap)) {
        vertex_t *u = minheap_extract_min(heap);

        short u_x = u->m_idx / grid->m_cols;
        short u_y = u->m_idx % grid->m_cols;

        short adj_indices[4] = { -1, -1, -1, -1 };
        short adj_it = 0;

        if (u_x != grid->m_rows - 1) {
            adj_indices[adj_it] = (grid->m_cols * (u_x + 1)) + u_y;
            adj_it++;
        }

        if (u_y != 0) {
            adj_indices[adj_it] = (grid->m_cols * u_x) + u_y - 1;
            adj_it++;
        }

        if (u_y != grid->m_cols - 1) {
            adj_indices[adj_it] = (grid->m_cols * u_x) + u_y + 1;
            adj_it++;
        }

        if (u_x != 0) {
            adj_indices[adj_it] = (grid->m_cols * (u_x - 1)) + u_y;
            adj_it++;
        }

        for (short i = 0; i < 4; ++i)
        {
            if (adj_indices[i] != -1) {
                short adj_idx = adj_indices[i];

                short adj_x = adj_idx / grid->m_cols;
                short adj_y = adj_idx % grid->m_cols;

                if (minheap_contains(heap, adj_idx) && dist[u->m_idx] != SHRT_MAX && dist[adj_idx] > dist[u->m_idx] + grid->m_grid[adj_x][adj_y]) {
                    dist[adj_idx] = dist[u->m_idx] + grid->m_grid[adj_x][adj_y];
                    pred[adj_idx] = u->m_idx;

                    minheap_update(heap, adj_idx, dist[adj_idx]);
                }
            }
        }

        free(u);
    }

    *dist_ret = dist;
    *pred_ret = pred;

    minheap_free(heap);
}
