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

#include "MapGenerator_D2.h"

void MapGenerator_D2::map_gen_D2_internal (Map* map_pointer)
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
    int max_r = (int)sqrt((float)(ROOM_MAX_X*ROOM_MAX_X)+(ROOM_MAX_Y*ROOM_MAX_Y));
    int max_rooms = (map_pointer->w*map_pointer->h) / (ROOM_MIN_X*ROOM_MIN_Y);
    room_struct* room = new room_struct[max_rooms];
    for (int i = 0; i < max_rooms; i++)
    {
        room[i].valid = true;
        room[i].w = ROOM_MIN_X + rand() % (ROOM_MAX_X-ROOM_MIN_X);
        room[i].h = ROOM_MIN_Y + rand() % (ROOM_MAX_Y-ROOM_MIN_Y);
        room[i].x = (room[i].w/2) + rand() % (int)(map_pointer->w - room[i].w)-1;
        room[i].y = (room[i].h/2) + rand() % (int)(map_pointer->h - room[i].h)-1;
    }
    for (int i = 0; i < max_rooms; i++)
    {
        for (int j = 0; j < max_rooms; j++)
        {
            if ((room[i].valid && room[j].valid)&&(i!=j))
            {
                if (max_r > (int)sqrt((float)((room[i].x-room[j].x)*(room[i].x-room[j].x))+((room[i].y-room[j].y)*(room[i].y-room[j].y))))
                    room[j].valid = false;
            }
        }
    }
    for (int i = 0; i < max_rooms; i++)
    {
        if (room[i].valid)
        {
            for (int j = 1; j < room[i].w; j++)
            {
                for (int k = 1; k < room[i].h; k++)
                    map_pointer->layers[intermediate][room[i].x-(room[i].w/2)+j][room[i].y-(room[i].h/2)+k] = Tile_Type::TILE_FLOOR;
            }
        }
    }
    for (int i = 0; i < max_rooms; i++)
    {
        if (room[i].valid)
        {
            for (int j = 0; j < max_rooms; j++)
            {
                if ((room[j].valid)&&(i != j))
                {
                    bool path_found = false;
                    if (!path_found)
                    {
                        int previous_tile = Tile_Type::TILE_FLOOR;
                        int transitions = 0;
                        int current_x = room[i].x;
                        int current_y = room[i].y;
                        for (current_x = room[i].x; current_x != room[j].x;)
                        {
                            if (previous_tile != map_pointer->layers[intermediate][current_x][current_y])
                                transitions++;
                            if (map_pointer->layers[intermediate][current_x][current_y] == Tile_Type::TILE_PATH)
                                transitions++;
                            previous_tile = map_pointer->layers[intermediate][current_x][current_y];
                            if (room[i].x < room[j].x) current_x++;
                                    else current_x--;
                        }
                        for (current_y = room[i].y; current_y != room[j].y;)
                        {
                            if (previous_tile != map_pointer->layers[intermediate][current_x][current_y])
                                transitions++;
                            if (map_pointer->layers[intermediate][current_x][current_y] == Tile_Type::TILE_PATH)
                                transitions++;
                            previous_tile = map_pointer->layers[intermediate][current_x][current_y];
                            if (room[i].y < room[j].y) current_y++;
                                    else current_y--;
                        }
                        if ((!path_found)&&(transitions == 2))
                        {
                            path_found = true;
                            current_x = room[i].x;
                            current_y = room[i].y;
                            for (current_x = room[i].x; current_x != room[j].x;)
                            {
                                map_pointer->layers[intermediate][current_x][current_y] = Tile_Type::TILE_PATH;
                                if (room[i].x < room[j].x) current_x++;
                                        else current_x--;
                            }
                            for (current_y = room[i].y; current_y != room[j].y;)
                            {
                                map_pointer->layers[intermediate][current_x][current_y] = Tile_Type::TILE_PATH;
                                if (room[i].y < room[j].y) current_y++;
                                        else current_y--;
                            }
                        }
                    }
                    if (!path_found)
                    {
                        int previous_tile = Tile_Type::TILE_FLOOR;
                        int transitions = 0;
                        int current_x = room[i].x;
                        int current_y = room[i].y;
                        for (current_y = room[i].y; current_y != room[j].y;)
                        {
                            if (previous_tile != map_pointer->layers[intermediate][current_x][current_y])
                                transitions++;
                            if (map_pointer->layers[intermediate][current_x][current_y] == Tile_Type::TILE_PATH)
                                transitions++;
                            previous_tile = map_pointer->layers[intermediate][current_x][current_y];
                            if (room[i].y < room[j].y) current_y++;
                                    else current_y--;
                        }
                        for (current_x = room[i].x; current_x != room[j].x;)
                        {
                            if (previous_tile != map_pointer->layers[intermediate][current_x][current_y])
                                transitions++;
                            if (map_pointer->layers[intermediate][current_x][current_y] == Tile_Type::TILE_PATH)
                                transitions++;
                            previous_tile = map_pointer->layers[intermediate][current_x][current_y];
                            if (room[i].x < room[j].x) current_x++;
                                    else current_x--;
                        }
                        if ((!path_found)&&(transitions == 2))
                        {
                            path_found = true;
                            current_x = room[i].x;
                            current_y = room[i].y;
                            for (current_y = room[i].y; current_y != room[j].y;)
                            {
                                map_pointer->layers[intermediate][current_x][current_y] = Tile_Type::TILE_PATH;
                                if (room[i].y < room[j].y) current_y++;
                                        else current_y--;
                            }
                            for (current_x = room[i].x; current_x != room[j].x;)
                            {
                                map_pointer->layers[intermediate][current_x][current_y] = Tile_Type::TILE_PATH;
                                if (room[i].x < room[j].x) current_x++;
                                        else current_x--;
                            }
                        }
                    }
                }
            }
        }
    }

	delete [] room;

    for (int j = 0; j < map_pointer->h; j++)
    {
        for (int i = 0; i < map_pointer->w; i++)
            if (map_pointer->layers[intermediate][i][j] == Tile_Type::TILE_PATH)
                map_pointer->layers[intermediate][i][j] = Tile_Type::TILE_FLOOR;
    }
}

void MapGenerator_D2::Generate (Map* map_pointer, MapProperties properties)
{
    Prepare(map_pointer, properties);
    GenerateMap(map_pointer,properties);
    PostProcess(map_pointer, properties.tile_set);
}

void MapGenerator_D2::GenerateMap(Map* map_pointer, MapProperties properties)
{
    map_gen_D2_internal(map_pointer);
    if (!map_gen_flood_fill(map_pointer)) map_gen_D2_internal(map_pointer);
    if (properties.gen_exits) map_gen_exits (map_pointer);
}
