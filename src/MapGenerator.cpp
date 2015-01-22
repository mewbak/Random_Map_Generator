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
#include "MapGenerator_C1.h"
#include "MapGenerator_D1.h"

MapType::MapType(void)
{
    MapType::dimension_x = 100;
    MapType::dimension_y = 100;
    MapType::map_name    = "default";
    MapType::algorithm   = GEN_ALGORITHM_C1;
}

MapType::~MapType(void)
{
    delete[] MapType::layer_background;
    delete[] MapType::layer_collision;
    delete[] MapType::layer_object;
}

void MapGenerator::Initialize(MapType* map_pointer)
{
    map_pointer->no_of_tiles = map_pointer->dimension_x * map_pointer->dimension_y;
    map_pointer->layer_background = new int[map_pointer->no_of_tiles];
    map_pointer->layer_collision = new int[map_pointer->no_of_tiles];
    map_pointer->layer_object = new int[map_pointer->no_of_tiles];
    for (int i = 0; i < map_pointer->no_of_tiles; i++)
    {
        map_pointer->layer_background[i] = 0;
        map_pointer->layer_collision[i] = 3;
        map_pointer->layer_object[i] = 0;
    }
}

void MapGenerate(MapType* map_pointer)
{
    MapGenerator* generator = NULL;
    if (map_pointer->algorithm == GEN_ALGORITHM_C1) generator = new MapGenerator_C1();
    else if (map_pointer->algorithm == GEN_ALGORITHM_D1) generator = new MapGenerator_D1();
    generator->Generate(map_pointer);
    delete generator;
}

void MapGenerator::Export(MapType* map_pointer)
{
    std::ofstream output_file;
    std::string file_name = map_pointer->map_name+".txt";
    output_file.open (file_name.c_str(), std::ios::out | std::ios::trunc);
    output_file << "test.\n";
    output_file.close();
}



