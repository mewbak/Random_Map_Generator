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

#include <iostream>
#include "flare/Map.h"
#include "MapHelper.h"
#include "MapGenerator.h"
#include "MapGenerator_C1.h"
#include "MapGenerator_M1.h"
#include "MapGenerator_D1.h"
#include "MapGenerator_D2.h"
#include "MapGenerator_D3.h"
#include "MapGenerator_T1.h"

void MapGenerate(Map* map_pointer, MapProperties properties)
{
    MapGenerator* generator = NULL;
    if (properties.algorithm == Algorithm_Type::GEN_ALGORITHM_C1) {
        generator = new MapGenerator_C1();
    }
    else if (properties.algorithm == Algorithm_Type::GEN_ALGORITHM_M1) {
        generator = new MapGenerator_M1();
    }
    else if (properties.algorithm == Algorithm_Type::GEN_ALGORITHM_D1) {
        generator = new MapGenerator_D1();
    }
    else if (properties.algorithm == Algorithm_Type::GEN_ALGORITHM_D2) {
        generator = new MapGenerator_D2();
    }
    else if (properties.algorithm == Algorithm_Type::GEN_ALGORITHM_D3) {
        generator = new MapGenerator_D3();
    }
    else if (properties.algorithm == Algorithm_Type::GEN_ALGORITHM_T1) {
        generator = new MapGenerator_T1();
    }
    generator->Generate(map_pointer, properties);
    delete generator;
}

bool map_gen_is_wall(Map* map_pointer, int tile_x, int tile_y)
{
    int intermediate = findLayerByName(map_pointer,"intermediate");
    if (intermediate == -1) return false;
    if ((tile_x < 0) || (tile_x >= map_pointer->w) || (tile_y < 0) || (tile_y >= map_pointer->h))
        return true;
    return ((map_pointer->layers[intermediate][tile_x][tile_y] == Tile_Type::TILE_WALL)
            || (map_pointer->layers[intermediate][tile_x][tile_y] == Tile_Type::TILE_NONE)
            || (map_pointer->layers[intermediate][tile_x][tile_y] == Tile_Type::TILE_EXIT));
}

void map_gen_remove_extra_wall(Map* map_pointer)
{
    int intermediate = findLayerByName(map_pointer,"intermediate");
    if (intermediate == -1) return;
    for (int i = 0; i < map_pointer->w; i++)
    {
        for (int j = 0; j < map_pointer->h; j++)
        {

            if (map_gen_is_wall(map_pointer, i, j)
                    && map_gen_is_wall(map_pointer, i, j+1)
                    && map_gen_is_wall(map_pointer, i, j-1)
                    && map_gen_is_wall(map_pointer, i+1, j)
                    && map_gen_is_wall(map_pointer, i+1, j+1)
                    && map_gen_is_wall(map_pointer, i+1, j-1)
                    && map_gen_is_wall(map_pointer, i-1, j)
                    && map_gen_is_wall(map_pointer, i-1, j+1)
                    && map_gen_is_wall(map_pointer, i-1, j-1))
                        map_pointer->layers[intermediate][i][j] = Tile_Type::TILE_NONE;
        }
    }
}

