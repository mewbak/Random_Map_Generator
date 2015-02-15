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

void MapGenerator_C1::Generate (Map* map_pointer, MapProperties properties)
{
    Prepare(map_pointer, properties);
    GenerateMap(map_pointer,properties);
    PostProcess(map_pointer, properties.tile_set);
}

void MapGenerator_C1::check_tile(int map_size, FillData* fill_data, int tile_parent, int tile_check)
{
    if ((tile_check >=0)&&(tile_check < map_size))
    {
            if ((fill_data[tile_parent].tile_join)&&(fill_data[tile_check].tile_data == Tile_Type::TILE_FLOOR))
                fill_data[tile_check].tile_join = true;
    }
}

void MapGenerator_C1::CheckJoiningTiles(Map* map_pointer, FillData* fill_data, int tile_number)
{
    // function assumes calling from desired floor tile.
    int map_size = map_pointer->w * map_pointer->h;
    fill_data[tile_number].tile_join = true;
    for (int j = 0; j < map_pointer->w; j++)
    {
        for (int i = 0; i < map_size; i++)
        {
            check_tile(map_size,fill_data,i,i-1);
            check_tile(map_size,fill_data,i,i-map_pointer->w-1);
            check_tile(map_size,fill_data,i,i-map_pointer->w);
            check_tile(map_size,fill_data,i,i-map_pointer->w+1);
            check_tile(map_size,fill_data,i,i+1);
            check_tile(map_size,fill_data,i,i+map_pointer->w+1);
            check_tile(map_size,fill_data,i,i+map_pointer->w);
            check_tile(map_size,fill_data,i,i+map_pointer->w-1);
        }
    }
}

void MapGenerator_C1::GenerateMap(Map* map_pointer, MapProperties properties)
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
            map_pointer->layers[intermediate][i][j] = Tile_Type::TILE_FLOOR;
    }
    FillData *fill_data = new FillData[(map_pointer->w*map_pointer->h)];
    for (int i = 0; i < (map_pointer->w*map_pointer->h); i++)
    {
        fill_data[i].tile_join = false;
    }
    fill_data[(map_pointer->w*(map_pointer->h/2))+(map_pointer->w/2)].tile_join = true;
    for (int j = 0; j < map_pointer->h; j++)
    {
        for (int i = 0; i < map_pointer->w; i++)
        {
            if ((j == 0)||(j == map_pointer->h-1)||(i == 0)||(i == map_pointer->w-1))
                map_pointer->layers[intermediate][i][j] = Tile_Type::TILE_WALL;
            else
                map_pointer->layers[intermediate][i][j] = Tile_Type::TILE_FLOOR;
        }
    }
    for (int i = 0; i < (((map_pointer->w*map_pointer->h)) * 0.6); i++)
    {
        map_pointer->layers[intermediate][rand() % map_pointer->w][rand() % map_pointer->h] = Tile_Type::TILE_WALL;
    }
    for(int j = -1; j < 2; j++)
    {
        for(int i = -1; i < 2; i++)
        {
            map_pointer->layers[intermediate][(map_pointer->w/2)+i][(map_pointer->h/2)+j] = Tile_Type::TILE_FLOOR;
        }
    }
    for (int s = 0; s < 2; s++)
    {
        for (int j = 1; j < map_pointer->h-1; j++)
        {
            for (int i = 1; i < map_pointer->w-1; i++)
            {
                int num_walls = 0;
                if (map_pointer->layers[intermediate][i][j-1] == Tile_Type::TILE_WALL) num_walls++;
                if (map_pointer->layers[intermediate][i][j+1] == Tile_Type::TILE_WALL) num_walls++;
                if (map_pointer->layers[intermediate][i-1][j] == Tile_Type::TILE_WALL) num_walls++;
                if (map_pointer->layers[intermediate][i-1][j-1] == Tile_Type::TILE_WALL) num_walls++;
                if (map_pointer->layers[intermediate][i-1][j+1] == Tile_Type::TILE_WALL) num_walls++;
                if (map_pointer->layers[intermediate][i+1][j] == Tile_Type::TILE_WALL) num_walls++;
                if (map_pointer->layers[intermediate][i+1][j-1] == Tile_Type::TILE_WALL) num_walls++;
                if (map_pointer->layers[intermediate][i+1][j+1] == Tile_Type::TILE_WALL) num_walls++;
                if ((map_pointer->layers[intermediate][i][j] == Tile_Type::TILE_WALL)&&(num_walls > 3)) map_pointer->layers[intermediate][i][j] = Tile_Type::TILE_WALL;
                else if ((map_pointer->layers[intermediate][i][j] == Tile_Type::TILE_FLOOR)&&(num_walls > 4)) map_pointer->layers[intermediate][i][j] = Tile_Type::TILE_WALL;
                else map_pointer->layers[intermediate][i][j] = Tile_Type::TILE_FLOOR;
            }
        }
    }
    map_check(map_pointer,map_pointer->layers[intermediate]);
    for (int i = 0; i < (map_pointer->w*map_pointer->h); i++)
    {
        fill_data[i].tile_data = map_pointer->layers[intermediate][i%map_pointer->w][i/map_pointer->w];
        fill_data[i].tile_join = false;
        fill_data[i].tile_done = false;
    }
    fill_data[(map_pointer->w*(map_pointer->h/2))+(map_pointer->w/2)].tile_join = true;
    CheckJoiningTiles(map_pointer,fill_data,(map_pointer->w*(map_pointer->h/2))+(map_pointer->w/2));
    for (int i = 0; i < (map_pointer->w*map_pointer->h); i++)
    {
        if (fill_data[i].tile_join) map_pointer->layers[intermediate][i%map_pointer->w][i/map_pointer->w] = Tile_Type::TILE_FLOOR;
        else map_pointer->layers[intermediate][i%map_pointer->w][i/map_pointer->w] = Tile_Type::TILE_WALL;
    }
    delete[] fill_data;
    if (properties.gen_exits) map_gen_exits (map_pointer);
}
