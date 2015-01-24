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

#include "MapType.h"

MapType::MapType(void)
{
    // this constructor will need to be edited when integrated, will use Map class
    MapType::title          = "Randomly generated map";
    MapType::filename       = "default.txt";
    MapType::music_filename = "default";
    MapType::tileset        = "dungeon";
    MapType::spawn_x        = 0.0f; // Fpoint
    MapType::spawn_y        = 0.0f; // Fpoint
    MapType::w              = 100;
    MapType::h              = 100;
    MapType::spawn_dir      = 0; // check this
    MapType::layernames.push_back("background");
    maprow* background_layer = new maprow[w];
    layers.push_back(background_layer);
    MapType::layernames.push_back("object");
    maprow* object_layer = new maprow[w];
    layers.push_back(object_layer);
    MapType::layernames.push_back("collision");
    maprow* collision_layer = new maprow[w];
    layers.push_back(collision_layer);
}

MapType::~MapType(void)
{
    // this destructor will need to be edited when integrated, will use Map class
	for (unsigned i = 0; i < layers.size(); ++i)
		delete[] layers[i];
	layers.clear();
	layernames.clear();
}
