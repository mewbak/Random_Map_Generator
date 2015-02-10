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

#include "MapGenerator_D1.h"

void MapGenerator_D1::map_gen_D1_split(MapNode *map_node)
{
    int  x_range = map_node->data.w - (ROOM_MAX_X*2) - 2;
    int  y_range = map_node->data.h - (ROOM_MAX_Y*2) - 2;
    bool split_x = (x_range > 0) ? true : false;
    bool split_y = (y_range > 0) ? true : false;
    if (split_x && split_y)
    {
        if (x_range > y_range)
        {
            split_x = true;
            split_y = false;
        }
        if (y_range > x_range)
        {
            split_x = false;
            split_y = true;
        }
        if (x_range == y_range)
        {
            if (rand()%100 > 50) split_x = false;
            else split_y = false;
        }
    }
    if (split_x) // split vertically
    {
        int tile_count_x_out = 0;
        int tile_count_y_out = 0;
        int tile_data_count  = 0;
        x_range = (x_range > 0) ? rand()%x_range : 0;
        int new_size_x_1 = ROOM_MAX_X + x_range + (map_node->data.w % 2);
        int new_size_x_2 = map_node->data.w - new_size_x_1;
        // left --------------------------------------------------------------------------------------------------------
        map_node->left = new MapNode;
        map_node->left->data.w = new_size_x_1;
        map_node->left->data.h = map_node->data.h;
        map_node->left->data.no_of_tiles = map_node->left->data.w * map_node->left->data.h;
        map_node->left->data.tile = new GenTile[map_node->left->data.no_of_tiles];
        tile_count_x_out = 0;
        tile_count_y_out = 0;
        tile_data_count  = 0;
        for(int tile_count = 0; tile_count < map_node->left->data.no_of_tiles; tile_count++)
        {
            tile_data_count = (tile_count_y_out*map_node->data.w)+tile_count_x_out;
            map_node->left->data.tile[tile_count].position.x = map_node->data.tile[tile_data_count].position.x;
            map_node->left->data.tile[tile_count].position.y = map_node->data.tile[tile_data_count].position.y;
            map_node->left->data.tile[tile_count].data       = map_node->data.tile[tile_data_count].data;
            tile_count_x_out++;
            if (tile_count_x_out >= new_size_x_1)
            {
                tile_count_x_out = 0;
                tile_count_y_out++;
            }
        }
        map_gen_D1_split(map_node->left);
        for(int tile_count = 0; tile_count < map_node->left->data.no_of_tiles; tile_count++)
        {
            for(tile_data_count = 0; tile_data_count < map_node->data.no_of_tiles; tile_data_count++)
            {
                if ((map_node->data.tile[tile_data_count].position.x == map_node->left->data.tile[tile_count].position.x) &&
                    (map_node->data.tile[tile_data_count].position.y == map_node->left->data.tile[tile_count].position.y))
                     map_node->data.tile[tile_data_count].data       = map_node->left->data.tile[tile_count].data;
            }
        }
        // right --------------------------------------------------------------------------------------------------------
        map_node->right = new MapNode;
        map_node->right->data.w = new_size_x_2;
        map_node->right->data.h = map_node->data.h;
        map_node->right->data.no_of_tiles = map_node->right->data.w * map_node->right->data.h;
        map_node->right->data.tile = new GenTile[map_node->right->data.no_of_tiles];
        tile_count_x_out = new_size_x_1;
        tile_count_y_out = 0;
        tile_data_count  = 0;
        for(int tile_count = 0; tile_count < map_node->right->data.no_of_tiles; tile_count++)
        {
            tile_data_count = (tile_count_y_out*map_node->data.w)+tile_count_x_out;
            map_node->right->data.tile[tile_count].position.x = map_node->data.tile[tile_data_count].position.x;
            map_node->right->data.tile[tile_count].position.y = map_node->data.tile[tile_data_count].position.y;
            map_node->right->data.tile[tile_count].data       = map_node->data.tile[tile_data_count].data;
            tile_count_x_out++;
            if (tile_count_x_out >= map_node->data.w)
            {
                tile_count_x_out = new_size_x_1;
                tile_count_y_out++;
            }
        }
        map_gen_D1_split(map_node->right);
        for(int tile_count = 0; tile_count < map_node->right->data.no_of_tiles; tile_count++)
        {
            for(tile_data_count = 0; tile_data_count < map_node->data.no_of_tiles; tile_data_count++)
            {
                if ((map_node->data.tile[tile_data_count].position.x == map_node->right->data.tile[tile_count].position.x) &&
                    (map_node->data.tile[tile_data_count].position.y == map_node->right->data.tile[tile_count].position.y))
                     map_node->data.tile[tile_data_count].data       = map_node->right->data.tile[tile_count].data;
            }
        }
        //generate horizontal passages

        int passage_y   =  map_node->data.h/2;
        int passage_x_1 =  map_node->left->data.w/2;
        int passage_x_2 = (map_node->right->data.w/2)+map_node->left->data.w;
        for (int pos_x = passage_x_1;pos_x < passage_x_2;pos_x++)
        {
            map_node->data.tile[((passage_y*map_node->data.w)+pos_x)].data = Tile_Type::TILE_FLOOR;
        }

        delete[] map_node->left->data.tile;
        delete map_node->left;
        delete[] map_node->right->data.tile;
        delete map_node->right;
    }
    if (split_y) // split horizontally
    {
        int tile_count_x_out = 0;
        int tile_count_y_out = 0;
        int tile_data_count  = 0;
        y_range = (y_range > 0) ? rand()%y_range : 0;
        int new_size_y_1 = ROOM_MAX_Y + y_range + (map_node->data.h % 2);
        int new_size_y_2 = map_node->data.h - new_size_y_1;
        // left --------------------------------------------------------------------------------------------------------
        map_node->left = new MapNode;
        map_node->left->data.w = map_node->data.w;
        map_node->left->data.h = new_size_y_1;
        map_node->left->data.no_of_tiles = map_node->left->data.w * map_node->left->data.h;
        map_node->left->data.tile = new GenTile[map_node->left->data.no_of_tiles];
        tile_count_x_out = 0;
        tile_count_y_out = 0;
        tile_data_count  = 0;
        for(int tile_count = 0; tile_count < map_node->left->data.no_of_tiles; tile_count++)
        {
            tile_data_count = (tile_count_y_out*map_node->data.w)+tile_count_x_out;
            map_node->left->data.tile[tile_count].position.x = map_node->data.tile[tile_data_count].position.x;
            map_node->left->data.tile[tile_count].position.y = map_node->data.tile[tile_data_count].position.y;
            map_node->left->data.tile[tile_count].data       = map_node->data.tile[tile_data_count].data;
            tile_count_x_out++;
            if (tile_count_x_out >= map_node->left->data.w)
            {
                tile_count_x_out = 0;
                tile_count_y_out++;
            }
        }
        map_gen_D1_split(map_node->left);
        for(int tile_count = 0; tile_count < map_node->left->data.no_of_tiles; tile_count++)
        {
            for(tile_data_count = 0; tile_data_count < map_node->data.no_of_tiles; tile_data_count++)
            {
                if ((map_node->data.tile[tile_data_count].position.x == map_node->left->data.tile[tile_count].position.x) &&
                    (map_node->data.tile[tile_data_count].position.y == map_node->left->data.tile[tile_count].position.y))
                     map_node->data.tile[tile_data_count].data       = map_node->left->data.tile[tile_count].data;
            }
        }
        // right --------------------------------------------------------------------------------------------------------
        map_node->right = new MapNode;
        map_node->right->data.w = map_node->data.w;
        map_node->right->data.h = new_size_y_2;
        map_node->right->data.no_of_tiles = map_node->right->data.w * map_node->right->data.h;
        map_node->right->data.tile = new GenTile[map_node->right->data.no_of_tiles];
        tile_count_x_out = 0;
        tile_count_y_out = new_size_y_1;
        tile_data_count  = 0;
        for(int tile_count = 0; tile_count < map_node->right->data.no_of_tiles; tile_count++)
        {
            tile_data_count = (tile_count_y_out*map_node->data.w)+tile_count_x_out;
            map_node->right->data.tile[tile_count].position.x = map_node->data.tile[tile_data_count].position.x;
            map_node->right->data.tile[tile_count].position.y = map_node->data.tile[tile_data_count].position.y;
            map_node->right->data.tile[tile_count].data       = map_node->data.tile[tile_data_count].data;
            tile_count_x_out++;
            if (tile_count_x_out >= map_node->right->data.w)
            {
                tile_count_x_out = 0;
                tile_count_y_out++;
            }
        }
        map_gen_D1_split(map_node->right);
        for(int tile_count = 0; tile_count < map_node->right->data.no_of_tiles; tile_count++)
        {
            for(tile_data_count = 0; tile_data_count < map_node->data.no_of_tiles; tile_data_count++)
            {
                if ((map_node->data.tile[tile_data_count].position.x == map_node->right->data.tile[tile_count].position.x) &&
                    (map_node->data.tile[tile_data_count].position.y == map_node->right->data.tile[tile_count].position.y))
                     map_node->data.tile[tile_data_count].data       = map_node->right->data.tile[tile_count].data;
            }
        }
        //generate vertical passages

        int passage_x   =  map_node->data.w/2;
        int passage_y_1 =  map_node->left->data.h/2;
        int passage_y_2 = (map_node->right->data.h/2)+map_node->left->data.h;
        for (int pos_y = passage_y_1;pos_y < passage_y_2;pos_y++)
        {
            map_node->data.tile[((pos_y*map_node->data.w)+passage_x)].data = Tile_Type::TILE_FLOOR;
        }

        delete[] map_node->left->data.tile;
        delete map_node->left;
        delete[] map_node->right->data.tile;
        delete map_node->right;
    }
    if (!split_x && !split_y)
    {
        int room_size_x = 0;
        int room_size_y = 0;
        if (map_node->data.w > ROOM_MAX_X)
        {
            room_size_x = ((map_node->data.w - ROOM_MAX_X)/2);
            room_size_x = (room_size_x > 0) ? rand()%room_size_x : 0;
        }
        if (map_node->data.h > ROOM_MAX_Y)
        {
            room_size_y = ((map_node->data.h - ROOM_MAX_Y)/2);
            room_size_y = (room_size_y > 0) ? rand()%room_size_y : 0;
        }
        for (int pos_y = (room_size_y+1); pos_y < (map_node->data.h-1-room_size_y); pos_y++)
        {
            for (int pos_x = (room_size_x+1); pos_x < (map_node->data.w-1-room_size_x); pos_x++)
                map_node->data.tile[((pos_y*map_node->data.w)+pos_x)].data = Tile_Type::TILE_FLOOR;
        }
    }
}

