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

#include <cstdlib>

#include "MapGenerator.h"
#include "MapGenerator_C1.h"

void MapGenerator_C1::Generate (MapType* map_pointer)
{
    #define TILE_FLOOR 0
    #define TILE_WALL  1
    Initialize(map_pointer);
    maprow *current_layer = new maprow[map_pointer->w];
    map_pointer->layers.push_back(current_layer);
    map_pointer->layernames.push_back("background");
    // set all tiles to floor, except perimeter set to wall
    for (int j = 0; j < map_pointer->h; j++)
    {
        for (int i = 0; i < map_pointer->w; i++)
        {
            if ((j == 0)||(j == map_pointer->h-1)||(i == 0)||(i == map_pointer->w-1))
                (current_layer)[i][j] = TILE_WALL;
            else
                (current_layer)[i][j] = TILE_FLOOR;
        }
    }
    // place random wall tiles
    for (int i = 0; i < ((map_pointer->w * map_pointer->h) * 0.6); i++)
    {
        (current_layer)[rand() % map_pointer->w][rand() % map_pointer->h] = TILE_WALL;
    }
    for (int i = 0; i < 2; i++) // iterate over map and "smooth" it
    {
        for (int j = 1; j < map_pointer->h-1; j++)
        {
            for (int i = 1; i < map_pointer->w-1; i++)
            {
                int num_walls = 0;
                if ((current_layer)[i][j-1] == TILE_WALL) num_walls++;
                if ((current_layer)[i][j+1] == TILE_WALL) num_walls++;
                if ((current_layer)[i-1][j] == TILE_WALL) num_walls++;
                if ((current_layer)[i-1][j-1] == TILE_WALL) num_walls++;
                if ((current_layer)[i-1][j+1] == TILE_WALL) num_walls++;
                if ((current_layer)[i+1][j] == TILE_WALL) num_walls++;
                if ((current_layer)[i+1][j-1] == TILE_WALL) num_walls++;
                if ((current_layer)[i+1][j+1] == TILE_WALL) num_walls++;
                if (((current_layer)[i][j] == TILE_WALL)&&(num_walls > 3)) (current_layer)[i][j] = TILE_WALL;
                else if (((current_layer)[i][j] == TILE_FLOOR)&&(num_walls > 4)) (current_layer)[i][j] = TILE_WALL;
                else (current_layer)[i][j] = TILE_FLOOR;
            }
        }
    }

    Export(map_pointer); // debug
}

/*

a lot more work needed:
clear center block after random wall placement.
add a few random lines in the middle to prevent large open cave?
fill all but main cave to prevent redundant rooms that are inaccessible.
flood fill?

*/