void map_check (Map* map_pointer)
{
    int index = findLayerByName(map_pointer,"intermediate");

    if (index == -1)
        return;

    Map_Layer& layer = map_pointer->layers[index];

    int iterations  = 2;
    for (int k = 0; k < iterations; k++)
    {
        for (int i = 1; i < map_pointer->h-1; i++)
        {
            for (int j = 1; j < map_pointer->w-1; j++)
            {
                if ((layer)[i][j] == Tile_Type::TILE_WALL)
                {
                    // Remove single tiles
                    // OOO
                    // OXO
                    // OOO
                    //--------------------
                    if (((layer)[i][j+1] == Tile_Type::TILE_FLOOR)
                            && ((layer)[i][j-1] == Tile_Type::TILE_FLOOR)
                            && ((layer)[i+1][j] == Tile_Type::TILE_FLOOR)
                            && ((layer)[i+1][j+1] == Tile_Type::TILE_FLOOR)
                            && ((layer)[i+1][j-1] == Tile_Type::TILE_FLOOR)
                            && ((layer)[i-1][j] == Tile_Type::TILE_FLOOR)
                            && ((layer)[i-1][j+1] == Tile_Type::TILE_FLOOR)
                            && ((layer)[i-1][j-1] == Tile_Type::TILE_FLOOR))
                    {
                        (layer)[i][j] = Tile_Type::TILE_FLOOR;
                    }
                    // Remove single horizontal tiles
                    // ???
                    // OXO
                    // ???
                    //--------------------
                    if (((layer)[i][j+1] == Tile_Type::TILE_FLOOR)
                            && ((layer)[i][j-1] == Tile_Type::TILE_FLOOR))
                    {
                        (layer)[i][j] = Tile_Type::TILE_FLOOR;
                    }
                    // Remove single vertical tiles
                    // ?O?
                    // ?X?
                    // ?O?
                    //--------------------
                    if (((layer)[i+1][j] == Tile_Type::TILE_FLOOR)
                            && ((layer)[i-1][j] == Tile_Type::TILE_FLOOR))
                    {
                        (layer)[i][j] = Tile_Type::TILE_FLOOR;
                    }
                    // Remove cross right tiles
                    // O?X
                    // ?X?
                    // X?O
                    //--------------------
                    if (((layer)[i+1][j+1] == Tile_Type::TILE_WALL)
                            && ((layer)[i+1][j-1] == Tile_Type::TILE_FLOOR)
                            && ((layer)[i-1][j+1] == Tile_Type::TILE_FLOOR)
                            && ((layer)[i-1][j-1] == Tile_Type::TILE_WALL))
                    {
                        (layer)[i+1][j-1] = Tile_Type::TILE_WALL;
                        (layer)[i-1][j+1] = Tile_Type::TILE_WALL;
                        (layer)[i][j-1] = Tile_Type::TILE_WALL;
                        (layer)[i][j-1] = Tile_Type::TILE_WALL;
                        (layer)[i+1][j] = Tile_Type::TILE_WALL;
                        (layer)[i-1][j] = Tile_Type::TILE_WALL;
                    }
                    // Remove cross left tiles
                    // X?O
                    // ?X?
                    // O?X
                    //--------------------
                    if (((layer)[i+1][j+1] == Tile_Type::TILE_FLOOR)
                            && ((layer)[i+1][j-1] == Tile_Type::TILE_WALL)
                            && ((layer)[i-1][j+1] == Tile_Type::TILE_WALL)
                            && ((layer)[i-1][j-1] == Tile_Type::TILE_FLOOR))
                    {
                        (layer)[i+1][j+1] = Tile_Type::TILE_WALL;
                        (layer)[i-1][j-1] = Tile_Type::TILE_WALL;
                        (layer)[i][j-1] = Tile_Type::TILE_WALL;
                        (layer)[i][j-1] = Tile_Type::TILE_WALL;
                        (layer)[i+1][j] = Tile_Type::TILE_WALL;
                        (layer)[i-1][j] = Tile_Type::TILE_WALL;
                    }
                }
            }
        }
    }
}

