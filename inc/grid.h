#ifndef __GRID_H__
#define __GRID_H__

typedef struct grid {
    short **m_grid;
    short m_rows;
    short m_cols;
} grid_t;

extern int parse_grid_file(char *filename, grid_t **grid);
extern void print_grid(char *filename, grid_t *grid);
extern void print_speeds(char *filename, grid_t *grid);
extern void free_grid(grid_t *grid);
extern void dijkstra(grid_t *grid, short start_idx, short **dist_ret, short **pred_ret);

#endif
