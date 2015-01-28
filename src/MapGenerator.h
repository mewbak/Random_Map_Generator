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
#include "Map.h"

/**
 * class MapType
 *
 * The MapType class holds map data for use by the various
 * generation algorithms, at a later stage in development
 * when integrating, use Map class from Map.h.
 *
 */

typedef unsigned short maprow[256];
/*
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
*/
/**
 * class MapGenerator
 *
 * The MapGenerator class is and abstract class which the
 * various map generation algorithm classes derive from.
 *
 */

// this type should not be needed in future
struct flare_map_type
{
    // misc
    int tileset; // generator only var
    //header
    int width;
    int height;
    int no_of_tiles;
    int tile_width;
    int tile_height;
    std::string music_file_name;
    std::string tileset_file_name;
    std::string map_name;
    //layer
    int* layer_background;
    int* layer_fringe;
    int* layer_object;
    int* layer_foreground;
    int* layer_collision;
    //events

    //npc

    //enemy

    //enemygroup

};

class MapGenerator
{
    public:
        virtual ~MapGenerator(void) {};
        void Initialize (Map* map_pointer, int dimension_x, int dimension_y);
        virtual void Generate (Map* map_pointer, int dimension_x, int dimension_y) = 0;
        static void Export (Map* map_pointer, std::string file_name);

        // this should not be needed in future
        static void map_to_flare_map (Map* map_pointer, flare_map_type* flare_map_pointer, int tile_set);

        // instead of file_export_flare should be used Export function
        static void file_export_flare (flare_map_type* flare_map_pointer, std::string file_name);
};

#endif // MAP_GENERATOR_H

