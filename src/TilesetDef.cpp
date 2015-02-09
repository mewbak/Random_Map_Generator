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
    return TILESET_TILE_TYPE::_TILE_FLOOR;
}

void TilesetDef::init()
{
    if (tilesets.empty())
    {
        /*FileParser infile;

	     if (!infile.open("tiletypes.txt"))
	      	return 0;
	      	
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
                tileset_definitions.back() += infile.val;
                tileset_definitions.back() += '\n';
            }
            else if (infile.key == "tile") {
                tilesets.back()[toInt(infile.nextValue())] = toTyleType(infile.nextValue());
            }
			}

        infile.close();
        */
        // Cave
        tilesets.resize(1);
        tileset_names.push_back("cave");
        tileset_locations.push_back("");
        tileset_definitions.push_back("");
        
        for (int i = 16; i < 31; i++)
        {
            tilesets[0][i] = TILESET_TILE_TYPE::_TILE_FLOOR;
        }

        tilesets[0][56] = TILESET_TILE_TYPE::TILE_FLOOR_RIGHT_HALF;
        tilesets[0][57] = TILESET_TILE_TYPE::TILE_FLOOR_LEFT_HALF;

        tilesets[0][132] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][133] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][134] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][135] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][48] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][49] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][50] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][51] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][52] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][53] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][54] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][55] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][144] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][145] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][146] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][147] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][148] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][149] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][150] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][151] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][152] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[0][153] = TILESET_TILE_TYPE::TILE_OBJECT;

        tilesets[0][107] = TILESET_TILE_TYPE::TILE_WALL_UP;
        tilesets[0][71] = TILESET_TILE_TYPE::TILE_WALL_UP;
        tilesets[0][67] = TILESET_TILE_TYPE::TILE_WALL_UP;

        tilesets[0][113] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        tilesets[0][109] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        tilesets[0][105] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        tilesets[0][101] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        tilesets[0][97] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        tilesets[0][69] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        tilesets[0][65] = TILESET_TILE_TYPE::TILE_WALL_DOWN;

        tilesets[0][106] = TILESET_TILE_TYPE::TILE_WALL_LEFT;
        tilesets[0][70] = TILESET_TILE_TYPE::TILE_WALL_LEFT;
        tilesets[0][66] = TILESET_TILE_TYPE::TILE_WALL_LEFT;

        tilesets[0][112] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        tilesets[0][108] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        tilesets[0][104] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        tilesets[0][100] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        tilesets[0][96] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        tilesets[0][68] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        tilesets[0][64] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;

        tilesets[0][114] = TILESET_TILE_TYPE::TILE_WALL_convex_down;
        tilesets[0][102] = TILESET_TILE_TYPE::TILE_WALL_convex_down;
        tilesets[0][98] = TILESET_TILE_TYPE::TILE_WALL_convex_down;
        tilesets[0][76] = TILESET_TILE_TYPE::TILE_WALL_convex_down;
        tilesets[0][72] = TILESET_TILE_TYPE::TILE_WALL_convex_down;

        tilesets[0][115] = TILESET_TILE_TYPE::TILE_WALL_concave_down;
        tilesets[0][103] = TILESET_TILE_TYPE::TILE_WALL_concave_down;
        tilesets[0][99] = TILESET_TILE_TYPE::TILE_WALL_concave_down;
        tilesets[0][84] = TILESET_TILE_TYPE::TILE_WALL_concave_down;
        tilesets[0][80] = TILESET_TILE_TYPE::TILE_WALL_concave_down;

        tilesets[0][77] = TILESET_TILE_TYPE::TILE_WALL_convex_left;
        tilesets[0][73] = TILESET_TILE_TYPE::TILE_WALL_convex_left;

        tilesets[0][85] = TILESET_TILE_TYPE::TILE_WALL_concave_left;
        tilesets[0][81] = TILESET_TILE_TYPE::TILE_WALL_concave_left;

        tilesets[0][78] = TILESET_TILE_TYPE::TILE_WALL_convex_up;
        tilesets[0][74] = TILESET_TILE_TYPE::TILE_WALL_convex_up;

        tilesets[0][86] = TILESET_TILE_TYPE::TILE_WALL_concave_up;
        tilesets[0][82] = TILESET_TILE_TYPE::TILE_WALL_concave_up;

        tilesets[0][79] = TILESET_TILE_TYPE::TILE_WALL_convex_right;
        tilesets[0][75] = TILESET_TILE_TYPE::TILE_WALL_convex_right;

        tilesets[0][87] = TILESET_TILE_TYPE::TILE_WALL_concave_right;
        tilesets[0][83] = TILESET_TILE_TYPE::TILE_WALL_concave_right;

        // Dungeon

        tilesets.resize(2);
        tileset_names.push_back("dungeon");
        tileset_locations.push_back("");
        tileset_definitions.push_back("");
        
        tilesets[1][16] = TILESET_TILE_TYPE::_TILE_FLOOR;
        tilesets[1][17] = TILESET_TILE_TYPE::_TILE_FLOOR;
        tilesets[1][18] = TILESET_TILE_TYPE::_TILE_FLOOR;
        for (int i = 36; i < 48; i++)
        {
            tilesets[1][i] = TILESET_TILE_TYPE::_TILE_FLOOR;
        }

        tilesets[1][176] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[1][177] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[1][178] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[1][179] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[1][180] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[1][181] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[1][182] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[1][183] = TILESET_TILE_TYPE::TILE_OBJECT;

        tilesets[1][67] = TILESET_TILE_TYPE::TILE_WALL_UP;

        tilesets[1][109] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        tilesets[1][107] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        tilesets[1][105] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        tilesets[1][103] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        tilesets[1][101] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        tilesets[1][99] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        tilesets[1][97] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        tilesets[1][69] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        tilesets[1][65] = TILESET_TILE_TYPE::TILE_WALL_DOWN;

        tilesets[1][66] = TILESET_TILE_TYPE::TILE_WALL_LEFT;

        tilesets[1][108] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        tilesets[1][106] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        tilesets[1][104] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        tilesets[1][102] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        tilesets[1][100] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        tilesets[1][96] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        tilesets[1][68] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        tilesets[1][64] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;

        tilesets[1][77] = TILESET_TILE_TYPE::TILE_WALL_convex_down;

        tilesets[1][73] = TILESET_TILE_TYPE::TILE_WALL_concave_down;

        tilesets[1][78] = TILESET_TILE_TYPE::TILE_WALL_convex_left;

        tilesets[1][74] = TILESET_TILE_TYPE::TILE_WALL_concave_left;

        tilesets[1][79] = TILESET_TILE_TYPE::TILE_WALL_convex_up;

        tilesets[1][75] = TILESET_TILE_TYPE::TILE_WALL_concave_up;

        tilesets[1][76] = TILESET_TILE_TYPE::TILE_WALL_convex_right;

        tilesets[1][72] = TILESET_TILE_TYPE::TILE_WALL_concave_right;

        // Grassland

        tilesets.resize(3);
        tileset_names.push_back("grassland");
        tileset_locations.push_back("");
        tileset_definitions.push_back("");
        
        for (int i = 16; i < 32; i++)
        {
            tilesets[2][i] = TILESET_TILE_TYPE::_TILE_FLOOR;
        }

        tilesets[2][112] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][113] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][114] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][115] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][116] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][117] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][118] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][119] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][120] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][121] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][122] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][123] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][124] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][125] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][126] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][127] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][128] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][129] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][130] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][131] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][136] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][137] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][240] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][241] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][242] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][243] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][244] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][245] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][246] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][247] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][248] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][249] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][250] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][241] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][242] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][243] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][244] = TILESET_TILE_TYPE::TILE_OBJECT;
        tilesets[2][245] = TILESET_TILE_TYPE::TILE_OBJECT;

        tilesets[2][51] = TILESET_TILE_TYPE::TILE_WALL_UP;

        tilesets[2][53] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        tilesets[2][49] = TILESET_TILE_TYPE::TILE_WALL_DOWN;

        tilesets[2][50] = TILESET_TILE_TYPE::TILE_WALL_LEFT;

        tilesets[2][52] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        tilesets[2][48] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;

        tilesets[2][60] = TILESET_TILE_TYPE::TILE_WALL_convex_down;
        tilesets[2][56] = TILESET_TILE_TYPE::TILE_WALL_convex_down;

        tilesets[2][68] = TILESET_TILE_TYPE::TILE_WALL_concave_down;
        tilesets[2][64] = TILESET_TILE_TYPE::TILE_WALL_concave_down;

        tilesets[2][61] = TILESET_TILE_TYPE::TILE_WALL_convex_left;
        tilesets[2][57] = TILESET_TILE_TYPE::TILE_WALL_convex_left;

        tilesets[2][69] = TILESET_TILE_TYPE::TILE_WALL_concave_left;
        tilesets[2][65] = TILESET_TILE_TYPE::TILE_WALL_concave_left;

        tilesets[2][62] = TILESET_TILE_TYPE::TILE_WALL_convex_up;
        tilesets[2][58] = TILESET_TILE_TYPE::TILE_WALL_convex_up;

        tilesets[2][70] = TILESET_TILE_TYPE::TILE_WALL_concave_up;
        tilesets[2][66] = TILESET_TILE_TYPE::TILE_WALL_concave_up;

        tilesets[2][63] = TILESET_TILE_TYPE::TILE_WALL_convex_right;
        tilesets[2][59] = TILESET_TILE_TYPE::TILE_WALL_convex_right;

        tilesets[2][71] = TILESET_TILE_TYPE::TILE_WALL_concave_right;
        tilesets[2][67] = TILESET_TILE_TYPE::TILE_WALL_concave_right;

    }

}

int TilesetDef::getRandomTile(TILESET::TILESET _tileset, TILESET_TILE_TYPE::TILESET_TILE_TYPE type)
{
    std::map<int, TILESET_TILE_TYPE::TILESET_TILE_TYPE> tileset;
    init();

    switch(_tileset)
    {
    case TILESET::TILESET_CAVE:
        tileset = tilesets[0];
        break;
    case TILESET::TILESET_DUNGEON:
        tileset = tilesets[1];
        break;
    case TILESET::TILESET_GRASSLAND:
        tileset = tilesets[2];
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
