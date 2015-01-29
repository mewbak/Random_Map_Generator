#ifndef MAP_HELPER
#define MAP_HELPER

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "flare/Map.h"

enum TILESET {TILESET_CAVE = 0, TILESET_DUNGEON = 1, TILESET_GRASSLAND = 2};

#define OUTPUT_FORMAT_FLARE           0
#define OUTPUT_FORMAT_TILED           1
#define OUTPUT_FORMAT_FROST_AND_FLAME 2

enum Tile_Type {TILE_NONE = 0, TILE_WALL = 1, TILE_FLOOR = 2, TILE_EXIT = 3, TILE_DOOR = 4, TILE_KEY = 5, TILE_PATH = 6};

#define TILE_ATTRIBUTE_NONE        0
#define TILE_ATTRIBUTE_PROCESSED   1

#define WALL_WIDTH                 1

#define DIRECTION_BIAS_THRESHOLD   32 // Higher numbers result in mazes with longer passages.
#define DIRECTION_BIAS_NONE        0
#define DIRECTION_BIAS_NORTH       1
#define DIRECTION_BIAS_EAST        2
#define DIRECTION_BIAS_SOUTH       3
#define DIRECTION_BIAS_WEST        4

enum Algorithm_Type {GEN_ALGORITHM_C1 = 0,
                     GEN_ALGORITHM_M1 = 1,
                     GEN_ALGORITHM_D1 = 2,
                     GEN_ALGORITHM_D2 = 3,
                     GEN_ALGORITHM_D3 = 4,
                     GEN_ALGORITHM_T1 = 5};

#define ROOM_MAX                   64
#define ROOM_PATH                  2
#define ROOM_MIN_X                 7
#define ROOM_MIN_Y                 7
#define ROOM_MAX_X                 9
#define ROOM_MAX_Y                 9
#define ROOM_MAX_R                 (int)(sqrt(pow((double)(ROOM_MAX_X/2), 2) + pow((double)(ROOM_MAX_Y/2), 2)) + ROOM_PATH)

struct FillData
{
    int  tile_data;
    bool tile_done;
    bool tile_join;
};

struct MapGen
{
    int        no_of_tiles;
    short      w;
    short      h;
    GenTile* tile;
};

struct MapNode
{
    bool           leaf;
    Room           room;
    MapGen   data;
    MapNode* left;
    MapNode* right;
};

struct AppData
{
    int     progress;
    int     algorithm;
    int     tile_set;
    int     output_format;
    int     size_x;
    int     size_y;
    bool    gen_exits;
    bool    gen_enemies;
    bool    gen_chests;
    std::string file_name;
    std::string status;
};

void map_check                   (Map* map_pointer);
int  map_gen_flood_fill_tile     (Map* map_pointer, FillData* fill_data, int tile_number);
bool map_gen_room_flood_fill     (Map* map_pointer);
bool map_gen_room                (Map* map_pointer, Room room);
void map_gen_room_add            (Map* map_pointer);
void map_gen_room_exits          (Map* map_pointer);
int  map_gen_room_check_path     (Map* map_pointer, int room_1, int room_2, bool x_then_y);
void map_gen_room_path           (Map* map_pointer, int room_1, int room_2, bool x_then_y);
void map_gen_room_connect        (Map* map_pointer);
void map_gen_room_connect_2      (Map* map_pointer);
void map_gen_room_connect_path   (Map* map_pointer);
Room map_gen_room_find_stats(Map* map_pointer, FillData* fill_data, int tile_number);
void map_gen_room_find           (Map* map_pointer);
void map_gen_redundant_tile_check(Map* map_pointer);
void map_gen_map_exits           (Map* map_pointer);

void MapGenerate(Map* map_pointer, Algorithm_Type generation_algorithm, int dimension_x, int dimension_y);

#endif // MAP_HELPER