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

#include "MapGenerator.h"
#include "MapGenerator_C1.h"

void MapGenerator::Initialize(map_type* map_pointer)
{

}

void MapGenerate(map_type* map_pointer)
{
    switch (map_pointer->tileset_type)
    {
        case TILESET_NONE:
        break;
        case TILESET_CAVE:
            //MapGenerator_C1(map_pointer);
        break;
        case TILESET_DUNGEON:
        break;
        case TILESET_GRASSLAND:
        break;
        default:
        break;
    }
}

