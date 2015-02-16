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

#include "MapGenerator_T1.h"

void MapGenerator_T1::Generate (Map* map_pointer, MapProperties properties)
{
    Prepare(map_pointer, properties);
    GenerateMap(map_pointer,properties);
    PostProcess(map_pointer, properties);
}

void MapGenerator_T1::GenerateMap(Map* map_pointer, MapProperties properties)
{
    if (findLayerByName(map_pointer,"intermediate") == -1)
    {
        map_pointer->layers.resize(map_pointer->layers.size()+1);
        map_pointer->layers.back().resize(map_pointer->w);
        for (unsigned i=0; i<map_pointer->w; ++i) {
            map_pointer->layers.back()[i].resize(map_pointer->h);
        }
        map_pointer->layernames.push_back("intermediate");
    }
    int intermediate = findLayerByName(map_pointer,"intermediate");
    for (int j = 0; j < map_pointer->h; j++)
    {
        for (int i = 0; i < map_pointer->w; i++)
            map_pointer->layers[intermediate][i][j] = Tile_Type::TILE_FLOOR;
    }

    if (properties.gen_exits) map_gen_exits (map_pointer);
}
