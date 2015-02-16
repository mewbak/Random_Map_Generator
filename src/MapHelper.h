/*
    Random Map Generator.
    Copyright (C) 2014  Paul Wortmann
    Copyright (C) 2015  Igor Paliychuk

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    paul.wortmann@gmail.com
    mansonigor@gmail.com
*/

#ifndef MAP_HELPER
#define MAP_HELPER

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "flare/Map.h"

#define WALL_WIDTH                 3

#define EXITS_COUNT                4

#define ROOM_MIN_X                 11
#define ROOM_MIN_Y                 11
#define ROOM_MAX_X                 15
#define ROOM_MAX_Y                 15

#define DIRECTION_BIAS_THRESHOLD   32 // Higher numbers result in mazes with longer passages.
#define DIRECTION_BIAS_NONE        0
#define DIRECTION_BIAS_NORTH       1
#define DIRECTION_BIAS_EAST        2
#define DIRECTION_BIAS_SOUTH       3
#define DIRECTION_BIAS_WEST        4

namespace Tile_Type
{
    enum Tile_Type {TILE_NONE,
                    TILE_WALL,
                    TILE_FLOOR,
                    TILE_EXIT,
                    TILE_DOOR,
                    TILE_KEY,
                    TILE_PATH,
                    TILE_ROOM,
                    TILE_WATER,
                    TILE_ROAD,
                    TILE_BRIDGE,
                    TILE_HOUSE};
}

namespace Algorithm_Type
{
    enum Algorithm_Type {GEN_ALGORITHM_C1,
                         GEN_ALGORITHM_M1,
                         GEN_ALGORITHM_D1,
                         GEN_ALGORITHM_D2,
                         GEN_ALGORITHM_D3,
                         GEN_ALGORITHM_T1};
}

struct FindData
{
    int        no_of_tiles;
    short      w;
    short      h;
    short      x;
    short      y;
    int*       tile;
};

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
    std::string     tile_set;
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
void map_gen_find            (Map* map_pointer, FindData* find_data, Point* location);
void map_gen_find_replace    (Map* map_pointer, FindData* find_data, FindData* replace_data);
void map_gen_exits           (Map* map_pointer);
void map_gen_river           (Map* map_pointer);
void map_gen_road            (Map* map_pointer);
void map_gen_bridge          (Map* map_pointer);

void MapGenerate(Map* map_pointer, MapProperties properties);
int findLayerByName(Map* map_pointer, std::string layer);

#endif // MAP_HELPER
