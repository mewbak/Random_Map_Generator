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

#ifndef MAP_GENERATOR_D1_H
#define MAP_GENERATOR_D1_H

#include "MapGenerator.h"

/**
 * class MapGenerator_D1
 *
 * The MapGenerator_D1 generates dungeon maps with various
 * rooms by recursively subdividing the map and generating 3
 * rooms in the end nodes when a base case is reached (minimum room size).
 *
 */

class MapGenerator_D1 : public MapGenerator
{
    public:
        void Generate (Map* map_pointer, MapProperties properties);

    private:
        void GenerateMap(Map* map_pointer);
};

#endif // MAP_GENERATOR_D1_H
