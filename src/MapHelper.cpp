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
#include "MapGenerator_D1.h"
#include "MapGenerator_D2.h"
#include "MapGenerator_M1.h"

void MapGenerate(Map* map_pointer, MapProperties properties)
{
    MapGenerator* generator = NULL;
    if (properties.algorithm == Algorithm_Type::GEN_ALGORITHM_C1) {
        generator = new MapGenerator_C1();
    }
    else if (properties.algorithm == Algorithm_Type::GEN_ALGORITHM_D1) {
        generator = new MapGenerator_D1();
    }
    else if (properties.algorithm == Algorithm_Type::GEN_ALGORITHM_D2) {
        generator = new MapGenerator_D2();
    }
    else if (properties.algorithm == Algorithm_Type::GEN_ALGORITHM_M1) {
        generator = new MapGenerator_M1();
    }
    generator->Generate(map_pointer, properties);
    delete generator;
}

void map_check (Map* map_pointer, maprow *layer)
{
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

int map_gen_flood_fill_tile (Map* map_pointer, FillData* fill_data, int tile_number)
{
    int return_value = 0;
    if ((fill_data[tile_number].tile_data == Tile_Type::TILE_FLOOR) && (!fill_data[tile_number].tile_done))
    {
        int map_size = map_pointer->w*map_pointer->h;
        fill_data[tile_number].tile_done = true;
        fill_data[tile_number].tile_join = true;
        return_value++;
        if ((tile_number+1) <= map_size)
            return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number+1);
        if ((tile_number-1) >= 0)
            return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number-1);
        if ((tile_number+map_pointer->w) <= map_size)
            return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number+map_pointer->w);
        if ((tile_number+map_pointer->w+1) <= map_size)
            return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number+map_pointer->w+1);
        if ((tile_number+map_pointer->w-1) <= map_size)
            return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number+map_pointer->w-1);
        if ((tile_number-map_pointer->w) >= 0)
            return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number-map_pointer->w);
        if ((tile_number-map_pointer->w+1) >= 0)
            return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number-map_pointer->w+1);
        if ((tile_number-map_pointer->w-1) >= 0)
            return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number-map_pointer->w-1);
    }
    return (return_value);
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
            if ((floor_count == 1) && (first_floor < 0)) first_floor = j;
            fill_data[(j*map_pointer->w)+i].tile_data = map_pointer->layers[intermediate][i][j];
            fill_data[(j*map_pointer->w)+i].tile_done = false;
            fill_data[(j*map_pointer->w)+i].tile_join = false;
        }
    }
    int   number_found = map_gen_flood_fill_tile(map_pointer,fill_data,first_floor);
    if (number_found < floor_count) return_value = false;
    delete[] fill_data;
    return (return_value);
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
        // will add better code for map exit tomorrow, it is now 2:44am and sleep is needed....
        if (exit[i])
            map_pointer->layers[intermediate][rand() % map_pointer->w][rand() % map_pointer->h] = Tile_Type::TILE_EXIT;
    }

}
