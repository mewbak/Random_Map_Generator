/*
    Random Map Generator.
    Copyright (C) 2015  Igor Paliychuk

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

    mansonigor@gmail.com
*/

#include "flare/Map.h"
#include "MapGenerator.h"
#include <sstream>

struct AppData
{
    // this is only used by console app.
    int     progress;
    int     algorithm;
    int     size_x;
    int     size_y;
    int     seed;
    bool    seed_set;
    bool    gen_exits;
    bool    gen_enemies;
    bool    gen_chests;
    std::string file_name;
    std::string tile_set;
    std::string status;
};

AppData app_data;
Map*  map_pointer;

int main(int argc, char *argv[])
{
    map_pointer            = new Map;
    app_data.algorithm     = Algorithm_Type::GEN_ALGORITHM_C1;
    app_data.file_name     = "default.txt";
    app_data.gen_chests    = false;
    app_data.gen_enemies   = false;
    app_data.gen_exits     = true;
    app_data.progress      = 0;
    app_data.size_x        = 100;
    app_data.size_y        = 100;
    app_data.seed          = time(NULL);
    app_data.seed_set      = false;
    app_data.status        = "Ready";
    app_data.tile_set      = "cave";

    if (argc > 1) {
         app_data.file_name = argv[1];
    }

    MapProperties properties;
    properties.algorithm = static_cast<Algorithm_Type::Algorithm_Type>(app_data.algorithm);
    properties.tile_set = app_data.tile_set;
    properties.size_x = app_data.size_x;
    properties.size_y = app_data.size_y;
    properties.seed = app_data.seed;
    properties.seed_set = app_data.seed_set;
    properties.gen_chests = app_data.gen_chests;
    properties.gen_enemies = app_data.gen_enemies;
    properties.gen_exits = app_data.gen_exits;

    MapGenerate(map_pointer, properties);
    MapGenerator::Export(map_pointer, app_data.file_name);
	
	delete   map_pointer;
		
    return 0;
}
