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

#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

#include <iostream>
#include <vector>
#include <queue>
#include "flare/Map.h"
#include "MapHelper.h"

/**
 * class MapGenerator
 *
 * The MapGenerator class is and abstract class which the
 * various map generation algorithm classes derive from.
 *
 */

class MapGenerator
{
    public:
        virtual ~MapGenerator(void) {};
        void Initialize (Map* map_pointer, int dimension_x, int dimension_y);
        virtual void Generate (Map* map_pointer, int dimension_x, int dimension_y) = 0;
        static void Export (Map* map_pointer, std::string file_name);

        // this should not be needed in future
//        static void map_to_flare_map (Map* map_pointer, flare_map_type* flare_map_pointer, TILESET tile_set);

//        static void finalizeMap(Map* map_pointer, flare_map_type* flare_map_pointer, TILESET tileset);
};

#endif // MAP_GENERATOR_H

