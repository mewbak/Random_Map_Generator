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

#include "MapGenerator_D3.h"

void MapGenerator_D3::Generate (Map* map_pointer, MapProperties properties)
{
    Prepare(map_pointer, properties);
    GenerateMap(map_pointer,properties);
    PostProcess(map_pointer, properties);
}

void MapGenerator_D3::GenerateMap(Map* map_pointer, MapProperties properties)
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
    // room data init
    int no_of_rooms = ((map_pointer->w * map_pointer->h) / (ROOM_MAX_X * ROOM_MAX_Y)) * 2;
    room_struct* room = new room_struct[no_of_rooms];
    for (int i = 0; i < no_of_rooms; i++)
        room[i].valid = false;
    Point* start_location;
    start_location = new Point;
    start_location->x = rand() % (map_pointer->w - ROOM_MAX_X);
    start_location->y = rand() % (map_pointer->h - ROOM_MAX_Y);
    for (int i = 0; i < no_of_rooms; i++)
    {
        SectionData* feature_data = new SectionData;
        SectionData* verify_data  = new SectionData;

        int no_of_features = 2;
        bool horizontal = ((rand() % 100) < 50) ? true : false;
        switch (rand() % no_of_features)
        {
            case 0:
                //define posible feature (passage / corridor)
                feature_data->w = ROOM_MAX_X + 4;
                feature_data->h = ROOM_MAX_Y + 4;
                if (horizontal)
                    feature_data->h = 3;
                else
                    feature_data->w = 3;
            break;
            case 1:
            default:
                //define posible feature (square room)
                feature_data->w = ROOM_MIN_X + rand() % abs(ROOM_MAX_X - ROOM_MIN_X);
                feature_data->h = ROOM_MIN_Y + rand() % abs(ROOM_MAX_Y - ROOM_MIN_Y);
            break;
                //define posible feature ('circular' room)
        }
        feature_data->no_of_tiles = (feature_data->w+2) * (feature_data->h+2);
        feature_data->tile = new int[feature_data->no_of_tiles];
        for (int i = 0; i < feature_data->no_of_tiles; i++)
        {
            if (((i / feature_data->w) != 0)
                && ((i / feature_data->w) != feature_data->h-1)
                && ((i % feature_data->w) != 0)
                && ((i % feature_data->w) != feature_data->w-1))
                feature_data->tile[i] = Tile_Type::TILE_FLOOR;
            else
                feature_data->tile[i] = Tile_Type::TILE_WALL;
        }
        verify_data->w = feature_data->w;
        verify_data->h = feature_data->h;
        verify_data->no_of_tiles = feature_data->no_of_tiles;
        verify_data->tile = new int[verify_data->no_of_tiles];
        for (int i = 0; i < verify_data->no_of_tiles; i++)
                verify_data->tile[i] = Tile_Type::TILE_WALL;

        // better placement next goal!
        map_gen_find_replace(map_pointer,verify_data,feature_data);

        delete[] feature_data->tile;
        delete feature_data;
        delete[] verify_data->tile;
        delete verify_data;
    }
    if (properties.gen_exits) map_gen_exits (map_pointer);
    delete[] room;
    delete start_location;
}
