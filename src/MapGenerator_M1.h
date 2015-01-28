
#ifndef MAP_GEN_M1_HPP
#define MAP_GEN_M1_HPP

#include "Map.h"
#include "MapHelper.h"

bool map_gen_maze_check_tile (Map* map_pointer, int tile_number);
void map_gen_maze            (Map* map_pointer, int tile_in);
void map_gen_maze            (Map* map_pointer, int tile_x, int tile_y);
void map_gen_M1              (Map* map_pointer, int seed);
void map_gen_M1              (Map* map_pointer);

#endif //MAP_GEN_M1_HPP