bool map_gen_flood_fill (Map* map_pointer)
{
    int intermediate = findLayerByName(map_pointer,"intermediate");
        if (intermediate == -1) return false;
    int  floor_count  = 0;
    int  first_floor  = -1;
    bool return_value = true;
    FillData* fill_data = new FillData[(map_pointer->w*map_pointer->h)];
    for (int j = 0; j < map_pointer->h; j++)
    {
        for (int i = 0; i < map_pointer->w; i++)
        {
            if (map_pointer->layers[intermediate][i][j] == Tile_Type::TILE_FLOOR) floor_count++;
            if ((floor_count == 1) && (first_floor < 0)) first_floor = (j*map_pointer->w)+i;
            fill_data[(j*map_pointer->w)+i].tile_data = map_pointer->layers[intermediate][i][j];
            fill_data[(j*map_pointer->w)+i].tile_done = false;
            fill_data[(j*map_pointer->w)+i].tile_join = false;
        }
    }
    int   number_found = CheckJoiningTiles(map_pointer,fill_data,first_floor);
    for (int i = 0; i < (map_pointer->w*map_pointer->h); i++)
    {
        if (fill_data[i].tile_join) map_pointer->layers[intermediate][i%map_pointer->w][i/map_pointer->w] = Tile_Type::TILE_FLOOR;
        else map_pointer->layers[intermediate][i%map_pointer->w][i/map_pointer->w] = Tile_Type::TILE_WALL;
    }
    if (number_found < floor_count) return_value = false;
    delete[] fill_data;
    return (return_value);
}

int CheckTile(int map_size, FillData* fill_data, int tile_parent, int tile_check)
{
    if ((tile_check >=0)&&(tile_check < map_size))
    {
        if ((fill_data[tile_parent].tile_join)&&(fill_data[tile_check].tile_data == Tile_Type::TILE_FLOOR))
            fill_data[tile_check].tile_join = true;
    }
    return (fill_data[tile_check].tile_join) ? 1 : 0;
}

int CheckJoiningTiles(Map* map_pointer, FillData* fill_data, int tile_number)
{
    // function assumes calling from desired floor tile.
    int map_size = map_pointer->w * map_pointer->h;
    fill_data[tile_number].tile_join = true;
    unsigned int return_value = 1;
    for (int j = 0; j < map_pointer->w+map_pointer->h; j++)
    {
        for (int i = 0; i < map_size; i++)
        {
            return_value += CheckTile(map_size,fill_data,i,i-1);
            return_value += CheckTile(map_size,fill_data,i,i-map_pointer->w-1);
            return_value += CheckTile(map_size,fill_data,i,i-map_pointer->w);
            return_value += CheckTile(map_size,fill_data,i,i-map_pointer->w+1);
            return_value += CheckTile(map_size,fill_data,i,i+1);
            return_value += CheckTile(map_size,fill_data,i,i+map_pointer->w+1);
            return_value += CheckTile(map_size,fill_data,i,i+map_pointer->w);
            return_value += CheckTile(map_size,fill_data,i,i+map_pointer->w-1);
        }
    }
    return return_value;
}

int findLayerByName(Map* map_pointer, std::string layer)
{
    for (unsigned i = 0; i < map_pointer->layers.size(); ++i) {
        if (map_pointer->layernames[i] == layer) {
            return i;
        }
    }
    return -1;
}

void map_gen_find (Map* map_pointer, SectionData* find_data, Point* location)
{
    int intermediate = findLayerByName(map_pointer,"intermediate");
    if (intermediate == -1) return;
    for (int i = 0; i < map_pointer->w; i++)
    {
        for (int j = 0; j < map_pointer->h; j++)
        {
            bool found = true;
            for (int k = 0; k < find_data->w; k++)
            {
                for (int l = 0; l < find_data->h; l++)
                {
                    if (!((i+k < map_pointer->w) && (j+l < map_pointer->h)))
                        found = false;
                    else if (map_pointer->layers[intermediate][i+k][j+l] != find_data->tile[(find_data->w*l)+k])
                        found = false;
                }
            }
            if (found)
            {
                location->x = i;
                location->y = j;
                return;
            }
        }
    }
}

