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
#include "flare/MapSaver.h"
#include "TilesetDef.h"

void MapGenerator::Initialize(Map* map_pointer, int dimension_x, int dimension_y)
{
    map_pointer->w = dimension_x;
    map_pointer->h = dimension_y;
}

void MapGenerator::Prepare(Map *map_pointer, MapProperties properties)
{
    Initialize(map_pointer, properties.size_x, properties.size_y);
    if (properties.seed_set)
    {
        srand(properties.seed);
    }
    else srand(time(NULL));
}

void MapGenerator::Export(Map* map_pointer, std::string file_name)
{
    MapSaver* mapSaver = new MapSaver(map_pointer);

	int tilesetIndex = TilesetDef::findTilesetByLocation(map_pointer->getTileset());
	mapSaver->saveMap(file_name, TilesetDef::tilesetDefinitions(tilesetIndex));
    delete mapSaver;
}

void MapGenerator::applyTileset(Map* map_pointer, std::string tileset)
{
    map_pointer->title = "Randomly generated " + tileset;
    
    map_pointer->setTileset(TilesetDef::tilesetLocation(TilesetDef::findTilesetByName(tileset)));

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
                    map_pointer->layers[background][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                       TILESET_TILE_TYPE::_TILE_FLOOR);
                    map_pointer->layers[collision][i][j] = 0;
                    map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(tileset,
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
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_UP);
                    }
                    //wall down
                    if ((temp_tile_ok[0])&&(temp_tile_ok[1])&&(temp_tile_ok[2])&&
                            (map_pointer->layers[intermediate][temp_tile[0].x][temp_tile[0].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[1].x][temp_tile[1].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[2].x][temp_tile[2].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_DOWN);
                        map_pointer->layers[background][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                           TILESET_TILE_TYPE::_TILE_FLOOR);
                    }
                    //wall left
                    if ((temp_tile_ok[2])&&(temp_tile_ok[5])&&(temp_tile_ok[0])&&
                            (map_pointer->layers[intermediate][temp_tile[2].x][temp_tile[2].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[5].x][temp_tile[5].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[0].x][temp_tile[0].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_LEFT);
                    }
                    //wall right
                    if ((temp_tile_ok[2])&&(temp_tile_ok[5])&&(temp_tile_ok[1])&&
                            (map_pointer->layers[intermediate][temp_tile[2].x][temp_tile[2].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[5].x][temp_tile[5].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[1].x][temp_tile[1].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_RIGHT);
                        map_pointer->layers[background][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                           TILESET_TILE_TYPE::_TILE_FLOOR);
                    }
                    //wall convex down
                    if ((temp_tile_ok[1])&&(temp_tile_ok[2])&&(temp_tile_ok[3])&&
                            (map_pointer->layers[intermediate][temp_tile[1].x][temp_tile[1].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[2].x][temp_tile[2].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[3].x][temp_tile[3].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_convex_down);
                        map_pointer->layers[background][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                           TILESET_TILE_TYPE::_TILE_FLOOR);
                    }
                    //wall concave down
                    if ((temp_tile_ok[1])&&(temp_tile_ok[2])&&(temp_tile_ok[3])&&
                            (map_pointer->layers[intermediate][temp_tile[1].x][temp_tile[1].y] == Tile_Type::TILE_FLOOR)&&
                            (map_pointer->layers[intermediate][temp_tile[2].x][temp_tile[2].y] == Tile_Type::TILE_FLOOR)&&
                            (map_pointer->layers[intermediate][temp_tile[3].x][temp_tile[3].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_concave_down);
                        map_pointer->layers[background][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                           TILESET_TILE_TYPE::_TILE_FLOOR);
                    }
                    //wall convex left
                    if ((temp_tile_ok[0])&&(temp_tile_ok[2])&&(temp_tile_ok[4])&&
                            (map_pointer->layers[intermediate][temp_tile[0].x][temp_tile[0].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[2].x][temp_tile[2].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[4].x][temp_tile[4].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_convex_left);
                    }
                    //wall concave left
                    if ((temp_tile_ok[0])&&(temp_tile_ok[2])&&(temp_tile_ok[4])&&
                            (map_pointer->layers[intermediate][temp_tile[0].x][temp_tile[0].y] == Tile_Type::TILE_FLOOR)&&
                            (map_pointer->layers[intermediate][temp_tile[2].x][temp_tile[2].y] == Tile_Type::TILE_FLOOR)&&
                            (map_pointer->layers[intermediate][temp_tile[4].x][temp_tile[4].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_concave_left);
                        map_pointer->layers[background][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                           TILESET_TILE_TYPE::TILE_FLOOR_LEFT_HALF);
                    }
                    //wall convex up
                    if ((temp_tile_ok[5])&&(temp_tile_ok[0])&&(temp_tile_ok[7])&&
                            (map_pointer->layers[intermediate][temp_tile[5].x][temp_tile[5].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[0].x][temp_tile[0].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[7].x][temp_tile[7].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_convex_up);
                    }
                    //wall concave up
                    if ((temp_tile_ok[5])&&(temp_tile_ok[0])&&(temp_tile_ok[7])&&
                            (map_pointer->layers[intermediate][temp_tile[5].x][temp_tile[5].y] == Tile_Type::TILE_FLOOR)&&
                            (map_pointer->layers[intermediate][temp_tile[0].x][temp_tile[0].y] == Tile_Type::TILE_FLOOR)&&
                            (map_pointer->layers[intermediate][temp_tile[7].x][temp_tile[7].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_concave_up);
                    }
                    //wall convex right
                    if ((temp_tile_ok[1])&&(temp_tile_ok[5])&&(temp_tile_ok[6])&&
                            (map_pointer->layers[intermediate][temp_tile[1].x][temp_tile[1].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[5].x][temp_tile[5].y] == Tile_Type::TILE_WALL)&&
                            (map_pointer->layers[intermediate][temp_tile[6].x][temp_tile[6].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_convex_right);
                    }
                    //wall concave right
                    if ((temp_tile_ok[1])&&(temp_tile_ok[5])&&(temp_tile_ok[6])&&
                            (map_pointer->layers[intermediate][temp_tile[1].x][temp_tile[1].y] == Tile_Type::TILE_FLOOR)&&
                            (map_pointer->layers[intermediate][temp_tile[5].x][temp_tile[5].y] == Tile_Type::TILE_FLOOR)&&
                            (map_pointer->layers[intermediate][temp_tile[6].x][temp_tile[6].y] == Tile_Type::TILE_FLOOR))
                    {
                        map_pointer->layers[object][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                       TILESET_TILE_TYPE::TILE_WALL_concave_right);
                        map_pointer->layers[background][i][j] = TilesetDef::getRandomTile(tileset,
                                                                                           TILESET_TILE_TYPE::TILE_FLOOR_RIGHT_HALF);
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
    
    // remove intermediate layer. Currently breaks UI Preview
    //delete [] map_pointer->layers[intermediate];
    //map_pointer->layers.erase(map_pointer->layers.begin() + intermediate);
    //map_pointer->layernames.erase(map_pointer->layernames.begin() + intermediate);
}
