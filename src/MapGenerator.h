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

#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

#include <iostream>
#include <vector>
#include <queue>

#define GEN_ALGORITHM_C1  0
#define GEN_ALGORITHM_D1  1
#define GEN_ALGORITHM_D2  2
#define GEN_ALGORITHM_D3  3
#define GEN_ALGORITHM_M1  4
#define GEN_ALGORITHM_T1  5

/**
 * class MapType
 *
 * The MapType class holds map data for use by the various
 * generation algorithms, at a later stage in development
 * when integrating, use Map class from Map.h.
 *
 */

typedef unsigned short maprow[256];

class MapType
{
    public:
        std::string filename;
        std::string tileset;
        std::string music_filename;
        std::vector<std::string> layernames;
        std::string title;
        short w;
        short h;
        int spawn_dir;

        //FPoint spawn;
        float spawn_x; // use FPoint once integrated
        float spawn_y; // use FPoint once integrated

        std::vector<maprow*> layers; // visible layers in maprenderer
        //std::queue<Map_Enemy> enemies;
        //std::queue<Map_NPC> npcs;
        //std::vector<Event> events;

        MapType(void);
        ~MapType(void);
};

/**
 * class MapGenerator
 *
 * The MapGenerator class is and abstract class which the
 * various map generation algorithm classes derive from.
 *
 */

class MapGenerator
{
    public:
        virtual ~MapGenerator(void) {};
        virtual void Initialize (MapType* map_pointer);
        virtual void Generate (MapType* map_pointer, int generation_algorithm) = 0;
        virtual void Export (MapType* map_pointer);
};

void MapGenerate(MapType* map_pointer, int generation_algorithm);

#endif // MAP_GENERATOR_H