void map_gen_find_replace (Map* map_pointer, SectionData* find_data, SectionData* replace_data)
{
    int intermediate = findLayerByName(map_pointer,"intermediate");
    if (intermediate == -1) return;
    for (int i = 0; i < map_pointer->w; i++)
    {
        for (int j = 0; j < map_pointer->h; j++)
        {
            bool found = true;
            for (int k = 0; k < find_data->w; k++)
            {
                for (int l = 0; l < find_data->h; l++)
                {
                    if (!((i+k < map_pointer->w) && (j+l < map_pointer->h)))
                        found = false;
                    else if (map_pointer->layers[intermediate][i+k][j+l] != find_data->tile[(find_data->w*l)+k])
                        found = false;
                }
            }
            if (found)
            {
                for (int k = 0; k < find_data->w; k++)
                {
                    for (int l = 0; l < find_data->h; l++)
                        map_pointer->layers[intermediate][i+k][j+l] = replace_data->tile[(find_data->w*l)+k];
                }
                return;
            }
        }
    }
}

void map_gen_replace (Map* map_pointer, SectionData* replace_data, Point* location)
{
    int intermediate = findLayerByName(map_pointer,"intermediate");
    if (intermediate == -1) return;
    int i = location->x;
    int j = location->y;
    for (int k = 0; k < replace_data->w; k++)
    {
        for (int l = 0; l < replace_data->h; l++)
            if ((i+k < map_pointer->w) && (j+l < map_pointer->h))
                map_pointer->layers[intermediate][i+k][j+l] = replace_data->tile[(replace_data->w*l)+k];
    }
}

bool map_gen_verify (Map* map_pointer, SectionData* verify_data, Point* location)
{
    int intermediate = findLayerByName(map_pointer,"intermediate");
    if (intermediate == -1) return false;
    int i = location->x;
    int j = location->y;
    for (int k = 0; k < verify_data->w; k++)
    {
        for (int l = 0; l < verify_data->h; l++)
        {
            if (!((i+k < map_pointer->w) && (j+l < map_pointer->h)))
                return false;
            if (map_pointer->layers[intermediate][i+k][j+l] != verify_data->tile[(verify_data->w*l)+k])
                return false;
        }
    }
    return true;
}

void map_gen_exits (Map* map_pointer)
{
    int intermediate = findLayerByName(map_pointer,"intermediate");
        if (intermediate == -1) return;
    bool exit[EXITS_COUNT];
    int exit_count = 0;
    while (exit_count < 2)
    {
        exit_count = 0;
        for (int i = 0; i < EXITS_COUNT; i++)
        {
            exit[i] = ((rand() % 100) < 50);
            if (exit[i]) exit_count++;
        }
    }
    for (int i = 0; i < EXITS_COUNT; i++)
    {
        if (exit[i])
        {
            int start_x = ((i == 0)||(i == 1)) ? 0 : map_pointer->w-1;
            int start_y = ((i == 0)||(i == 3)) ? 0 : map_pointer->h-1;
            int previous_x = start_x;
            int previous_y = start_y;
            int previous_tile = Tile_Type::TILE_NONE;
            bool done = false;
            while (!done)
            {
                if ((map_pointer->layers[intermediate][start_x][start_y] == Tile_Type::TILE_FLOOR)&&(map_pointer->layers[intermediate][previous_x][previous_y] == Tile_Type::TILE_WALL))
                {
                    map_pointer->layers[intermediate][previous_x][previous_y] = Tile_Type::TILE_EXIT;
                    done = true;
                }
                previous_tile = map_pointer->layers[intermediate][start_x][start_y];
                previous_x = start_x;
                previous_y = start_y;
                ((i == 0)||(i == 1)) ? start_x++ : start_x--;
                ((i == 0)||(i == 3)) ? start_y++ : start_y--;
                if     ((((i == 0)||(i == 1))&&(start_x > map_pointer->w/2))
                    || (!((i == 0)||(i == 1))&&(start_x < map_pointer->w/2))
                    ||  (((i == 0)||(i == 3))&&(start_y > map_pointer->h/2))
                    || (!((i == 0)||(i == 3))&&(start_y < map_pointer->h/2)))
                        done = true;
            }
        }
    }
}

void map_gen_river (Map* map_pointer)
{
    (void) map_pointer;
}

void map_gen_road (Map* map_pointer)
{
    (void) map_pointer;
}

void map_gen_bridge (Map* map_pointer)
{
    (void) map_pointer;
}
