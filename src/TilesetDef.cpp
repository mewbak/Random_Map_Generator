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

std::map<int, TILESET_TILE_TYPE::TILESET_TILE_TYPE> TilesetDef::cave;
std::map<int, TILESET_TILE_TYPE::TILESET_TILE_TYPE> TilesetDef::dungeon;
std::map<int, TILESET_TILE_TYPE::TILESET_TILE_TYPE> TilesetDef::grassland;

TilesetDef::TilesetDef()
{
}

TilesetDef::~TilesetDef()
{
}

void TilesetDef::init()
{
    if (cave.empty() || dungeon.empty() || grassland.empty())
    {
        // Cave

        for (int i = 16; i < 31; i++)
        {
            cave[i] = TILESET_TILE_TYPE::_TILE_FLOOR;
        }

        cave[56] = TILESET_TILE_TYPE::TILE_FLOOR_RIGHT_HALF;
        cave[57] = TILESET_TILE_TYPE::TILE_FLOOR_LEFT_HALF;

        cave[132] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[133] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[134] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[135] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[48] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[49] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[50] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[51] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[52] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[53] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[54] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[55] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[144] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[145] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[146] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[147] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[148] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[149] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[150] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[151] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[152] = TILESET_TILE_TYPE::TILE_OBJECT;
        cave[153] = TILESET_TILE_TYPE::TILE_OBJECT;

        cave[107] = TILESET_TILE_TYPE::TILE_WALL_UP;
        cave[71] = TILESET_TILE_TYPE::TILE_WALL_UP;
        cave[67] = TILESET_TILE_TYPE::TILE_WALL_UP;

        cave[113] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        cave[109] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        cave[105] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        cave[101] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        cave[97] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        cave[69] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        cave[65] = TILESET_TILE_TYPE::TILE_WALL_DOWN;

        cave[106] = TILESET_TILE_TYPE::TILE_WALL_LEFT;
        cave[70] = TILESET_TILE_TYPE::TILE_WALL_LEFT;
        cave[66] = TILESET_TILE_TYPE::TILE_WALL_LEFT;

        cave[112] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        cave[108] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        cave[104] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        cave[100] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        cave[96] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        cave[68] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        cave[64] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;

        cave[114] = TILESET_TILE_TYPE::TILE_WALL_convex_down;
        cave[102] = TILESET_TILE_TYPE::TILE_WALL_convex_down;
        cave[98] = TILESET_TILE_TYPE::TILE_WALL_convex_down;
        cave[76] = TILESET_TILE_TYPE::TILE_WALL_convex_down;
        cave[72] = TILESET_TILE_TYPE::TILE_WALL_convex_down;

        cave[115] = TILESET_TILE_TYPE::TILE_WALL_concave_down;
        cave[103] = TILESET_TILE_TYPE::TILE_WALL_concave_down;
        cave[99] = TILESET_TILE_TYPE::TILE_WALL_concave_down;
        cave[84] = TILESET_TILE_TYPE::TILE_WALL_concave_down;
        cave[80] = TILESET_TILE_TYPE::TILE_WALL_concave_down;

        cave[77] = TILESET_TILE_TYPE::TILE_WALL_convex_left;
        cave[73] = TILESET_TILE_TYPE::TILE_WALL_convex_left;

        cave[85] = TILESET_TILE_TYPE::TILE_WALL_concave_left;
        cave[81] = TILESET_TILE_TYPE::TILE_WALL_concave_left;

        cave[78] = TILESET_TILE_TYPE::TILE_WALL_convex_up;
        cave[74] = TILESET_TILE_TYPE::TILE_WALL_convex_up;

        cave[86] = TILESET_TILE_TYPE::TILE_WALL_concave_up;
        cave[82] = TILESET_TILE_TYPE::TILE_WALL_concave_up;

        cave[79] = TILESET_TILE_TYPE::TILE_WALL_convex_right;
        cave[75] = TILESET_TILE_TYPE::TILE_WALL_convex_right;

        cave[87] = TILESET_TILE_TYPE::TILE_WALL_concave_right;
        cave[83] = TILESET_TILE_TYPE::TILE_WALL_concave_right;

        // Dungeon

        dungeon[16] = TILESET_TILE_TYPE::_TILE_FLOOR;
        dungeon[17] = TILESET_TILE_TYPE::_TILE_FLOOR;
        dungeon[18] = TILESET_TILE_TYPE::_TILE_FLOOR;
        for (int i = 36; i < 48; i++)
        {
            dungeon[i] = TILESET_TILE_TYPE::_TILE_FLOOR;
        }

        dungeon[176] = TILESET_TILE_TYPE::TILE_OBJECT;
        dungeon[177] = TILESET_TILE_TYPE::TILE_OBJECT;
        dungeon[178] = TILESET_TILE_TYPE::TILE_OBJECT;
        dungeon[179] = TILESET_TILE_TYPE::TILE_OBJECT;
        dungeon[180] = TILESET_TILE_TYPE::TILE_OBJECT;
        dungeon[181] = TILESET_TILE_TYPE::TILE_OBJECT;
        dungeon[182] = TILESET_TILE_TYPE::TILE_OBJECT;
        dungeon[183] = TILESET_TILE_TYPE::TILE_OBJECT;

        dungeon[67] = TILESET_TILE_TYPE::TILE_WALL_UP;

        dungeon[109] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        dungeon[107] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        dungeon[105] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        dungeon[103] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        dungeon[101] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        dungeon[99] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        dungeon[97] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        dungeon[69] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        dungeon[65] = TILESET_TILE_TYPE::TILE_WALL_DOWN;

        dungeon[66] = TILESET_TILE_TYPE::TILE_WALL_LEFT;

        dungeon[108] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        dungeon[106] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        dungeon[104] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        dungeon[102] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        dungeon[100] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        dungeon[96] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        dungeon[68] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        dungeon[64] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;

        dungeon[77] = TILESET_TILE_TYPE::TILE_WALL_convex_down;

        dungeon[73] = TILESET_TILE_TYPE::TILE_WALL_concave_down;

        dungeon[78] = TILESET_TILE_TYPE::TILE_WALL_convex_left;

        dungeon[74] = TILESET_TILE_TYPE::TILE_WALL_concave_left;

        dungeon[79] = TILESET_TILE_TYPE::TILE_WALL_convex_up;

        dungeon[75] = TILESET_TILE_TYPE::TILE_WALL_concave_up;

        dungeon[76] = TILESET_TILE_TYPE::TILE_WALL_convex_right;

        dungeon[72] = TILESET_TILE_TYPE::TILE_WALL_concave_right;

        // Grassland

        for (int i = 16; i < 32; i++)
        {
            grassland[i] = TILESET_TILE_TYPE::_TILE_FLOOR;
        }

        grassland[112] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[113] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[114] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[115] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[116] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[117] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[118] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[119] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[120] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[121] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[122] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[123] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[124] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[125] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[126] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[127] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[128] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[129] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[130] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[131] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[136] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[137] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[240] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[241] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[242] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[243] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[244] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[245] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[246] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[247] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[248] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[249] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[250] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[241] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[242] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[243] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[244] = TILESET_TILE_TYPE::TILE_OBJECT;
        grassland[245] = TILESET_TILE_TYPE::TILE_OBJECT;

        grassland[51] = TILESET_TILE_TYPE::TILE_WALL_UP;

        grassland[53] = TILESET_TILE_TYPE::TILE_WALL_DOWN;
        grassland[49] = TILESET_TILE_TYPE::TILE_WALL_DOWN;

        grassland[50] = TILESET_TILE_TYPE::TILE_WALL_LEFT;

        grassland[52] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;
        grassland[48] = TILESET_TILE_TYPE::TILE_WALL_RIGHT;

        grassland[60] = TILESET_TILE_TYPE::TILE_WALL_convex_down;
        grassland[56] = TILESET_TILE_TYPE::TILE_WALL_convex_down;

        grassland[68] = TILESET_TILE_TYPE::TILE_WALL_concave_down;
        grassland[64] = TILESET_TILE_TYPE::TILE_WALL_concave_down;

        grassland[61] = TILESET_TILE_TYPE::TILE_WALL_convex_left;
        grassland[57] = TILESET_TILE_TYPE::TILE_WALL_convex_left;

        grassland[69] = TILESET_TILE_TYPE::TILE_WALL_concave_left;
        grassland[65] = TILESET_TILE_TYPE::TILE_WALL_concave_left;

        grassland[62] = TILESET_TILE_TYPE::TILE_WALL_convex_up;
        grassland[58] = TILESET_TILE_TYPE::TILE_WALL_convex_up;

        grassland[70] = TILESET_TILE_TYPE::TILE_WALL_concave_up;
        grassland[66] = TILESET_TILE_TYPE::TILE_WALL_concave_up;

        grassland[63] = TILESET_TILE_TYPE::TILE_WALL_convex_right;
        grassland[59] = TILESET_TILE_TYPE::TILE_WALL_convex_right;

        grassland[71] = TILESET_TILE_TYPE::TILE_WALL_concave_right;
        grassland[67] = TILESET_TILE_TYPE::TILE_WALL_concave_right;

    }

}

int TilesetDef::getRandomTile(TILESET::TILESET _tileset, TILESET_TILE_TYPE::TILESET_TILE_TYPE type)
{
    std::map<int, TILESET_TILE_TYPE::TILESET_TILE_TYPE> tileset;
    init();

    switch(_tileset)
    {
    case TILESET::TILESET_CAVE:
        tileset = cave;
        break;
    case TILESET::TILESET_DUNGEON:
        tileset = dungeon;
        break;
    case TILESET::TILESET_GRASSLAND:
        tileset = grassland;
        break;
    }

    std::map<int, TILESET_TILE_TYPE::TILESET_TILE_TYPE>::iterator it = tileset.begin();
    while (it != tileset.end())
    {
      if (it->second != type)
      {
        it = tileset.erase(it);
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
