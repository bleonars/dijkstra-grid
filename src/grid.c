#include <stdlib.h>
#include <stdio.h>

#include "grid.h"

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

void free_grid(grid_t *grid)
{
    for (int i = 0; i < grid->m_rows; ++i) {
        free(grid->m_grid[i]);
    }

    free(grid->m_grid);
    free(grid);
}
