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

#ifndef MAP_GENERATOR_M1_H
#define MAP_GENERATOR_M1_H

#include "MapGenerator.h"

/**
 * class MapGenerator_M1
 *
 * The MapGenerator_M1 used to generate maze like maps.
 *
 */

class MapGenerator_M1 : public MapGenerator
{
    public:
        void Generate (Map* map_pointer, MapProperties properties);

    private:
        void GenerateMap(Map* map_pointer, MapProperties properties);
        bool isWallTile(Map* map_pointer, int tile);
        bool map_gen_maze_check_tile(Map* map_pointer, Point tile, int direction_bias);
        void map_gen_maze(Map* map_pointer, int tile_x, int tile_y, int direction_bias);
        bool add_room (Map* map_pointer, room_struct* room);
        bool connect_room (Map* map_pointer, room_struct* room);
};

#endif // MAP_GENERATOR_M1_H

