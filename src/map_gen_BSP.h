
#ifndef MAP_GEN_BSP_HPP
#define MAP_GEN_BSP_HPP

#include "Map.h"
#include "MapHelper.h"

void           map_gen_BSP_split       (Map* map_pointer, MapNode *map_node);
void           map_gen_BSP             (Map* map_pointer);
void           map_gen_BSP_internal    (Map* map_pointer);
void           map_gen_BSP             (Map* map_pointer, int seed);

#endif //MAP_GEN_BSP_HPP

