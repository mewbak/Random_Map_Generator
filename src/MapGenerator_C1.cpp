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

void MapGenerator_C1::Generate (MapType* map_pointer)
{
    Initialize(map_pointer);
    // generate in map_pointer
    maprow *current_layer = new maprow[map_pointer->w];
    map_pointer->layers.push_back(current_layer);
    map_pointer->layernames.push_back("background");
    for (int j = 0; j < map_pointer->h; j++)
    {
        for (int i = 0; i < map_pointer->w; i++)
        {
            (current_layer)[i][j] = 9; // just checking we are generating a correctly formed background layer...
        }
    }
    Export(map_pointer);
}

