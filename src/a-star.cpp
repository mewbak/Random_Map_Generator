

#include "a-star.h"
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "MapHelper.h"

int  calc_manhattan(Map* map_pointer, int tile_start, int tile_end)
{
    return (abs(map_pointer->tile[tile_start].position.x - map_pointer->tile[tile_end].position.x) + abs(map_pointer->tile[tile_start].position.y - map_pointer->tile[tile_end].position.y));
}

int  calc_G_value  (Map* map_pointer, int tile_start, int tile_end)
{
    return (((map_pointer->tile[tile_start].position.x == map_pointer->tile[tile_end].position.x) || (map_pointer->tile[tile_start].position.y == map_pointer->tile[tile_end].position.y)) ? 10 : 14);
}

void path_init_map (Map* map_pointer, int tile_start, int tile_end)
{
    for (int i= 0; i < map_pointer->size(); i++)
    {
        map_pointer->tile[i].open_list   = false;
        map_pointer->tile[i].closed_list = false;
        map_pointer->tile[i].H = calc_manhattan(map_pointer, i, tile_end);
        map_pointer->tile[i].G = 0;
        map_pointer->tile[i].F = 0;
        map_pointer->tile[i].parent = 0;
        if (map_pointer->tile[i].data == Tile_Type::TILE_PATH) map_pointer->tile[i].data = Tile_Type::TILE_FLOOR;
    }
    map_pointer->tile[tile_start].parent = tile_start;
}

void path_calc_node(Map* map_pointer, int tile_no, int tile_start, int tile_end)
{
    if ((map_pointer->tile[tile_no].data != Tile_Type::TILE_WALL) && (!map_pointer->tile[tile_no].closed_list))
    {
        if (map_pointer->tile[tile_no].open_list)
        {
            if (map_pointer->tile[tile_start].G + calc_G_value(map_pointer, tile_start, tile_no) < map_pointer->tile[tile_no].G) map_pointer->tile[tile_no].parent = tile_start;
        }
        else
        {
            map_pointer->tile[tile_no].open_list      = true;
            map_pointer->tile[tile_start].closed_list = false;
            map_pointer->tile[tile_no].H = calc_manhattan(map_pointer, tile_no, tile_end);
            map_pointer->tile[tile_no].G = map_pointer->tile[tile_start].G + calc_G_value(map_pointer, tile_start, tile_no);
            map_pointer->tile[tile_no].F = map_pointer->tile[tile_no].H + map_pointer->tile[tile_no].G;
            map_pointer->tile[tile_no].parent = tile_start;
        }
    }
    else map_pointer->tile[tile_no].closed_list = true;
}

void path_calc_nodes(Map* map_pointer, int tile_start, int tile_end)
{
    for (int i = tile_start - map_pointer->w - 1; i < (tile_start - map_pointer->w + 2); i++)
        if ((i > 0)&&(i < map_pointer->size()))
            path_calc_node(map_pointer, i, tile_start, tile_end);
    for (int i = tile_start + map_pointer->w - 1; i < (tile_start + map_pointer->w + 2); i++)
        if ((i > 0)&&(i < map_pointer->size()))
            path_calc_node(map_pointer, i, tile_start, tile_end);
    if (((tile_start - 1) > 0)&&((tile_start - 1) < map_pointer->size()))
        path_calc_node(map_pointer, tile_start - 1, tile_start, tile_end);
    if (((tile_start + 1) > 0)&&((tile_start + 1) < map_pointer->size()))
        path_calc_node(map_pointer, tile_start + 1, tile_start, tile_end);
}

/*
// Debug only function
void print_tile_data (map_type* map_pointer, int tile_no)
{
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Tile number - " << tile_no << std::endl;
    if (map_pointer->tile[tile_no].open_list) std::cout << "Open list   - " << "true" << std::endl;
    else std::cout << "Open list   - " << "false" << std::endl;
    if (map_pointer->tile[tile_no].closed_list) std::cout << "Closed list - " << "true" << std::endl;
    else std::cout << "Closed list - " << "false" << std::endl;
    std::cout << "H - value   - " << map_pointer->tile[tile_no].H << std::endl;
    std::cout << "G - value   - " << map_pointer->tile[tile_no].G << std::endl;
    std::cout << "F - value   - " << map_pointer->tile[tile_no].F << std::endl;
    std::cout << "Parent tile - " << map_pointer->tile[tile_no].parent << std::endl;
    std::cout << "D - value   - " << map_pointer->tile[tile_no].data << std::endl;
    std::cout << "-----------------------------------" << std::endl;
}
*/