void MapGenerator_D1::map_gen_D1_internal(Map* map_pointer)
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
    MapNode* temp_map = new MapNode;
    temp_map->data.w = map_pointer->w;
    temp_map->data.h = map_pointer->h;
    temp_map->data.no_of_tiles = temp_map->data.w * temp_map->data.h;
    temp_map->data.tile = new GenTile[temp_map->data.no_of_tiles];
    for (int j = 0; j < map_pointer->h; j++)
    {
        for (int i = 0; i < map_pointer->w; i++)
        {
            temp_map->data.tile[(j*map_pointer->w)+i].data = Tile_Type::TILE_WALL;
            temp_map->data.tile[(j*map_pointer->w)+i].position.x = i;
            temp_map->data.tile[(j*map_pointer->w)+i].position.y = j;
        }
    }
    map_gen_D1_split(temp_map);
    for (int j = 0; j < map_pointer->h; j++)
    {
        for (int i = 0; i < map_pointer->w; i++)
            map_pointer->layers[intermediate][i][j] = temp_map->data.tile[(j*map_pointer->w)+i].data;
    }
    delete[] temp_map->data.tile;
    delete temp_map;
}

void MapGenerator_D1::Generate (Map* map_pointer, MapProperties properties)
{
    Prepare(map_pointer, properties);
    GenerateMap(map_pointer,properties);
    applyTileset(map_pointer, properties.tile_set);
}

void MapGenerator_D1::GenerateMap(Map* map_pointer, MapProperties properties)
{
    map_gen_D1_internal(map_pointer);
    if (properties.gen_exits) map_gen_exits (map_pointer);
}

