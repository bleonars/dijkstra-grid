#include <stdlib.h>

#include "grid.h"

int main(int argc, char **argv)
{
    grid_t *grid = NULL;

    if (argc != 5 || !parse_grid_file(argv[1], &grid))
        return EXIT_FAILURE;

    print_grid(argv[2], grid);

    free_grid(grid);
    return EXIT_SUCCESS;
}
