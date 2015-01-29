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

#include <iostream>
#include <fstream>
#include "MapGenerator.h"
#include "MapHelper.h"
#include "export/MapSaver.h"
#include "TilesetDef.h"


void MapGenerator::Initialize(Map* map_pointer, int dimension_x, int dimension_y)
{
    map_pointer->clearEvents();
    //map_pointer->clearQueues(); // cant use protected
    //map_pointer->clearLayers(); // cant use protected

    // this is moved from
    // void map_init(Map* map_pointer, int size_x, int size_y);
    delete[] map_pointer->room;
    delete[] map_pointer->tile;
    map_pointer->no_of_rooms = ROOM_MAX;
    //if (map_pointer->room != NULL) delete[] map_pointer->room;
    map_pointer->room = new Room[map_pointer->no_of_rooms];
    for (int i = 0; i < map_pointer->no_of_rooms; i++)
    {
        map_pointer->room[i].active      = false;
        map_pointer->room[i].position.x  = 0;
        map_pointer->room[i].position.y  = 0;
        map_pointer->room[i].w = 0;
        map_pointer->room[i].h = 0;
        map_pointer->room[i].no_of_connected_rooms = 0;
        for (int j = 0; j < ROOM_MAX_CONNECTIONS; j++)
        {
            map_pointer->room[i].exit[j] = false;
            map_pointer->room[i].connected_room[j].connected = false;
            map_pointer->room[i].connected_room[j].room_no   = 0;
            map_pointer->room[i].connected_room[j].direction = DIRECTION_BIAS_NONE;
        }
    }
    map_pointer->no_of_rooms = 0;
    map_pointer->w = dimension_x;
    map_pointer->h = dimension_y;
    //delete[] map_pointer->tile;
    map_pointer->tile = new GenTile[map_pointer->size()];
    for (int i = 0; i < map_pointer->h; i++)
    {
        for (int j = 0; j < map_pointer->w; j++)
        {
            map_pointer->tile[(i*map_pointer->w)+j].attribute = TILE_ATTRIBUTE_NONE;
            if ((j == 0) || (j == map_pointer->w-1) || (i == 0) || (i == map_pointer->h-1))
                 map_pointer->tile[(i*map_pointer->w)+j].data = Tile_Type::TILE_WALL;
            else map_pointer->tile[(i*map_pointer->w)+j].data = Tile_Type::TILE_FLOOR;
            map_pointer->tile[(i*map_pointer->w)+j].position.x = j;
            map_pointer->tile[(i*map_pointer->w)+j].position.y = i;

            if ((i < (map_pointer->h / 1.5)) && (j == (map_pointer->w / 4))) map_pointer->tile[(i*map_pointer->w)+j].data = Tile_Type::TILE_WALL;
            if ((i > (map_pointer->h / 4)) && (j == (map_pointer->w / 2))) map_pointer->tile[(i*map_pointer->w)+j].data = Tile_Type::TILE_WALL;
            //if ((i < (map_pointer->h )) && (j == (map_pointer->w / 4))) map_pointer->tile[(i*map_pointer->w)+j].data = Tile_Type::TILE_WALL;
        }
    }
}

void MapGenerator::Export(Map* map_pointer, std::string file_name)
{
    MapSaver* mapSaver = new MapSaver(map_pointer);
    mapSaver->saveMap(file_name);
    delete mapSaver;
}

