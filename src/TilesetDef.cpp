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

#include "TilesetDef.h"
#include "UtilsParsing.h"

std::vector< std::map<int, TILESET_TILE_TYPE::TILESET_TILE_TYPE> > TilesetDef::tilesets;

std::vector<std::string> TilesetDef::tileset_names;
    
std::vector<std::string> TilesetDef::tileset_locations;
    
std::vector<std::string> TilesetDef::tileset_definitions;

TilesetDef::TilesetDef()
{
}

TilesetDef::~TilesetDef()
{
}

TILESET_TILE_TYPE::TILESET_TILE_TYPE TilesetDef::toTyleType(std::string type)
{
	if (type == "floor")
		return TILESET_TILE_TYPE::_TILE_FLOOR;
	else if (type == "floor_right_half")
		return TILESET_TILE_TYPE::TILE_FLOOR_RIGHT_HALF;
	else if (type == "floor_left_half")
		return TILESET_TILE_TYPE::TILE_FLOOR_LEFT_HALF;

	else if (type == "object")
		return TILESET_TILE_TYPE::TILE_OBJECT;

	else if (type == "wall_up")
		return TILESET_TILE_TYPE::TILE_WALL_UP;
	else if (type == "wall_down")
		return TILESET_TILE_TYPE::TILE_WALL_DOWN;
	else if (type == "wall_left")
		return TILESET_TILE_TYPE::TILE_WALL_LEFT;
	else if (type == "wall_right")
		return TILESET_TILE_TYPE::TILE_WALL_RIGHT;

	else if (type == "wall_convex_down")
		return TILESET_TILE_TYPE::TILE_WALL_convex_down;
	else if (type == "wall_concave_down")
		return TILESET_TILE_TYPE::TILE_WALL_concave_down;

	else if (type == "wall_convex_left")
		return TILESET_TILE_TYPE::TILE_WALL_convex_left;
	else if (type == "wall_concave_left")
		return TILESET_TILE_TYPE::TILE_WALL_concave_left;

	else if (type == "wall_convex_up")
		return TILESET_TILE_TYPE::TILE_WALL_convex_up;
	else if (type == "wall_concave_up")
		return TILESET_TILE_TYPE::TILE_WALL_concave_up;

	else if (type == "wall_convex_right")
		return TILESET_TILE_TYPE::TILE_WALL_convex_right;
	else if (type == "wall_concave_right")
		return TILESET_TILE_TYPE::TILE_WALL_concave_right;
	else
	{
		logError("TilesetDef: invalid tile type in tiletypes.txt, setting to floor");
		return TILESET_TILE_TYPE::_TILE_FLOOR;
	}

}

void TilesetDef::init()
{
    if (tilesets.empty())
    {
		FileParser infile;

		if (!infile.open("tiles/tiletypes.txt"))
		return;
	      	
		while (infile.next()) {
		if (infile.new_section) {
			tilesets.resize(tilesets.size()+1);
			tileset_names.resize(tilesets.size());
			tileset_locations.resize(tilesets.size());
			tileset_definitions.resize(tilesets.size());
		}
            
		if (infile.key == "name") {
			tileset_names.back() = infile.val;
		}
		else if (infile.key == "location") {
			tileset_locations.back() = infile.val;
		}
		else if (infile.key == "tileset") {
			tileset_definitions.back() += "tileset=";
			tileset_definitions.back() += infile.val;
			tileset_definitions.back() += '\n';
		}
		else if (infile.key == "tile") {
			tilesets.back()[toInt(infile.nextValue())] = toTyleType(infile.nextValue());
		}
		}

		infile.close();
    }

}

 std::string TilesetDef::tilesetDefinitions(int index)
{
	init();

	return tileset_definitions[index];
}

int TilesetDef::findTilesetByName(std::string name)
{
    for (unsigned i = 0; i < tilesets.size(); ++i) {
        if (tileset_names[i] == name) {
            return i;
        }
    }
    return -1;
}

int TilesetDef::findTilesetByLocation(std::string location)
{
    for (unsigned i = 0; i < tilesets.size(); ++i) {
        if (tileset_locations[i] == location) {
            return i;
        }
    }
    return -1;
}

int TilesetDef::getRandomTile(TILESET::TILESET _tileset, TILESET_TILE_TYPE::TILESET_TILE_TYPE type)
{
    std::map<int, TILESET_TILE_TYPE::TILESET_TILE_TYPE> tileset;
    init();

	int i = -1;
    switch(_tileset)
    {
    case TILESET::TILESET_CAVE:
		i = findTilesetByName("cave");
		if (i == -1)
		{
			logError("TilesetDef: tileset \"cave\" not defined");
			return 0;
		}
        tileset = tilesets[i];
        break;
    case TILESET::TILESET_DUNGEON:
		i = findTilesetByName("dungeon");
		if (i == -1)
		{
			logError("TilesetDef: tileset \"dungeon\" not defined");
			return 0;
		}
        tileset = tilesets[i];
        break;
    case TILESET::TILESET_GRASSLAND:
		i = findTilesetByName("grassland");
		if (i == -1)
		{
			logError("TilesetDef: tileset \"grassland\" not defined");
			return 0;
		}
        tileset = tilesets[i];
        break;
    }

    std::map<int, TILESET_TILE_TYPE::TILESET_TILE_TYPE>::iterator it = tileset.begin();
    while (it != tileset.end())
    {
      if (it->second != type)
      {
        tileset.erase(it++);
      }
      else
        ++it;
    }

    int rand_value = 0;

    if (type == TILESET_TILE_TYPE::TILE_OBJECT)
    {
        rand_value = rand() % 400;

        if (rand_value < tileset.size())
        {
            it = tileset.begin();
            std::advance(it, rand_value);
            return it->first;
        }
        else
        {
            return 0;
        }
    }
    if ( (type == TILESET_TILE_TYPE::TILE_FLOOR_RIGHT_HALF && tileset.size() == 0) ||
         (type == TILESET_TILE_TYPE::TILE_FLOOR_LEFT_HALF && tileset.size() == 0) )
    {
        return getRandomTile(_tileset, TILESET_TILE_TYPE::_TILE_FLOOR);
    }

    if (tileset.size() == 0)
        return 0;

    rand_value = rand() % tileset.size();
    it = tileset.begin();
    std::advance(it, rand_value);
    return it->first;
}
