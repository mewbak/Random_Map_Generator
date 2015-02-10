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

#ifndef TILESETDEF_H
#define TILESETDEF_H

#include <map>
#include "MapHelper.h"

namespace TILESET_TILE_TYPE
{
    enum TILESET_TILE_TYPE {
        _TILE_FLOOR,
        TILE_FLOOR_RIGHT_HALF,
        TILE_FLOOR_LEFT_HALF,
        TILE_OBJECT,

        TILE_WALL_UP,
        TILE_WALL_DOWN,
        TILE_WALL_LEFT,
        TILE_WALL_RIGHT,

        TILE_WALL_convex_down,
        TILE_WALL_concave_down,

        TILE_WALL_convex_left,
        TILE_WALL_concave_left,

        TILE_WALL_convex_up,
        TILE_WALL_concave_up,

        TILE_WALL_convex_right,
        TILE_WALL_concave_right,

        TILE_EXIT_LEFT,
        TILE_EXIT_RIGHT,

        TILE_PATH_LEFT,
        TILE_PATH_RIGHT//,

    //TILE_NONE
    };
}

class TilesetDef
{
public:
    static int getRandomTile(std::string _tileset, TILESET_TILE_TYPE::TILESET_TILE_TYPE type);

    static std::string tilesetDefinitions(int index);
	
    static std::string tilesetLocation(int index);
	
    static std::vector<std::string> tilesetNames();
	
    static int findTilesetByName(std::string name);

    static int findTilesetByLocation(std::string location);

private:
    static void init();
    
    static TILESET_TILE_TYPE::TILESET_TILE_TYPE toTyleType(std::string type);

    static std::vector< std::map<int, TILESET_TILE_TYPE::TILESET_TILE_TYPE> > tilesets;
    
    static std::vector<std::string> tileset_names;
    
    static std::vector<std::string> tileset_locations;

    static std::vector<std::string> tileset_definitions;

    TilesetDef();
    ~TilesetDef();
};

#endif // TILESETDEF_H