/*
// Debug only function
void write_data_file(map_type* map_pointer, std::string file_name)
{
    std::ofstream output_file;
    output_file.open (file_name.c_str());
    for (int i = 0; i < map_pointer->h; i++)
    {
        for (int j = 0; j < map_pointer->w; j++) output_file << "---------"; output_file << "-" << std::endl;
        for (int j = 0; j < map_pointer->w; j++) output_file << "| D -   " << map_pointer->tile[(i*map_pointer->w)+j].data; output_file << "|" << std::endl;
        for (int j = 0; j < map_pointer->w; j++)
        {
            output_file << "| H - ";
            if (map_pointer->tile[(i*map_pointer->w)+j].H < 10) output_file << " ";
            if (map_pointer->tile[(i*map_pointer->w)+j].H < 100) output_file << " ";
            output_file << map_pointer->tile[(i*map_pointer->w)+j].H;
        }
        output_file << "|" << std::endl;
        for (int j = 0; j < map_pointer->w; j++)
        {
            output_file << "| G - ";
            if (map_pointer->tile[(i*map_pointer->w)+j].G < 10) output_file << " ";
            if (map_pointer->tile[(i*map_pointer->w)+j].G < 100) output_file << " ";
            output_file << map_pointer->tile[(i*map_pointer->w)+j].G;
        }
        output_file << "|" << std::endl;
        for (int j = 0; j < map_pointer->w; j++)
        {
            output_file << "| F - ";
            if (map_pointer->tile[(i*map_pointer->w)+j].F < 10) output_file << " ";
            if (map_pointer->tile[(i*map_pointer->w)+j].F < 100) output_file << " ";
            output_file << map_pointer->tile[(i*map_pointer->w)+j].F;
        }
        output_file << "|" << std::endl;
        for (int j = 0; j < map_pointer->w; j++)
        {
            output_file << "| C -   ";
            if (map_pointer->tile[(i*map_pointer->w)+j].closed_list) output_file << "T";
            else output_file << "F";
        }
        output_file << "|" << std::endl;
        for (int j = 0; j < map_pointer->w; j++)
        {
            output_file << "| O -   ";
            if (map_pointer->tile[(i*map_pointer->w)+j].open_list) output_file << "T";
            else output_file << "F";
        }
        output_file << "|" << std::endl;
        for (int j = 0; j < map_pointer->w; j++)
        {
            output_file << "| T - ";
            if ((i*map_pointer->w)+j < 10) output_file << " ";
            if ((i*map_pointer->w)+j < 100) output_file << " ";
            output_file << (i*map_pointer->w)+j;
        }
        output_file << "|" << std::endl;
        for (int j = 0; j < map_pointer->w; j++)
        {
            output_file << "| P - ";
            if (map_pointer->tile[(i*map_pointer->w)+j].parent < 10) output_file << " ";
            if (map_pointer->tile[(i*map_pointer->w)+j].parent < 100) output_file << " ";
            output_file << map_pointer->tile[(i*map_pointer->w)+j].parent;
        }
        output_file << "|" << std::endl;
    }
  output_file.close();
}
*/

int get_next_tile(Map* map_pointer, int tile_start, int tile_end)
{
    path_calc_nodes(map_pointer, tile_start, tile_end);
    int tile_next = tile_start;
    map_pointer->tile[tile_next].open_list = false;
    map_pointer->tile[tile_next].closed_list = true;
    for (int i = 0; i < map_pointer->size(); i++)
    {
        if (map_pointer->tile[i].open_list) tile_next = i;
    }
    for (int i = 0; i < map_pointer->size(); i++)
    {
        if ((map_pointer->tile[i].open_list) && (map_pointer->tile[i].F < map_pointer->tile[tile_next].F)) tile_next = i;
    }
    return tile_next;
}

int path_find_recursive(Map* map_pointer, int tile_start, int tile_end)
{
    int tile_next = get_next_tile(map_pointer, tile_start, tile_end);
        //map_pointer->tile[tile_next].data = TILE_PATH; // debug
    if ((tile_next != tile_end) && (tile_next != tile_start))
    {
        tile_next = path_find_recursive(map_pointer, tile_next, tile_end);
    }
    return tile_next;
}

int get_path_length(Map* map_pointer, int tile_start, int tile_end)
{
    int p_temp = 0;
    if (tile_start != tile_end) p_temp = get_path_length(map_pointer ,tile_start,map_pointer->tile[tile_end].parent);
    return ++p_temp;
}

void display_path_data(path_type* path_data)
{
    for (int i = 0; i < path_data->length; i++)
    {
        std::cout << path_data->data[i] << std::endl;
    }
}

void get_path_data(Map* map_pointer, path_type* path_pointer, int tile_start, int tile_end)
{
    path_pointer->length = get_path_length(map_pointer ,tile_start,tile_end);
    //std::cout << "Path length - " << path_pointer->length << std::endl; // debug
    path_pointer->data = new int[path_pointer->length];
    int tile_next = tile_end;
    for (int i = 0; i < path_pointer->length; i++)
    {
        map_pointer->tile[tile_next].data = Tile_Type::TILE_PATH; // debug
        path_pointer->data[path_pointer->length-i-1] = tile_next;
        tile_next = map_pointer->tile[tile_next].parent;
    }
    //display_path_data(path_pointer); // debug
}

bool path_find(Map* map_pointer, path_type* path_pointer, int tile_start, int tile_end)
{
    path_init_map (map_pointer ,tile_start ,tile_end);
    bool path_found = (path_find_recursive(map_pointer, tile_start, tile_end) == tile_end);
    if (path_found) get_path_data(map_pointer, path_pointer, tile_start, tile_end);
    //write_data_file(map_pointer,"tile_data.txt");
    return path_found;
}

bool path_find(Map* map_pointer, int tile_start, int tile_end)
{
    path_init_map (map_pointer ,tile_start ,tile_end);
    return (path_find_recursive(map_pointer, tile_start, tile_end) == tile_end);
}

