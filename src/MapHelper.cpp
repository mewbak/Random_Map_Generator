#include <iostream>
#include "a-star.h"
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
        fill_data[tile_number].tile_done = true;
        fill_data[tile_number].tile_join = true;
        return_value++;
        if ((tile_number+1) <= map_pointer->size()) return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number+1);
        if ((tile_number-1) >= 0) return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number-1);
        if ((tile_number+map_pointer->w) <= map_pointer->size()) return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number+map_pointer->w);
        if ((tile_number+map_pointer->w+1) <= map_pointer->size()) return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number+map_pointer->w+1);
        if ((tile_number+map_pointer->w-1) <= map_pointer->size()) return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number+map_pointer->w-1);
        if ((tile_number-map_pointer->w) >= 0) return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number-map_pointer->w);
        if ((tile_number-map_pointer->w+1) >= 0) return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number-map_pointer->w+1);
        if ((tile_number-map_pointer->w-1) >= 0) return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number-map_pointer->w-1);
    }
    return (return_value);
}

bool map_gen_room_flood_fill (Map* map_pointer)
{
    int  floor_count  = 0;
    int  first_floor  = -1;
    bool return_value = true;
    FillData* fill_data = new FillData[map_pointer->size()];
    for(int j = 0; j < map_pointer->size(); j++)
    {
        if (map_pointer->tile[j].data == Tile_Type::TILE_FLOOR) floor_count++;
        if ((floor_count == 1) && (first_floor < 0)) first_floor = j;
        fill_data[j].tile_data = map_pointer->tile[j].data ;
        fill_data[j].tile_done = false;
        fill_data[j].tile_join = false;
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
