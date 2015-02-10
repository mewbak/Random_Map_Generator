/*
    Random Map Generator.
    Copyright (C) 2014  Paul Wortmann

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
*/

#include "MapGenerator_M1.h"

bool MapGenerator_M1::map_gen_maze_check_tile(Map* map_pointer, Point tile, int direction_bias)
{
    int intermediate = findLayerByName(map_pointer,"intermediate");
    if (intermediate == -1) return true;

    bool return_value = true;
    if (tile.y == 0 || tile.y == map_pointer->h-1 || tile.x == 0 || tile.x == map_pointer->w-1)
        return_value = false;
    if (map_pointer->layers[intermediate][tile.x][tile.y] == Tile_Type::TILE_FLOOR)
        return_value = false;
    int tile_count = tile.y * map_pointer->w + tile.x;
    if (return_value)
    {
        switch (direction_bias)
        {
            case DIRECTION_BIAS_NORTH:
                for (int wall_count = 1; wall_count <= WALL_WIDTH; wall_count++)
                {
                    int t[5];
                    t[0] = tile_count-wall_count;
                    t[1] = tile_count+wall_count;
                    t[2] = tile_count-map_pointer->w*wall_count;
                    t[3] = tile_count-map_pointer->w-wall_count;
                    t[4] = tile_count-map_pointer->w+wall_count;

                    for (int i = 0; i < 5; i++)
                    {
                        return_value = isWallTile(map_pointer, t[i]);
                    }
                }
            break;
            case DIRECTION_BIAS_SOUTH:
                for (int wall_count = 1; wall_count <= WALL_WIDTH; wall_count++)
                {
                    int t[5];
                    t[0] = tile_count-wall_count;
                    t[1] = tile_count+wall_count;
                    t[2] = tile_count+map_pointer->w*wall_count;
                    t[3] = tile_count+map_pointer->w-wall_count;
                    t[4] = tile_count+map_pointer->w+wall_count;

                    for (int i = 0; i < 5; i++)
                    {
                        return_value = isWallTile(map_pointer, t[i]);
                    }
                }
            break;
            case DIRECTION_BIAS_EAST:
                for (int wall_count = 1; wall_count <= WALL_WIDTH; wall_count++)
                {
                    int t[5];
                    t[0] = tile_count-wall_count;
                    t[1] = tile_count-map_pointer->w*wall_count;
                    t[2] = tile_count-map_pointer->w-wall_count;
                    t[3] = tile_count+map_pointer->w*wall_count;
                    t[4] = tile_count+map_pointer->w-wall_count;

                    for (int i = 0; i < 5; i++)
                    {
                        return_value = isWallTile(map_pointer, t[i]);
                    }
                }
            break;
            case DIRECTION_BIAS_WEST:
                for (int wall_count = 1; wall_count <= WALL_WIDTH; wall_count++)
                {
                    int t[5];
                    t[0] = tile_count+wall_count;
                    t[1] = tile_count-map_pointer->w*wall_count;
                    t[2] = tile_count-map_pointer->w+wall_count;
                    t[3] = tile_count+map_pointer->w*wall_count;
                    t[4] = tile_count+map_pointer->w+wall_count;

                    for (int i = 0; i < 5; i++)
                    {
                        return_value = isWallTile(map_pointer, t[i]);
                    }
                }
            break;
            case DIRECTION_BIAS_NONE:
            default:
                // throw error
                return_value = true;
            break;
        }
    }
    return (return_value);
}

bool MapGenerator_M1::isWallTile(Map* map_pointer, int tile)
{
    int intermediate = findLayerByName(map_pointer, "intermediate");
    if (intermediate == -1) return true;

    int x = tile % map_pointer->w;
    int y = tile / map_pointer->w;

    if (x >= 0 && y >= 0 && x < map_pointer->w && y < map_pointer->h && map_pointer->layers[intermediate][x][y] != Tile_Type::TILE_WALL)
    {
        return false;
    }
    return true;
}