/*
void MapGenerator::map_to_flare_map (Map* map_pointer, flare_map_type* flare_map_pointer, int tile_set)
{
    map_gen_redundant_tile_check (map_pointer);
    flare_map_pointer->width = map_pointer->w;
    flare_map_pointer->height = map_pointer->h;
    flare_map_pointer->no_of_tiles = map_pointer->size();
    flare_map_pointer->layer_background = new int[flare_map_pointer->no_of_tiles];
    flare_map_pointer->layer_fringe = new int[flare_map_pointer->no_of_tiles];
    flare_map_pointer->layer_object = new int[flare_map_pointer->no_of_tiles];
    flare_map_pointer->layer_foreground = new int[flare_map_pointer->no_of_tiles];
    flare_map_pointer->layer_collision = new int[flare_map_pointer->no_of_tiles];
    flare_map_pointer->tileset = tile_set;
    for (int i = 0; i < map_pointer->size(); i++)
    {
        flare_map_pointer->layer_background[i] = 0;
        flare_map_pointer->layer_fringe[i] = 0;
        flare_map_pointer->layer_object[i] = 0;
        flare_map_pointer->layer_foreground[i] = 0;
        flare_map_pointer->layer_collision[i] = 0;
    }
    switch (tile_set)
    {
        case TILESET_CAVE:
            flare_map_pointer->map_name = "Randomly generated cave";
            flare_map_pointer->tileset_file_name = "tilesetdefs/tileset_cave.txt";
            flare_map_pointer->tile_width = 64;
            flare_map_pointer->tile_height = 32;
            flare_map_pointer->music_file_name = "music/cave_theme.ogg";
            for (int i = 0; i < flare_map_pointer->no_of_tiles; i++)
            {
                int temp_tile[8];
                int temp_tile_ok[8];
                temp_tile[0] = i-1;
                temp_tile[1] = i+1;
                temp_tile[2] = i+map_pointer->w;
                temp_tile[3] = i+map_pointer->w+1;
                temp_tile[4] = i+map_pointer->w-1;
                temp_tile[5] = i-map_pointer->w;
                temp_tile[6] = i-map_pointer->w+1;
                temp_tile[7] = i-map_pointer->w-1;
                for (int j = 0; j < 8; j++) temp_tile_ok[j] = ((temp_tile[j] >= 0)&&(temp_tile[j] < map_pointer->size()));

                switch (map_pointer->tile[i].data)
                {
                    case Tile_Type::TILE_FLOOR:
                        flare_map_pointer->layer_background[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                           TILESET_TILE_TYPE::_TILE_FLOOR);
                        flare_map_pointer->layer_collision[i] = 0;
                        flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                       TILESET_TILE_TYPE::TILE_OBJECT);
                    break;
                    case Tile_Type::TILE_WALL:
                        flare_map_pointer->layer_collision[i] = 1;
                        //wall up
                        if ((temp_tile_ok[0])&&(temp_tile_ok[1])&&(temp_tile_ok[5])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_UP);
                        }
                        //wall down
                        if ((temp_tile_ok[0])&&(temp_tile_ok[1])&&(temp_tile_ok[2])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_DOWN);
                            flare_map_pointer->layer_background[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                               TILESET_TILE_TYPE::_TILE_FLOOR);
                        }
                        //wall left
                        if ((temp_tile_ok[2])&&(temp_tile_ok[5])&&(temp_tile_ok[0])&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_LEFT);
                        }
                        //wall right
                        if ((temp_tile_ok[2])&&(temp_tile_ok[5])&&(temp_tile_ok[1])&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_RIGHT);
                            flare_map_pointer->layer_background[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                               TILESET_TILE_TYPE::_TILE_FLOOR);
                        }
                        //wall convex down
                        if ((temp_tile_ok[1])&&(temp_tile_ok[2])&&(temp_tile_ok[3])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[3]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_convex_down);
                            flare_map_pointer->layer_background[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                               TILESET_TILE_TYPE::_TILE_FLOOR);
                        }
                        //wall concave down
                        if ((temp_tile_ok[1])&&(temp_tile_ok[2])&&(temp_tile_ok[3])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[3]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_concave_down);
                            flare_map_pointer->layer_background[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                               TILESET_TILE_TYPE::_TILE_FLOOR);
                        }
                        //wall convex left
                        if ((temp_tile_ok[0])&&(temp_tile_ok[2])&&(temp_tile_ok[4])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[4]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_convex_left);
                        }
                        //wall concave left
                        if ((temp_tile_ok[0])&&(temp_tile_ok[2])&&(temp_tile_ok[4])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[4]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_concave_left);
                            flare_map_pointer->layer_background[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                               TILESET_TILE_TYPE::TILE_FLOOR_LEFT_HALF);
                        }
                        //wall convex up
                        if ((temp_tile_ok[5])&&(temp_tile_ok[0])&&(temp_tile_ok[7])&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[7]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_convex_up);
                        }
                        //wall concave up
                        if ((temp_tile_ok[5])&&(temp_tile_ok[0])&&(temp_tile_ok[7])&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[7]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_concave_up);
                        }
                        //wall convex right
                        if ((temp_tile_ok[1])&&(temp_tile_ok[5])&&(temp_tile_ok[6])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[6]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_convex_right);
                        }
                        //wall concave right
                        if ((temp_tile_ok[1])&&(temp_tile_ok[5])&&(temp_tile_ok[6])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[6]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_concave_right);
                            flare_map_pointer->layer_background[i] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                               TILESET_TILE_TYPE::TILE_FLOOR_RIGHT_HALF);
                        }
                    break;
                    case Tile_Type::TILE_EXIT:
                    break;
                    case Tile_Type::TILE_PATH:
                    break;
                case Tile_Type::TILE_NONE:
                    default:
                        flare_map_pointer->layer_background[i] = 0;
                        flare_map_pointer->layer_collision[i] = 3;
                    break;
                }
            }
        break;
        case TILESET_DUNGEON:
            flare_map_pointer->map_name = "Randomly generated dungeon";
            flare_map_pointer->tileset_file_name = "tilesetdefs/tileset_dungeon.txt";
            flare_map_pointer->tile_width = 64;
            flare_map_pointer->tile_height = 32;
            flare_map_pointer->music_file_name = "music/dungeon_theme.ogg";
            for (int i = 0; i < flare_map_pointer->no_of_tiles; i++)
            {
                int temp_tile[8];
                int temp_tile_ok[8];
                temp_tile[0] = i-1;
                temp_tile[1] = i+1;
                temp_tile[2] = i+map_pointer->w;
                temp_tile[3] = i+map_pointer->w+1;
                temp_tile[4] = i+map_pointer->w-1;
                temp_tile[5] = i-map_pointer->w;
                temp_tile[6] = i-map_pointer->w+1;
                temp_tile[7] = i-map_pointer->w-1;
                for (int j = 0; j < 8; j++) temp_tile_ok[j] = ((temp_tile[j] >= 0)&&(temp_tile[j] < map_pointer->size()));

                switch (map_pointer->tile[i].data)
                {
                    case Tile_Type::TILE_FLOOR:
                        flare_map_pointer->layer_background[i] = TilesetDef::getRandomTile(TILESET::TILESET_DUNGEON,
                                                                                           TILESET_TILE_TYPE::_TILE_FLOOR);
                        flare_map_pointer->layer_collision[i] = 0;
                        flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_DUNGEON,
                                                                                       TILESET_TILE_TYPE::TILE_OBJECT);
                    break;
                    case Tile_Type::TILE_WALL:
                        flare_map_pointer->layer_background[i] = TilesetDef::getRandomTile(TILESET::TILESET_DUNGEON,
                                                                                       TILESET_TILE_TYPE::_TILE_FLOOR);
                        flare_map_pointer->layer_collision[i] = 1;
                        //wall up
                        if ((temp_tile_ok[0])&&(temp_tile_ok[1])&&(temp_tile_ok[5])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_DUNGEON,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_UP);
                        }
                        //wall down
                        if ((temp_tile_ok[0])&&(temp_tile_ok[1])&&(temp_tile_ok[2])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_DUNGEON,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_DOWN);
                        }
                        //wall left
                        if ((temp_tile_ok[2])&&(temp_tile_ok[5])&&(temp_tile_ok[0])&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_DUNGEON,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_LEFT);
                        }
                        //wall right
                        if ((temp_tile_ok[2])&&(temp_tile_ok[5])&&(temp_tile_ok[1])&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_DUNGEON,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_RIGHT);
                        }
                        //wall convex down
                        if ((temp_tile_ok[1])&&(temp_tile_ok[2])&&(temp_tile_ok[3])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[3]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_DUNGEON,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_convex_down);
                        }
                        //wall concave down
                        if ((temp_tile_ok[1])&&(temp_tile_ok[2])&&(temp_tile_ok[3])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[3]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_DUNGEON,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_concave_down);
                        }
                        //wall convex left
                        if ((temp_tile_ok[0])&&(temp_tile_ok[2])&&(temp_tile_ok[4])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[4]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_DUNGEON,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_convex_left);
                        }
                        //wall concave left
                        if ((temp_tile_ok[0])&&(temp_tile_ok[2])&&(temp_tile_ok[4])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[4]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_DUNGEON,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_concave_left);
                        }
                        //wall convex up
                        if ((temp_tile_ok[5])&&(temp_tile_ok[0])&&(temp_tile_ok[7])&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[7]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_DUNGEON,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_convex_up);
                        }
                        //wall concave up
                        if ((temp_tile_ok[5])&&(temp_tile_ok[0])&&(temp_tile_ok[7])&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[7]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_DUNGEON,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_concave_up);
                        }
                        //wall convex right
                        if ((temp_tile_ok[1])&&(temp_tile_ok[5])&&(temp_tile_ok[6])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[6]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_DUNGEON,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_convex_right);
                        }
                        //wall concave right
                        if ((temp_tile_ok[1])&&(temp_tile_ok[5])&&(temp_tile_ok[6])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[6]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_DUNGEON,
                                                                                           TILESET_TILE_TYPE::TILE_WALL_concave_right);
                        }
                    break;
                    case Tile_Type::TILE_EXIT:
                    break;
                    case Tile_Type::TILE_PATH:
                    break;
                case Tile_Type::TILE_NONE:
                    default:
                        flare_map_pointer->layer_background[i] = 0;
                        flare_map_pointer->layer_collision[i] = 3;
                    break;
                }
            }
        break;
        case TILESET_GRASSLAND:
            flare_map_pointer->map_name = "Randomly generated grassland";
            flare_map_pointer->tileset_file_name = "tilesetdefs/tileset_grassland.txt";
            flare_map_pointer->tile_width = 64;
            flare_map_pointer->tile_height = 32;
            flare_map_pointer->music_file_name = "music/forest_theme.ogg";
            for (int i = 0; i < flare_map_pointer->no_of_tiles; i++)
            {
                int temp_tile[8];
                int temp_tile_ok[8];
                temp_tile[0] = i-1;
                temp_tile[1] = i+1;
                temp_tile[2] = i+map_pointer->w;
                temp_tile[3] = i+map_pointer->w+1;
                temp_tile[4] = i+map_pointer->w-1;
                temp_tile[5] = i-map_pointer->w;
                temp_tile[6] = i-map_pointer->w+1;
                temp_tile[7] = i-map_pointer->w-1;
                for (int j = 0; j < 8; j++) temp_tile_ok[j] = ((temp_tile[j] >= 0)&&(temp_tile[j] < map_pointer->size()));
                int rand_value = 0;
                switch (map_pointer->tile[i].data)
                {
                    case Tile_Type::TILE_FLOOR:
                    flare_map_pointer->layer_background[i] = TilesetDef::getRandomTile(TILESET::TILESET_GRASSLAND,
                                                                                       TILESET_TILE_TYPE::_TILE_FLOOR);
                    flare_map_pointer->layer_collision[i] = 0;
                    flare_map_pointer->layer_object[i] = TilesetDef::getRandomTile(TILESET::TILESET_GRASSLAND,
                                                                                   TILESET_TILE_TYPE::TILE_OBJECT);
                    break;
                    case Tile_Type::TILE_WALL:
                        flare_map_pointer->layer_background[i] = 16;
                        flare_map_pointer->layer_collision[i] = 1;
                        //wall up
                        if ((temp_tile_ok[0])&&(temp_tile_ok[1])&&(temp_tile_ok[5])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = 51;
                        }
                        //wall down
                        if ((temp_tile_ok[0])&&(temp_tile_ok[1])&&(temp_tile_ok[2])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 53;
                            else                      flare_map_pointer->layer_object[i] = 49;
                        }
                        //wall left
                        if ((temp_tile_ok[2])&&(temp_tile_ok[5])&&(temp_tile_ok[0])&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = 50;
                        }
                        //wall right
                        if ((temp_tile_ok[2])&&(temp_tile_ok[5])&&(temp_tile_ok[1])&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 52;
                            else                      flare_map_pointer->layer_object[i] = 48;
                        }
                        //wall convex down
                        if ((temp_tile_ok[1])&&(temp_tile_ok[2])&&(temp_tile_ok[3])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[3]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 60;
                            else                      flare_map_pointer->layer_object[i] = 56;
                        }
                        //wall concave down
                        if ((temp_tile_ok[1])&&(temp_tile_ok[2])&&(temp_tile_ok[3])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[3]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 68;
                            else                      flare_map_pointer->layer_object[i] = 64;
                        }
                        //wall convex left
                        if ((temp_tile_ok[0])&&(temp_tile_ok[2])&&(temp_tile_ok[4])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[4]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 61;
                            else                      flare_map_pointer->layer_object[i] = 57;
                        }
                        //wall concave left
                        if ((temp_tile_ok[0])&&(temp_tile_ok[2])&&(temp_tile_ok[4])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[4]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 69;
                            else                      flare_map_pointer->layer_object[i] = 65;
                        }
                        //wall convex up
                        if ((temp_tile_ok[5])&&(temp_tile_ok[0])&&(temp_tile_ok[7])&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[7]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 62;
                            else                      flare_map_pointer->layer_object[i] = 58;
                        }
                        //wall concave up
                        if ((temp_tile_ok[5])&&(temp_tile_ok[0])&&(temp_tile_ok[7])&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[7]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 70;
                            else                      flare_map_pointer->layer_object[i] = 66;
                        }
                        //wall convex right
                        if ((temp_tile_ok[1])&&(temp_tile_ok[5])&&(temp_tile_ok[6])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[6]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 63;
                            else                      flare_map_pointer->layer_object[i] = 59;
                        }
                        //wall concave right
                        if ((temp_tile_ok[1])&&(temp_tile_ok[5])&&(temp_tile_ok[6])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[6]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 71;
                            else                      flare_map_pointer->layer_object[i] = 67;
                        }
                    break;
                    case Tile_Type::TILE_EXIT:
                    break;
                    case Tile_Type::TILE_PATH:
                    break;
                    case Tile_Type::TILE_NONE:
                    default:
                        flare_map_pointer->layer_background[i] = 0;
                        flare_map_pointer->layer_collision[i] = 3;
                    break;
                }
            }
        break;
    }
}
*/

