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


void MapGenerator::applyTileset(Map* map_pointer, TILESET tileset)
{
    switch (tileset)
    {
    case TILESET::TILESET_CAVE:
        map_pointer->title = "Randomly generated cave";
        map_pointer->setTileset("tilesetdefs/tileset_cave.txt");
        break;
    case TILESET::TILESET_DUNGEON:
        map_pointer->title = "Randomly generated dungeon";
        map_pointer->setTileset("tilesetdefs/tileset_dungeon.txt");
        break;
    case TILESET::TILESET_GRASSLAND:
        map_pointer->title = "Randomly generated grassland";
        map_pointer->setTileset("tilesetdefs/tileset_grassland.txt");
        break;
    }

    //map_pointer->tile_width = 64;
    //map_pointer->tile_height = 32;
    map_pointer->music_filename = "music/cave_theme.ogg";

    int background = -1;
    int object = -1;
    int collision = -1;
    int intermediate = -1;

    for (unsigned i = 0; i < map_pointer->layers.size(); ++i) {
            if (map_pointer->layernames[i] == "collision") {
                collision = i;
            }
            if (map_pointer->layernames[i] == "object") {
                object = i;
            }
            if (map_pointer->layernames[i] == "background") {
                background = i;
            }
            if (map_pointer->layernames[i] == "intermediate") {
                intermediate = i;
            }
    }
    //if (background == -1 || object == -1 || collision == -1)
    //    return;

    // Maybe move this to Initialize() later
    if (background == -1)
    {
        maprow *current_layer = new maprow[map_pointer->w];
        map_pointer->layers.push_back(current_layer);
        map_pointer->layernames.push_back("background");
        background = map_pointer->layernames.size() - 1;
    }
    if (object == -1)
    {
        maprow *current_layer = new maprow[map_pointer->w];
        map_pointer->layers.push_back(current_layer);
        map_pointer->layernames.push_back("object");
        object = map_pointer->layernames.size() - 1;
    }
    if (collision == -1)
    {
        maprow *current_layer = new maprow[map_pointer->w];
        map_pointer->layers.push_back(current_layer);
        map_pointer->layernames.push_back("collision");
        collision = map_pointer->layernames.size() - 1;
    }

    for (int j = 0; j < map_pointer->h; j++)
    {
        for (int i = 0; i < map_pointer->w; i++)
        {
            // temp_tile
            //          7 5 6
            //          0 c 1
            //          4 2 3

            Point temp_tile[8];
            int temp_tile_ok[8];
            // i, j - center
            temp_tile[0] = Point(i-1, j);
            temp_tile[1] = Point(i+1, j);

            temp_tile[2] = Point(i,   j+1);
            temp_tile[3] = Point(i+1, j+1);
            temp_tile[4] = Point(i-1, j+1);

            temp_tile[5] = Point(i,   j-1);
            temp_tile[6] = Point(i+1, j-1);
            temp_tile[7] = Point(i-1, j-1);

            for (int k = 0; k < 8; k++)
            {
                temp_tile_ok[k] = ( (temp_tile[k].x >= 0 && temp_tile[k].y >= 0) &&
                                    (temp_tile[k].x < map_pointer->w && temp_tile[k].y < map_pointer->h)
                                  );
            }

            switch (map_pointer->layers[intermediate][i][j])
            {
                case Tile_Type::TILE_FLOOR:
                    map_pointer->layers[background][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                       TILESET_TILE_TYPE::_TILE_FLOOR);
                    map_pointer->layers[collision][i][j] = 0;
                    map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                   TILESET_TILE_TYPE::TILE_OBJECT);
                break;
                case Tile_Type::TILE_WALL:
                    map_pointer->layers[collision][i][j] = 1;
                    //wall up
                    if ((temp_tile_ok[0])&&(temp_tile_ok[1])&&(temp_tile_ok[5])&&
                            (map_pointer->layers[intermediate][temp_tile[0].x][temp_tile[0].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[1].x][temp_tile[1].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[5].x][temp_tile[5].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_UP);
                    }
                    //wall down
                    if ((temp_tile_ok[0])&&(temp_tile_ok[1])&&(temp_tile_ok[2])&&
                            (map_pointer->layers[intermediate][temp_tile[0].x][temp_tile[0].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[1].x][temp_tile[1].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[2].x][temp_tile[2].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_DOWN);
                        //map_pointer->layers[background][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                        //                                                                   TILESET_TILE_TYPE::_TILE_FLOOR);
                    }
                    //wall left
                    if ((temp_tile_ok[2])&&(temp_tile_ok[5])&&(temp_tile_ok[0])&&
                            (map_pointer->layers[intermediate][temp_tile[2].x][temp_tile[2].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[5].x][temp_tile[5].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[0].x][temp_tile[0].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_LEFT);
                    }
                    //wall right
                    if ((temp_tile_ok[2])&&(temp_tile_ok[5])&&(temp_tile_ok[1])&&
                            (map_pointer->layers[intermediate][temp_tile[2].x][temp_tile[2].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[5].x][temp_tile[5].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[1].x][temp_tile[1].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_RIGHT);
                        //map_pointer->layers[background][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                        //                                                                   TILESET_TILE_TYPE::_TILE_FLOOR);
                    }
                    //wall convex down
                    if ((temp_tile_ok[1])&&(temp_tile_ok[2])&&(temp_tile_ok[3])&&
                            (map_pointer->layers[intermediate][temp_tile[1].x][temp_tile[1].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[2].x][temp_tile[2].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[3].x][temp_tile[3].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_convex_down);
                        //map_pointer->layers[background][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                        //                                                                   TILESET_TILE_TYPE::_TILE_FLOOR);
                    }
                    //wall concave down
                    if ((temp_tile_ok[1])&&(temp_tile_ok[2])&&(temp_tile_ok[3])&&
                            (map_pointer->layers[intermediate][temp_tile[1].x][temp_tile[1].y] == Tile_Type::TILE_FLOOR)&&
                            (map_pointer->layers[intermediate][temp_tile[2].x][temp_tile[2].y] == Tile_Type::TILE_FLOOR)&&
                            (map_pointer->layers[intermediate][temp_tile[3].x][temp_tile[3].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_concave_down);
                        //map_pointer->layers[background][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                        //                                                                   TILESET_TILE_TYPE::_TILE_FLOOR);
                    }
                    //wall convex left
                    if ((temp_tile_ok[0])&&(temp_tile_ok[2])&&(temp_tile_ok[4])&&
                            (map_pointer->layers[intermediate][temp_tile[0].x][temp_tile[0].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[2].x][temp_tile[2].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[4].x][temp_tile[4].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_convex_left);
                    }
                    //wall concave left
                    if ((temp_tile_ok[0])&&(temp_tile_ok[2])&&(temp_tile_ok[4])&&
                            (map_pointer->layers[intermediate][temp_tile[0].x][temp_tile[0].y] == Tile_Type::TILE_FLOOR)&&
                            (map_pointer->layers[intermediate][temp_tile[2].x][temp_tile[2].y] == Tile_Type::TILE_FLOOR)&&
                            (map_pointer->layers[intermediate][temp_tile[4].x][temp_tile[4].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_concave_left);
                        //map_pointer->layers[background][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                        //                                                                   TILESET_TILE_TYPE::TILE_FLOOR_LEFT_HALF);
                    }
                    //wall convex up
                    if ((temp_tile_ok[5])&&(temp_tile_ok[0])&&(temp_tile_ok[7])&&
                            (map_pointer->layers[intermediate][temp_tile[5].x][temp_tile[5].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[0].x][temp_tile[0].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[7].x][temp_tile[7].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_convex_up);
                    }
                    //wall concave up
                    if ((temp_tile_ok[5])&&(temp_tile_ok[0])&&(temp_tile_ok[7])&&
                            (map_pointer->layers[intermediate][temp_tile[5].x][temp_tile[5].y] == Tile_Type::TILE_FLOOR)&&
                            (map_pointer->layers[intermediate][temp_tile[0].x][temp_tile[0].y] == Tile_Type::TILE_FLOOR)&&
                            (map_pointer->layers[intermediate][temp_tile[7].x][temp_tile[7].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_concave_up);
                    }
                    //wall convex right
                    if ((temp_tile_ok[1])&&(temp_tile_ok[5])&&(temp_tile_ok[6])&&
                            (map_pointer->layers[intermediate][temp_tile[1].x][temp_tile[1].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[5].x][temp_tile[5].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[6].x][temp_tile[6].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_convex_right);
                    }
                    //wall concave right
                    if ((temp_tile_ok[1])&&(temp_tile_ok[5])&&(temp_tile_ok[6])&&
                            (map_pointer->layers[intermediate][temp_tile[1].x][temp_tile[1].y] == Tile_Type::TILE_FLOOR)&&
                            (map_pointer->layers[intermediate][temp_tile[5].x][temp_tile[5].y] == Tile_Type::TILE_FLOOR)&&
                            (map_pointer->layers[intermediate][temp_tile[6].x][temp_tile[6].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_concave_right);
                        //map_pointer->layers[background][i][j] = TilesetDef::getRandomTile(TILESET::TILESET_CAVE,
                        //                                                                   TILESET_TILE_TYPE::TILE_FLOOR_RIGHT_HALF);
                    }
                break;
                case Tile_Type::TILE_EXIT:
                break;
                case Tile_Type::TILE_PATH:
                break;
            case Tile_Type::TILE_NONE:
                default:
                    map_pointer->layers[background][i][j] = 0;
                    map_pointer->layers[collision][i][j] = 3;
                break;
            }
        }
    }
}