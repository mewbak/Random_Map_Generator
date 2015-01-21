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

#define TILESET_NONE      0
#define TILESET_CAVE      1
#define TILESET_DUNGEON   2
#define TILESET_GRASSLAND 3

struct map_type
{
	std::string filename;
	std::string tileset;
	int  dimension_x;
	int  dimension_y;
    int  no_of_tiles;
    int  tileset_type;
    int* layer_background;
    int* layer_object;
    int* layer_collision;
};

class MapGenerator
{
    private:
        void Initialize (map_type* map_pointer);
    public:
        virtual void Generate (map_type* map_pointer);
};

void MapGenerate(map_type* map_pointer);

#endif // MAP_GENERATOR_H