void MapGenerator_M1::map_gen_maze(Map* map_pointer, int tile_x, int tile_y, int direction_bias)
{
    int intermediate = findLayerByName(map_pointer, "intermediate");
    if (intermediate == -1) return;

    map_pointer->layers[intermediate][tile_x][tile_y] = Tile_Type::TILE_FLOOR;
    int  tiles_available    = 1;
    bool new_direction_bias = false;
    int tile_direction      = 0;
    while (tiles_available > 0)
    {
        tiles_available = 0;
        Point tile_north     = Point(tile_x, tile_y-1);
        bool tile_north_ok   = map_gen_maze_check_tile(map_pointer, tile_north, DIRECTION_BIAS_NORTH);
        if (tile_north_ok) tiles_available++;

        Point tile_south     = Point(tile_x, tile_y+1);
        bool tile_south_ok   = map_gen_maze_check_tile(map_pointer, tile_south, DIRECTION_BIAS_SOUTH);
        if (tile_south_ok) tiles_available++;

        Point tile_east      = Point(tile_x-1, tile_y);
        bool tile_east_ok    = map_gen_maze_check_tile(map_pointer, tile_east, DIRECTION_BIAS_EAST);
        if (tile_east_ok) tiles_available++;

        Point tile_west      = Point(tile_x+1, tile_y);
        bool tile_west_ok    = map_gen_maze_check_tile(map_pointer, tile_west, DIRECTION_BIAS_WEST);
        if (tile_west_ok) tiles_available++;

        if ((new_direction_bias) || (direction_bias == DIRECTION_BIAS_NONE)) tile_direction = (rand() % 4) + 1;
        else
        {
            tile_direction = (rand()%(4+DIRECTION_BIAS_THRESHOLD))+1;
            if (tile_direction > 4) tile_direction = direction_bias;
            else
            {
                direction_bias = tile_direction;
                new_direction_bias = true;
            }
        }
        switch (tile_direction)
        {
            case DIRECTION_BIAS_NORTH:
                if (tile_north_ok)
                {
                    tiles_available--;
                    tile_north_ok = false;
                    map_gen_maze(map_pointer, tile_north.x, tile_north.y, direction_bias);
                }
            break;
            case DIRECTION_BIAS_SOUTH:
                if (tile_south_ok)
                {
                    tiles_available--;
                    tile_south_ok = false;
                    map_gen_maze(map_pointer, tile_south.x, tile_south.y, direction_bias);
                }
            break;
            case DIRECTION_BIAS_EAST:
                if (tile_east_ok)
                {
                    tiles_available--;
                    tile_east_ok = false;
                    map_gen_maze(map_pointer, tile_east.x, tile_east.y, direction_bias);
                }
            break;
            case DIRECTION_BIAS_WEST:
                if (tile_west_ok)
                {
                    tiles_available--;
                    tile_west_ok = false;
                    map_gen_maze(map_pointer, tile_west.x, tile_west.y, direction_bias);
                }
            break;
            default:
                // throw error?
                tiles_available--;
            break;
        }
    }
}

void MapGenerator_M1::Generate (Map* map_pointer, MapProperties properties)
{
    Prepare(map_pointer, properties);
    GenerateMap(map_pointer);
    applyTileset(map_pointer, properties.tile_set);
}

void MapGenerator_M1::GenerateMap(Map* map_pointer)
{
    if (findLayerByName(map_pointer,"intermediate") == -1)
    {
        maprow *current_layer = new maprow[map_pointer->w];
        map_pointer->layers.push_back(current_layer);
        map_pointer->layernames.push_back("intermediate");
    }
    int intermediate = findLayerByName(map_pointer,"intermediate");
    for (int j = 0; j < map_pointer->h; j++)
    {
        for (int i = 0; i < map_pointer->w; i++)
            map_pointer->layers[intermediate][i][j] = Tile_Type::TILE_WALL;
    }
    map_gen_maze(map_pointer,map_pointer->w/2,map_pointer->h/2,DIRECTION_BIAS_NONE);
    //map_gen_maze_check(map_pointer);
}
