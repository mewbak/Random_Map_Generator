
#ifndef ASTAR
#define ASTAR

#include "flare/Map.h"

struct path_type
{
    int  length;
    int* data;
};

bool path_find  (Map* map_pointer, path_type* path_pointer, int tile_start, int tile_end);
bool path_find  (Map* map_pointer, int tile_start, int tile_end);

#endif // ASTAR
