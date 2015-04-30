/*
    Random Map Generator.
    Copyright (C) 2015  Igor Paliychuk, Paul Wortmann

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
    paul.wortmann@gmail.com
*/

#include "src/flare/Map.h"
#include "src/MapGenerator.h"
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

int main(int argc, char *argv[])
{
    Map* map_pointer     = new Map;
    AppData app_data;
    app_data.algorithm   = Algorithm_Type::GEN_ALGORITHM_C1;
    app_data.file_name   = "default.txt";
    app_data.gen_chests  = false;
    app_data.gen_enemies = false;
    app_data.gen_exits   = true;
    app_data.progress    = 0;
    app_data.size_x      = 100;
    app_data.size_y      = 100;
    app_data.seed        = time(NULL);
    app_data.seed_set    = false;
    app_data.status      = "Ready";
    app_data.tile_set    = "cave";
    std::string tempArg  = "";
    std::string tempVal  = "";
    for (int i = 0; i < argc; i++)
    {
        tempArg = argv[i];
        if ((tempArg.compare("--help") == 0) ||
            (tempArg.compare("-h") == 0))
        {
            std::cout << "Random Dungeon Genorator." << std::endl;
            std::cout << "-------------------------" << std::endl << std::endl;
            std::cout << "--help This help screen." << std::endl;
            std::cout << "--algorithm Specify algorithm, C1, D1, D2, D3, M1, T1" << std::endl;
            std::cout << "--filename Specify output file name." << std::endl;
            std::cout << "--tileset Specify tileset, cave, dungeon, grassland, snowplains" << std::endl;
        }
        if (i+1 < argc)
        {
            tempVal = argv[i+1];
            if ((tempArg.compare("--algorithm") == 0) ||
                (tempArg.compare("-a") == 0))
            {
                if (tempVal.compare("C1") == 0)
                    app_data.algorithm     = Algorithm_Type::GEN_ALGORITHM_C1;
                else if (tempVal.compare("D1") == 0)
                    app_data.algorithm     = Algorithm_Type::GEN_ALGORITHM_D1;
                else if (tempVal.compare("D2") == 0)
                    app_data.algorithm     = Algorithm_Type::GEN_ALGORITHM_D2;
                else if (tempVal.compare("D3") == 0)
                    app_data.algorithm     = Algorithm_Type::GEN_ALGORITHM_D3;
                else if (tempVal.compare("M1") == 0)
                    app_data.algorithm     = Algorithm_Type::GEN_ALGORITHM_M1;
                else if (tempVal.compare("T1") == 0)
                    app_data.algorithm     = Algorithm_Type::GEN_ALGORITHM_T1;
                else
                    std::cout << "Invalid algorithm: " << tempVal.c_str() << std::endl;
            }

            else if ((tempArg.compare("--filename") == 0) ||
                (tempArg.compare("-f") == 0))
                app_data.file_name = tempVal.c_str();

            else if ((tempArg.compare("--tileset") == 0) ||
                (tempArg.compare("-a") == 0))
            {
                if (tempVal.compare("cave") == 0)
                    app_data.tile_set     = tempVal.c_str();
                else if (tempVal.compare("dungeon") == 0)
                    app_data.tile_set     = tempVal.c_str();
                else if (tempVal.compare("grassland") == 0)
                    app_data.tile_set     = tempVal.c_str();
                else if (tempVal.compare("snowplains") == 0)
                    app_data.tile_set     = tempVal.c_str();
                else
                    std::cout << "Invalid tileset: " << tempVal.c_str() << std::endl;
            }
        }
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
