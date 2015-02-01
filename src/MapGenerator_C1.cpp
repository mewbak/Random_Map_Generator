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

void MapGenerator_C1::Generate (Map* map_pointer, int dimension_x, int dimension_y, TILESET tileset)
{
    Initialize(map_pointer, dimension_x, dimension_y);
    GenerateMap(map_pointer);
    applyTileset(map_pointer, tileset);
}

void MapGenerator_C1::Generate (Map* map_pointer, int dimension_x, int dimension_y, TILESET tileset, int seed)
{
    srand(seed);
    Generate(map_pointer, dimension_x, dimension_y, tileset);
}

void MapGenerator_C1::CheckJoiningTiles(Map* map_pointer, FillData* fill_data, int tile_number)
{
    if ((fill_data[tile_number].tile_data == Tile_Type::TILE_FLOOR) && (!fill_data[tile_number].tile_done))
    {
        fill_data[tile_number].tile_done = true;
        fill_data[tile_number].tile_join = true;
        if ((tile_number+1) <= map_pointer->size()) CheckJoiningTiles(map_pointer,fill_data,tile_number+1);
        if ((tile_number-1) >= 0) CheckJoiningTiles(map_pointer,fill_data,tile_number-1);
        if ((tile_number+map_pointer->w) <= map_pointer->size()) CheckJoiningTiles(map_pointer,fill_data,tile_number+map_pointer->w);
        if ((tile_number+map_pointer->w+1) <= map_pointer->size()) CheckJoiningTiles(map_pointer,fill_data,tile_number+map_pointer->w+1);
        if ((tile_number+map_pointer->w-1) <= map_pointer->size()) CheckJoiningTiles(map_pointer,fill_data,tile_number+map_pointer->w-1);
        if ((tile_number-map_pointer->w) >= 0) CheckJoiningTiles(map_pointer,fill_data,tile_number-map_pointer->w);
        if ((tile_number-map_pointer->w+1) >= 0) CheckJoiningTiles(map_pointer,fill_data,tile_number-map_pointer->w+1);
        if ((tile_number-map_pointer->w-1) >= 0) CheckJoiningTiles(map_pointer,fill_data,tile_number-map_pointer->w-1);
    }
}

void MapGenerator_C1::GenerateMap(Map* map_pointer)
{
    maprow *current_layer = new maprow[map_pointer->w];
    map_pointer->layers.push_back(current_layer);
    map_pointer->layernames.push_back("intermediate");
    FillData *fill_data = new FillData[map_pointer->size()];
    for (int i = 0; i < map_pointer->size(); i++)
    {
        fill_data[i].tile_join = false;
    }
    fill_data[(map_pointer->w*(map_pointer->h/2))+(map_pointer->w/2)].tile_join = true;
    for (int j = 0; j < map_pointer->h; j++)
    {
        for (int i = 0; i < map_pointer->w; i++)
        {
            if ((j == 0)||(j == map_pointer->h-1)||(i == 0)||(i == map_pointer->w-1))
                (current_layer)[i][j] = TILE_WALL;
            else
                (current_layer)[i][j] = TILE_FLOOR;
        }
    }
    for (int i = 0; i < ((map_pointer->size()) * 0.6); i++)
    {
        (current_layer)[rand() % map_pointer->w][rand() % map_pointer->h] = TILE_WALL;
    }
    for(int j = -1; j < 2; j++)
    {
        for(int i = -1; i < 2; i++)
        {
            (current_layer)[(map_pointer->w/2)+i][(map_pointer->h/2)+j] = TILE_FLOOR;
        }
    }
    for (int s = 0; s < 2; s++)
    {
        for (int j = 1; j < map_pointer->h-1; j++)
        {
            for (int i = 1; i < map_pointer->w-1; i++)
            {
                int num_walls = 0;
                if ((current_layer)[i][j-1] == TILE_WALL) num_walls++;
                if ((current_layer)[i][j+1] == TILE_WALL) num_walls++;
                if ((current_layer)[i-1][j] == TILE_WALL) num_walls++;
                if ((current_layer)[i-1][j-1] == TILE_WALL) num_walls++;
                if ((current_layer)[i-1][j+1] == TILE_WALL) num_walls++;
                if ((current_layer)[i+1][j] == TILE_WALL) num_walls++;
                if ((current_layer)[i+1][j-1] == TILE_WALL) num_walls++;
                if ((current_layer)[i+1][j+1] == TILE_WALL) num_walls++;
                if (((current_layer)[i][j] == TILE_WALL)&&(num_walls > 3)) (current_layer)[i][j] = TILE_WALL;
                else if (((current_layer)[i][j] == TILE_FLOOR)&&(num_walls > 4)) (current_layer)[i][j] = TILE_WALL;
                else (current_layer)[i][j] = TILE_FLOOR;
            }
        }
    }
    map_check(map_pointer,current_layer);
    for (int i = 0; i < map_pointer->size(); i++)
    {
        fill_data[i].tile_data = (current_layer)[i%map_pointer->w][i/map_pointer->w];
        fill_data[i].tile_join = false;
        fill_data[i].tile_done = false;
    }
    fill_data[(map_pointer->w*(map_pointer->h/2))+(map_pointer->w/2)].tile_join = true;
    CheckJoiningTiles(map_pointer,fill_data,(map_pointer->w*(map_pointer->h/2))+(map_pointer->w/2));
    for (int i = 0; i < map_pointer->size(); i++)
    {
        if (fill_data[i].tile_join) (current_layer)[i%map_pointer->w][i/map_pointer->w] = TILE_FLOOR;
        else (current_layer)[i%map_pointer->w][i/map_pointer->w] = TILE_WALL;
    }
    delete[] fill_data;
}
