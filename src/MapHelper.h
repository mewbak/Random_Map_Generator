#ifndef MAP_HELPER
#define MAP_HELPER

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "flare/Map.h"

namespace TILESET
{
    enum TILESET {TILESET_CAVE, TILESET_DUNGEON, TILESET_GRASSLAND};
}

namespace Tile_Type
{
    enum Tile_Type {TILE_NONE, TILE_WALL, TILE_FLOOR, TILE_EXIT, TILE_DOOR, TILE_KEY, TILE_PATH};
}

#define TILE_ATTRIBUTE_NONE        0
#define TILE_ATTRIBUTE_PROCESSED   1

#define WALL_WIDTH                 1

#define DIRECTION_BIAS_THRESHOLD   32 // Higher numbers result in mazes with longer passages.
#define DIRECTION_BIAS_NONE        0
#define DIRECTION_BIAS_NORTH       1
#define DIRECTION_BIAS_EAST        2
#define DIRECTION_BIAS_SOUTH       3
#define DIRECTION_BIAS_WEST        4

namespace Algorithm_Type
{
    enum Algorithm_Type {GEN_ALGORITHM_C1,
                         GEN_ALGORITHM_M1,
                         GEN_ALGORITHM_D1,
                         GEN_ALGORITHM_D2,
                         GEN_ALGORITHM_D3,
                         GEN_ALGORITHM_T1};
}

#define ROOM_MIN_X                 11
#define ROOM_MIN_Y                 11
#define ROOM_MAX_X                 15
#define ROOM_MAX_Y                 15

struct FillData
{
    int  tile_data;
    bool tile_done;
    bool tile_join;
};

struct GenTile
{
    Point position;
    int   data;
};

struct MapGen
{
    int        no_of_tiles;
    short      w;
    short      h;
    GenTile* tile;
};

struct room_struct
{
    bool valid;
    int x;
    int y;
    int w;
    int h;
};

struct MapNode
{
    MapGen   data;
    MapNode* left;
    MapNode* right;
};

struct MapProperties
{
    Algorithm_Type::Algorithm_Type  algorithm;
    TILESET::TILESET                 tile_set;
    int             size_x;
    int             size_y;
    int             seed;
    bool            seed_set;
    bool            gen_exits;
    bool            gen_enemies;
    bool            gen_chests;
};

void map_check               (Map* map_pointer, maprow *layer);
int  map_gen_flood_fill_tile (Map* map_pointer, FillData* fill_data, int tile_number);
bool map_gen_flood_fill      (Map* map_pointer);

void MapGenerate(Map* map_pointer, MapProperties properties);
int findLayerByName(Map* map_pointer, std::string layer);

#endif // MAP_HELPER
