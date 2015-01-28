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

#include "MapGenerator_C1.h"

void MapGenerator_C1::Generate (Map* map_pointer, int dimension_x, int dimension_y)
{
    Initialize(map_pointer, dimension_x, dimension_y);

    // generate in map_pointer
    GenerateMap(map_pointer);

    // export is called from outside
    //Export(map_pointer);
}

void MapGenerator_C1::GenerateMap(Map* map_pointer)
{
    FloodFill* fill_data = new FloodFill[map_pointer->size()];
    bool ca_map_good               = false;
    int  ca_minimum_cave_dimension      = 60;
    int  ca_wall_stay              = 4;
    int  ca_wall_new               = 5;
    int  ca_iterations             = 4;
    int  ca_no_of_random_tiles     = ((map_pointer->w * map_pointer->h) * ca_minimum_cave_dimension) / 100;
    Map temp_map;
    temp_map.tile = new GenTile[map_pointer->size()];
    //------------------------------------------------------------
    while (!ca_map_good)
    {
        for (int tile_count = 0; tile_count < map_pointer->size(); tile_count++)
        {
            map_pointer->tile[tile_count].data  = TILE_FLOOR;
        }
        for (int tile_count = 0; tile_count < ca_no_of_random_tiles; tile_count++)
        {
            map_pointer->tile[rand()%map_pointer->size()].data = TILE_WALL;
        }
        //add a 3x3 floored room to the middle of the map
        //this is for use later when checking for disjointed parts.
        {
            int middle_tile_number = ((map_pointer->size()/2)+(map_pointer->w/2));
            map_pointer->tile[middle_tile_number].data                       = TILE_FLOOR;
            map_pointer->tile[middle_tile_number+1].data                     = TILE_FLOOR;
            map_pointer->tile[middle_tile_number-1].data                     = TILE_FLOOR;
            map_pointer->tile[middle_tile_number+map_pointer->w].data   = TILE_FLOOR;
            map_pointer->tile[middle_tile_number+map_pointer->w+1].data = TILE_FLOOR;
            map_pointer->tile[middle_tile_number+map_pointer->w-1].data = TILE_FLOOR;
            map_pointer->tile[middle_tile_number-map_pointer->w].data   = TILE_FLOOR;
            map_pointer->tile[middle_tile_number-map_pointer->w+1].data = TILE_FLOOR;
            map_pointer->tile[middle_tile_number-map_pointer->w-1].data = TILE_FLOOR;
        }
        //fill perimeter with wall tiles
        for (int tile_count = 0; tile_count < map_pointer->w; tile_count++)
        {
            map_pointer->tile[tile_count].data                          = TILE_WALL;
            map_pointer->tile[map_pointer->size()-tile_count-1].data  = TILE_WALL;
        }
        for (int tile_count = 0; tile_count < map_pointer->h; tile_count++)
        {
            map_pointer->tile[tile_count*map_pointer->w].data                        = TILE_WALL;
            map_pointer->tile[(tile_count*map_pointer->w)+map_pointer->w-1].data = TILE_WALL;
        }
        //smooth map, depending on neighboring tiles.
        for (int refine_count = 0; refine_count < ca_iterations; refine_count++)
        {
            for(int tile_count = 0; tile_count < map_pointer->size(); tile_count++)
            {
                int number_of_neighbors = 0;
                temp_map.tile[tile_count].data = TILE_FLOOR; // new tile is initially a floor tile
                int temp_tile_number = tile_count+1;
                if ((temp_tile_number >= 0) && (temp_tile_number < map_pointer->size()) && (map_pointer->tile[temp_tile_number].data == TILE_WALL)) number_of_neighbors++;
                temp_tile_number = tile_count-1;
                if ((temp_tile_number >= 0) && (temp_tile_number < map_pointer->size()) && (map_pointer->tile[temp_tile_number].data == TILE_WALL)) number_of_neighbors++;
                temp_tile_number = tile_count+map_pointer->w;
                if ((temp_tile_number >= 0) && (temp_tile_number < map_pointer->size()) && (map_pointer->tile[temp_tile_number].data == TILE_WALL)) number_of_neighbors++;
                temp_tile_number = tile_count+map_pointer->w+1;
                if ((temp_tile_number >= 0) && (temp_tile_number < map_pointer->size()) && (map_pointer->tile[temp_tile_number].data == TILE_WALL)) number_of_neighbors++;
                temp_tile_number = tile_count+map_pointer->w-1;
                if ((temp_tile_number >= 0) && (temp_tile_number < map_pointer->size()) && (map_pointer->tile[temp_tile_number].data == TILE_WALL)) number_of_neighbors++;
                temp_tile_number = tile_count-map_pointer->w;
                if ((temp_tile_number >= 0) && (temp_tile_number < map_pointer->size()) && (map_pointer->tile[temp_tile_number].data == TILE_WALL)) number_of_neighbors++;
                temp_tile_number = tile_count-map_pointer->w+1;
                if ((temp_tile_number >= 0) && (temp_tile_number < map_pointer->size()) && (map_pointer->tile[temp_tile_number].data == TILE_WALL)) number_of_neighbors++;
                temp_tile_number = tile_count-map_pointer->w-1;
                if ((temp_tile_number >= 0) && (temp_tile_number < map_pointer->size()) && (map_pointer->tile[temp_tile_number].data == TILE_WALL)) number_of_neighbors++;
                if ((map_pointer->tile[tile_count].data  == TILE_WALL)  && (number_of_neighbors >= ca_wall_stay)) temp_map.tile[tile_count].data = TILE_WALL; //Tile on temp map is a wall
                if ((map_pointer->tile[tile_count].data  == TILE_FLOOR) && (number_of_neighbors >= ca_wall_new )) temp_map.tile[tile_count].data = TILE_WALL; //Tile on temp map is a wall
            }
            //copy tiles from temp map to the main map.
            for(int tile_count = 0; tile_count < map_pointer->size(); tile_count++)
            {
                map_pointer->tile[tile_count].data  = temp_map.tile[tile_count].data;
            }
            //fill perimeter with wall tiles
            for (int tile_count = 0; tile_count < map_pointer->w; tile_count++)
            {
                map_pointer->tile[tile_count].data                            = TILE_WALL;
                map_pointer->tile[map_pointer->size()-tile_count-1].data  = TILE_WALL;
            }
            for (int tile_count = 0; tile_count < map_pointer->h; tile_count++)
            {
                map_pointer->tile[tile_count*map_pointer->w].data                                     = TILE_WALL;
                map_pointer->tile[(tile_count*map_pointer->w)+map_pointer->w-1].data = TILE_WALL;
            }
        }
        // find out if cave from the center is the largest part, and discard disjointed parts
        // if main cave is of adequate dimension, keep and return good, else regenerate.
        for(int tile_count = 0; tile_count < map_pointer->size(); tile_count++)
        {
            fill_data[tile_count].tile_data      = map_pointer->tile[tile_count].data ;
            fill_data[tile_count].processed      = false;
            fill_data[tile_count].adjoining_tile = false;
        }
        // we already know these are floor tiles, so mark as part of the fill.
        int middle_tile_number = ((map_pointer->size()/2)+(map_pointer->w/2));
        fill_data[middle_tile_number].processed                                        = true;
        fill_data[middle_tile_number+1].processed                                      = true;
        fill_data[middle_tile_number-1].processed                                      = true;
        fill_data[middle_tile_number+map_pointer->w].processed        = true;
        fill_data[middle_tile_number+map_pointer->w+1].processed      = true;
        fill_data[middle_tile_number+map_pointer->w-1].processed      = true;
        fill_data[middle_tile_number-map_pointer->w].processed        = true;
        fill_data[middle_tile_number-map_pointer->w+1].processed      = true;
        fill_data[middle_tile_number-map_pointer->w-1].processed      = true;
        fill_data[middle_tile_number].adjoining_tile                                   = true;
        fill_data[middle_tile_number+1].adjoining_tile                                 = true;
        fill_data[middle_tile_number-1].adjoining_tile                                 = true;
        fill_data[middle_tile_number+map_pointer->w].adjoining_tile   = true;
        fill_data[middle_tile_number+map_pointer->w+1].adjoining_tile = true;
        fill_data[middle_tile_number+map_pointer->w-1].adjoining_tile = true;
        fill_data[middle_tile_number-map_pointer->w].adjoining_tile   = true;
        fill_data[middle_tile_number-map_pointer->w+1].adjoining_tile = true;
        fill_data[middle_tile_number-map_pointer->w-1].adjoining_tile = true;
        int   number_found         = 0;
        int   temp_tile            = 0;
        bool  is_an_adjoining_tile = false;
        //while((!fill_data[0].processed) && (!fill_data[map_pointer->size()-1].processed))
        for(int repeat_count = 0; repeat_count < ((map_pointer->w+map_pointer->h)/2); repeat_count++)
        {
            for(int tile_count = 0; tile_count < map_pointer->size(); tile_count++)
            {
                // we don't need to check if temp_tile is going passed the borders as the borders are always walls
                is_an_adjoining_tile  = false;
                temp_tile = tile_count+1;
                if((temp_tile >= 0) && (temp_tile < map_pointer->size()))
                {
                    if (fill_data[temp_tile].adjoining_tile)
                    {
                        is_an_adjoining_tile = true;
                        number_found++;
                    }
                }
                temp_tile = tile_count-1;
                if((temp_tile >= 0) && (temp_tile < map_pointer->size()))
                {
                    if (fill_data[temp_tile].adjoining_tile)
                    {
                        is_an_adjoining_tile = true;
                        number_found++;
                    }
                }
                temp_tile = tile_count+map_pointer->w;
                if((temp_tile >= 0) && (temp_tile < map_pointer->size()))
                {
                    if (fill_data[temp_tile].adjoining_tile)
                    {
                        is_an_adjoining_tile = true;
                        number_found++;
                    }
                }
                temp_tile = tile_count-map_pointer->w;
                if((temp_tile >= 0) && (temp_tile < map_pointer->size()))
                {
                    if (fill_data[temp_tile].adjoining_tile)
                    {
                        is_an_adjoining_tile = true;
                        number_found++;
                    }
                }
                fill_data[tile_count].processed = true;
                if ((is_an_adjoining_tile) && (fill_data[tile_count].tile_data == TILE_FLOOR)) fill_data[tile_count].adjoining_tile = true;
            }
        }
        ca_map_good = (number_found >= ((map_pointer->size()*ca_minimum_cave_dimension)/100.0f)) ? true : false;
    }
    // write new values to array discarding disjointed tiles
    for (int tile_count = 0; tile_count < map_pointer->size(); tile_count++)
    {
        if (fill_data[tile_count].adjoining_tile) fill_data[tile_count].tile_data = TILE_FLOOR;
        else fill_data[tile_count].tile_data = TILE_WALL;
    }
    // Push data from fill struct back to map.
    for(int tile_count = 0; tile_count < map_pointer->size(); tile_count++)
    {
        map_pointer->tile[tile_count].data  = fill_data[tile_count].tile_data;
    }
    map_check(map_pointer);
    delete[] temp_map.tile;
    delete[] fill_data;
}

void MapGenerator_C1::GenerateMap(Map* map_pointer, int seed)
{
    srand(seed);
    GenerateMap(map_pointer);
}
