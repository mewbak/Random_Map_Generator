#include <iostream>
#include "a-star.h"
#include "Map.h"
#include "MapHelper.h"
#include "MapGenerator.h"
#include "MapGenerator_C1.h"
#include "MapGenerator_D1.h"

int get_A(Map* map_pointer)
{
    for (int i = map_pointer->w*4+2; i < map_pointer->size(); i+=1)
    {
        if ((i < map_pointer->size()) && (map_pointer->tile[i].data == Tile_Type::TILE_FLOOR)) return(i);
    }
    return map_pointer->w+2;
}

int get_B(Map* map_pointer)
{
    for (int i = map_pointer->size()-map_pointer->w*4-2; i > 0; i-=1)
    {
        if ((i > 0) && (map_pointer->tile[i].data == Tile_Type::TILE_FLOOR)) return(i);
    }
    return map_pointer->size()-map_pointer->w-2;
}

void MapGenerate(Map* map_pointer, int generation_algorithm, int dimension_x, int dimension_y)
{
    MapGenerator* generator = NULL;
    if (generation_algorithm == GEN_ALGORITHM_C1) {
        generator = new MapGenerator_C1();
    }
    else if (generation_algorithm == GEN_ALGORITHM_D1) {
        generator = new MapGenerator_D1();
    }
    generator->Generate(map_pointer, dimension_x, dimension_y);
    delete generator;

    /*int return_value = EXIT_SUCCESS;
    switch (gen_type)
    {
        case Algorithm_Type::MAP_GEN_BSP:
            map_gen_BSP(map_pointer);
        break;
        case Algorithm_Type::MAP_GEN_CA:
            map_gen_CA(map_pointer);
        break;
        case Algorithm_Type::MAP_GEN_M1:
            map_gen_M1(map_pointer);
        break;
        case Algorithm_Type::MAP_GEN_RC:
            map_gen_RC(map_pointer);
        break;
        case Algorithm_Type::MAP_GEN_RR:
            map_gen_RR(map_pointer);
        break;
        default:
            return_value = EXIT_FAILURE;
        break;
    }

    return return_value;*/
}

void map_check   (Map* map_pointer)
{
    int iterations  = 2;
    int mw          = map_pointer->w;
    for (int iteration_count = 0; iteration_count < iterations; iteration_count++)
    {
        for (int tile_count = mw; tile_count < (map_pointer->size()-map_pointer->w-1); tile_count++)
        {
            if(map_pointer->tile[tile_count].data == Tile_Type::TILE_WALL)
            {
                // Remove single tiles
                // OOO
                // OXO
                // OOO
                //--------------------
                if(  (map_pointer->tile[tile_count+1].data    == Tile_Type::TILE_FLOOR)
                   &&(map_pointer->tile[tile_count-1].data    == Tile_Type::TILE_FLOOR)
                   &&(map_pointer->tile[tile_count+mw].data   == Tile_Type::TILE_FLOOR)
                   &&(map_pointer->tile[tile_count+mw+1].data == Tile_Type::TILE_FLOOR)
                   &&(map_pointer->tile[tile_count+mw-1].data == Tile_Type::TILE_FLOOR)
                   &&(map_pointer->tile[tile_count-mw].data   == Tile_Type::TILE_FLOOR)
                   &&(map_pointer->tile[tile_count-mw+1].data == Tile_Type::TILE_FLOOR)
                   &&(map_pointer->tile[tile_count-mw-1].data == Tile_Type::TILE_FLOOR))
                {
                    map_pointer->tile[tile_count].data = Tile_Type::TILE_FLOOR;
                }
                // Remove single horizontal tiles
                // ???
                // OXO
                // ???
                //--------------------
                if(  (map_pointer->tile[tile_count+1].data    == Tile_Type::TILE_FLOOR)
                   &&(map_pointer->tile[tile_count-1].data    == Tile_Type::TILE_FLOOR))
                {
                    map_pointer->tile[tile_count].data = Tile_Type::TILE_FLOOR;
                }
                // Remove single vertical tiles
                // ?O?
                // ?X?
                // ?O?
                //--------------------
                if(  (map_pointer->tile[tile_count+mw].data   == Tile_Type::TILE_FLOOR)
                   &&(map_pointer->tile[tile_count-mw].data   == Tile_Type::TILE_FLOOR))
                {
                    map_pointer->tile[tile_count].data = Tile_Type::TILE_FLOOR;
                }
                // Remove cross right tiles
                // O?X
                // ?X?
                // X?O
                //--------------------
                if(  (map_pointer->tile[tile_count+mw+1].data == Tile_Type::TILE_WALL)
                   &&(map_pointer->tile[tile_count+mw-1].data == Tile_Type::TILE_FLOOR)
                   &&(map_pointer->tile[tile_count-mw+1].data == Tile_Type::TILE_FLOOR)
                   &&(map_pointer->tile[tile_count-mw-1].data == Tile_Type::TILE_WALL))
                {
                    map_pointer->tile[tile_count+mw-1].data = Tile_Type::TILE_WALL;
                    map_pointer->tile[tile_count-mw+1].data = Tile_Type::TILE_WALL;
                }
                // Remove cross left tiles
                // X?O
                // ?X?
                // O?X
                //--------------------
                if ((tile_count-mw-1 >= 0)
                   &&(map_pointer->tile[tile_count+mw+1].data == Tile_Type::TILE_FLOOR)
                   &&(map_pointer->tile[tile_count+mw-1].data == Tile_Type::TILE_WALL)
                   &&(map_pointer->tile[tile_count-mw+1].data == Tile_Type::TILE_WALL)
                   &&(map_pointer->tile[tile_count-mw-1].data == Tile_Type::TILE_FLOOR))
                {
                    map_pointer->tile[tile_count+mw+1].data = Tile_Type::TILE_WALL;
                    map_pointer->tile[tile_count-mw-1].data = Tile_Type::TILE_WALL;
                }
            }
        }
    }
}

void map_display (Map* map_pointer)
{
    for (int i = 0; i < map_pointer->h; i++)
    {
        for (int j = 0; j < map_pointer->w; j++)
        {
            switch (map_pointer->tile[(i*map_pointer->w)+j].data)
            {
                case Tile_Type::TILE_WALL:
                    std::cout << "|";
                break;
                case Tile_Type::TILE_FLOOR:
                    std::cout << " ";
                break;
                case Tile_Type::TILE_EXIT:
                    std::cout << "E";
                break;
                case Tile_Type::TILE_DOOR:
                    std::cout << "D";
                break;
                case Tile_Type::TILE_KEY:
                    std::cout << "K";
                break;
                case Tile_Type::TILE_PATH:
                    std::cout << ".";
                break;
                case Tile_Type::TILE_NONE:
                default:
                    std::cout << "?";
                break;
            }
        }
        std::cout << std::endl;
    }
}

int map_gen_flood_fill_tile (Map* map_pointer, FloodFill* fill_data, int tile_number)
{
    int return_value = 0;
    if ((fill_data[tile_number].tile_data == Tile_Type::TILE_FLOOR) && (!fill_data[tile_number].processed))
    {
        fill_data[tile_number].processed = true;
        fill_data[tile_number].adjoining_tile = true;
        return_value++;
        if ((tile_number+1) <= map_pointer->size()) return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number+1);
        if ((tile_number-1) >= 0) return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number-1);
        if ((tile_number+map_pointer->w) <= map_pointer->size()) return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number+map_pointer->w);
        if ((tile_number+map_pointer->w+1) <= map_pointer->size()) return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number+map_pointer->w+1);
        if ((tile_number+map_pointer->w-1) <= map_pointer->size()) return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number+map_pointer->w-1);
        if ((tile_number-map_pointer->w) >= 0) return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number-map_pointer->w);
        if ((tile_number-map_pointer->w+1) >= 0) return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number-map_pointer->w+1);
        if ((tile_number-map_pointer->w-1) >= 0) return_value += map_gen_flood_fill_tile(map_pointer,fill_data,tile_number-map_pointer->w-1);
    }
    return (return_value);
}

bool map_gen_room_flood_fill (Map* map_pointer)
{
    int  floor_count  = 0;
    int  first_floor  = -1;
    bool return_value = true;
    FloodFill* fill_data = new FloodFill[map_pointer->size()];
    for(int tile_count = 0; tile_count < map_pointer->size(); tile_count++)
    {
        if (map_pointer->tile[tile_count].data == Tile_Type::TILE_FLOOR) floor_count++;
        if ((floor_count == 1) && (first_floor < 0)) first_floor = tile_count;
        fill_data[tile_count].tile_data       = map_pointer->tile[tile_count].data ;
        fill_data[tile_count].processed       = false;
        fill_data[tile_count].adjoining_tile  = false;
    }
    int   number_found = map_gen_flood_fill_tile(map_pointer,fill_data,first_floor);
    if (number_found < floor_count) return_value = false;
    //std::cout << "found - " << number_found << " of " << floor_count << std::endl;
    delete[] fill_data;
    return (return_value);
}

bool map_gen_room (Map* map_pointer, Room room)
{
    bool return_value = (map_pointer->no_of_rooms < ROOM_MAX);
    if (return_value)
    {
        int map_boundry = 4;
        if (room.position.x <= 0) room.position.x = map_boundry + rand() % (map_pointer->w -((ROOM_MAX_X+2)*2)+(ROOM_MAX_X+2)-(map_boundry*2));
        if (room.position.y <= 0) room.position.y = map_boundry + rand() % (map_pointer->h-((ROOM_MAX_Y+2)*2)+(ROOM_MAX_Y+2)-(map_boundry*2));
        if (room.w <= 0) room.w = ROOM_MIN_X + rand() % ROOM_MAX_X;
        if (room.h <= 0) room.h = ROOM_MIN_Y + rand() % ROOM_MAX_Y;
        //int temp_pos_x = room.position.x - (room.w/2)-1;
        //int temp_pos_y = room.position.y - (room.h/2)-1;
        int temp_tile = 0;
        for (int count_x = 0; count_x < room.w+2; count_x++)
        {
            for (int count_y = 0; count_y < room.h+2; count_y++)
            {
                temp_tile = (map_pointer->w*(room.position.y+count_y))+(room.position.x+count_x);
                if ((temp_tile > 0) && (temp_tile < (map_pointer->size()-map_pointer->w)) && (map_pointer->tile[temp_tile].data != Tile_Type::TILE_WALL)) return_value = false;
            }
        }
        if (return_value)
        {
            for (int count_x = 1; count_x < room.w; count_x++)
            {
                for (int count_y = 1; count_y < room.h; count_y++)
                {
                    temp_tile = (map_pointer->w*(room.position.y+count_y))+(room.position.x+count_x);
                    if  ((temp_tile > 0) && (temp_tile < map_pointer->size())) map_pointer->tile[temp_tile].data = Tile_Type::TILE_FLOOR;
                }
            }
            map_pointer->room[map_pointer->no_of_rooms].active      = true;
            map_pointer->room[map_pointer->no_of_rooms].position    = room.position;

            map_pointer->room[map_pointer->no_of_rooms].w   = room.w;
            map_pointer->room[map_pointer->no_of_rooms].h   = room.h;

            map_pointer->room[map_pointer->no_of_rooms].position.y += map_pointer->room[map_pointer->no_of_rooms].h / 2;
            map_pointer->room[map_pointer->no_of_rooms].position.x += map_pointer->room[map_pointer->no_of_rooms].w / 2;
            map_pointer->no_of_rooms++;
        }
    }
    return (return_value);
}

void map_gen_room_exits(Map* map_pointer)
{
    for (int i = 0; i < map_pointer->no_of_rooms; i++)
    {
        if (map_pointer->room[i].active)
        {
            int exit_tile = 0;
            int exit_count = 0;
            // bottom exit
            if (50 > rand() % 100)
            {
                exit_count++;
                exit_tile = map_pointer->room[i].position.x+(map_pointer->w*map_pointer->room[i].position.y);
                while ((exit_tile >= 0)&&(exit_tile < map_pointer->size())&&(map_pointer->tile[exit_tile].data == Tile_Type::TILE_FLOOR))
                {
                    exit_tile += map_pointer->w;
                }
                while ((exit_tile >= 0)&&(exit_tile < map_pointer->size())&&(map_pointer->tile[exit_tile].data != Tile_Type::TILE_FLOOR))
                {
                    map_pointer->tile[exit_tile].data = Tile_Type::TILE_FLOOR;
                    exit_tile += map_pointer->w;
                }
                map_pointer->room[i].exit[0] = true;
            }
            // top exit
            if (50 > rand() % 100)
            {
                exit_count++;
                exit_tile = map_pointer->room[i].position.x+(map_pointer->w*map_pointer->room[i].position.y);
                while ((exit_tile >= 0)&&(exit_tile < map_pointer->size())&&(map_pointer->tile[exit_tile].data == Tile_Type::TILE_FLOOR))
                {
                    exit_tile -= map_pointer->w;
                }
                while ((exit_tile >= 0)&&(exit_tile < map_pointer->size())&&(map_pointer->tile[exit_tile].data != Tile_Type::TILE_FLOOR))
                {
                    map_pointer->tile[exit_tile].data = Tile_Type::TILE_FLOOR;
                    exit_tile -= map_pointer->w;
                }
                map_pointer->room[i].exit[1] = true;
            }
            // right exit
            if (50 > rand() % 100)
            {
                exit_count++;
                exit_tile = map_pointer->room[i].position.x+(map_pointer->w*map_pointer->room[i].position.y);
                while ((exit_tile >= 0)&&(exit_tile < map_pointer->size())&&(map_pointer->tile[exit_tile].data == Tile_Type::TILE_FLOOR))
                {
                    exit_tile++;
                }
                while ((exit_tile >= 0)&&(exit_tile < map_pointer->size())&&(map_pointer->tile[exit_tile].data != Tile_Type::TILE_FLOOR))
                {
                    map_pointer->tile[exit_tile].data = Tile_Type::TILE_FLOOR;
                    exit_tile++;
                }
                map_pointer->room[i].exit[2] = true;
            }
            // left exit
            if ((exit_count <= 0) || (50 > rand() % 100))
            {
                exit_count++;
                exit_tile = map_pointer->room[i].position.x+(map_pointer->w*map_pointer->room[i].position.y);
                while ((exit_tile >= 0)&&(exit_tile < map_pointer->size())&&(map_pointer->tile[exit_tile].data == Tile_Type::TILE_FLOOR))
                {
                    exit_tile--;
                }
                while ((exit_tile >= 0)&&(exit_tile < map_pointer->size())&&(map_pointer->tile[exit_tile].data != Tile_Type::TILE_FLOOR))
                {
                    map_pointer->tile[exit_tile].data = Tile_Type::TILE_FLOOR;
                    exit_tile--;
                }
                map_pointer->room[i].exit[3] = true;
            }
        }
    }
}

int map_gen_room_check_path (Map* map_pointer, int room_1, int room_2, bool x_then_y)
{
    int return_value         = 0;
    int to_floor_transitions = 0;
    int r1_x = map_pointer->room[room_1].position.x;
    int r1_y = map_pointer->room[room_1].position.y;
    int r2_x = map_pointer->room[room_2].position.x;
    int r2_y = map_pointer->room[room_2].position.y;
    if ((r1_x >= 0) && (r1_x < map_pointer->w) &&
        (r1_y >= 0) && (r1_y < map_pointer->h) &&
        (r2_x >= 0) && (r2_x < map_pointer->w) &&
        (r2_y >= 0) && (r2_y < map_pointer->h))
    {
        int previous_tile        = Tile_Type::TILE_NONE;
        if (x_then_y)
        {
            previous_tile = Tile_Type::TILE_NONE;
            to_floor_transitions = 0;
            int x_count = r1_x;
            int y_count = r1_y;
            if (r1_x > r2_x)
            {
                for (x_count = r1_x; x_count > r2_x; x_count--)
                {
                    int tile_count = x_count + (map_pointer->w * (y_count));
                    if (map_pointer->tile[tile_count].data == Tile_Type::TILE_WALL) return_value++;
                    if ((map_pointer->tile[tile_count].data == Tile_Type::TILE_FLOOR) && (previous_tile == Tile_Type::TILE_WALL)) to_floor_transitions++;
                    previous_tile = map_pointer->tile[tile_count].data;
                }
            }
            else
            {
                for (x_count = r1_x; x_count < r2_x; x_count++)
                {
                    int tile_count = x_count + (map_pointer->w * (y_count));
                    if (map_pointer->tile[tile_count].data == Tile_Type::TILE_WALL) return_value++;
                    if ((map_pointer->tile[tile_count].data == Tile_Type::TILE_FLOOR) && (previous_tile == Tile_Type::TILE_WALL)) to_floor_transitions++;
                    previous_tile = map_pointer->tile[tile_count].data;
                }
            }
            if (r1_y > r2_y)
            {
                for (y_count = r1_y; y_count > r2_y; y_count--)
                {
                    int tile_count = x_count + (map_pointer->w * (y_count));
                    if (map_pointer->tile[tile_count].data == Tile_Type::TILE_WALL) return_value++;
                    if ((map_pointer->tile[tile_count].data == Tile_Type::TILE_FLOOR) && (previous_tile == Tile_Type::TILE_WALL)) to_floor_transitions++;
                    previous_tile = map_pointer->tile[tile_count].data;
                }
            }
            else
            {
                for (y_count = r1_y; y_count < r2_y; y_count++)
                {
                    int tile_count = x_count + (map_pointer->w * (y_count));
                    if (map_pointer->tile[tile_count].data == Tile_Type::TILE_WALL) return_value++;
                    if ((map_pointer->tile[tile_count].data == Tile_Type::TILE_FLOOR) && (previous_tile == Tile_Type::TILE_WALL)) to_floor_transitions++;
                    previous_tile = map_pointer->tile[tile_count].data;
                }
            }
        }
        else
        {
            previous_tile = Tile_Type::TILE_NONE;
            to_floor_transitions = 0;
            int x_count = r1_x;
            int y_count = r1_y;
            if (r1_y > r2_y)
            {
                for (y_count = r1_y; y_count > r2_y; y_count--)
                {
                    int tile_count = x_count + (map_pointer->w * (y_count));
                    if (map_pointer->tile[tile_count].data == Tile_Type::TILE_WALL) return_value++;
                    if ((map_pointer->tile[tile_count].data == Tile_Type::TILE_FLOOR) && (previous_tile == Tile_Type::TILE_WALL)) to_floor_transitions++;
                    previous_tile = map_pointer->tile[tile_count].data;
                }
            }
            else
            {
                for (y_count = r1_y; y_count < r2_y; y_count++)
                {
                    int tile_count = x_count + (map_pointer->w * (y_count));
                    if (map_pointer->tile[tile_count].data == Tile_Type::TILE_WALL) return_value++;
                    if ((map_pointer->tile[tile_count].data == Tile_Type::TILE_FLOOR) && (previous_tile == Tile_Type::TILE_WALL)) to_floor_transitions++;
                    previous_tile = map_pointer->tile[tile_count].data;
                }
            }
            if (r1_x > r2_x)
            {
                for (x_count = r1_x; x_count > r2_x; x_count--)
                {
                    int tile_count = x_count + (map_pointer->w * (y_count));
                    if (map_pointer->tile[tile_count].data == Tile_Type::TILE_WALL) return_value++;
                    if ((map_pointer->tile[tile_count].data == Tile_Type::TILE_FLOOR) && (previous_tile == Tile_Type::TILE_WALL)) to_floor_transitions++;
                    previous_tile = map_pointer->tile[tile_count].data;
                }
            }
            else
            {
                for (x_count = r1_x; x_count < r2_x; x_count++)
                {
                    int tile_count = x_count + (map_pointer->w * (y_count));
                    if (map_pointer->tile[tile_count].data == Tile_Type::TILE_WALL) return_value++;
                    if ((map_pointer->tile[tile_count].data == Tile_Type::TILE_FLOOR) && (previous_tile == Tile_Type::TILE_WALL)) to_floor_transitions++;
                    previous_tile = map_pointer->tile[tile_count].data;
                }
            }
        }
    }
    if (to_floor_transitions > 1) return_value = 0;
    return (return_value);
}

void map_gen_room_path (Map* map_pointer, int room_1, int room_2, bool x_then_y)
{
    bool add_room      = true;
    if ((room_1 != room_2)&&(map_pointer->room[room_1].no_of_connected_rooms <= ROOM_MAX_CONNECTIONS)&&(map_pointer->room[room_2].no_of_connected_rooms <= ROOM_MAX_CONNECTIONS))
    {
        for (int room_count = 0; room_count < ROOM_MAX_CONNECTIONS; room_count++)
        {
            if ((map_pointer->room[room_1].connected_room[room_count].connected) && (map_pointer->room[room_1].connected_room[room_count].room_no == room_2)) add_room = false;
            if ((map_pointer->room[room_2].connected_room[room_count].connected) && (map_pointer->room[room_2].connected_room[room_count].room_no == room_1)) add_room = false;
        }
        if (add_room)
        {
            bool room_1_done = false;
            bool room_2_done = false;
            for (int room_count = 0; room_count < ROOM_MAX_CONNECTIONS; room_count++)
            {
                if ((!room_1_done)&&(!map_pointer->room[room_1].connected_room[room_count].connected))
                {
                    room_1_done = true;
                    map_pointer->room[room_1].no_of_connected_rooms++;
                    map_pointer->room[room_1].connected_room[room_count].connected = true;
                    map_pointer->room[room_1].connected_room[room_count].room_no   = room_2;
                }
                if ((!room_2_done)&&(!map_pointer->room[room_2].connected_room[room_count].connected))
                {
                    room_2_done = true;
                    map_pointer->room[room_2].no_of_connected_rooms++;
                    map_pointer->room[room_2].connected_room[room_count].connected = true;
                    map_pointer->room[room_2].connected_room[room_count].room_no   = room_1;
                }
            }
            int r1_x = map_pointer->room[room_1].position.x;
            int r1_y = map_pointer->room[room_1].position.y;
            int r2_x = map_pointer->room[room_2].position.x;
            int r2_y = map_pointer->room[room_2].position.y;
            if (x_then_y)
            {
                int x_count = r1_x;
                int y_count = r1_y;
                if (r1_x > r2_x)
                {
                    for (x_count = r1_x; x_count > r2_x; x_count--)
                    {
                        int tile_count = x_count + (map_pointer->w * (y_count));
                        map_pointer->tile[tile_count].data = Tile_Type::TILE_FLOOR;
                    }
                }
                else
                {
                    for (x_count = r1_x; x_count < r2_x; x_count++)
                    {
                        int tile_count = x_count + (map_pointer->w * (y_count));
                        map_pointer->tile[tile_count].data = Tile_Type::TILE_FLOOR;
                    }
                }
                if (r1_y > r2_y)
                {
                    for (y_count = r1_y; y_count > r2_y; y_count--)
                    {
                        int tile_count = x_count + (map_pointer->w * (y_count));
                        map_pointer->tile[tile_count].data = Tile_Type::TILE_FLOOR;
                    }
                }
                else
                {
                    for (y_count = r1_y; y_count < r2_y; y_count++)
                    {
                        int tile_count = x_count + (map_pointer->w * (y_count));
                        map_pointer->tile[tile_count].data = Tile_Type::TILE_FLOOR;
                    }
                }
            }
            else
            {
                int x_count = r1_x;
                int y_count = r1_y;
                if (r1_y > r2_y)
                {
                    for (y_count = r1_y; y_count > r2_y; y_count--)
                    {
                        int tile_count = x_count + (map_pointer->w * (y_count));
                        map_pointer->tile[tile_count].data = Tile_Type::TILE_FLOOR;
                    }
                }
                else
                {
                    for (y_count = r1_y; y_count < r2_y; y_count++)
                    {
                        int tile_count = x_count + (map_pointer->w * (y_count));
                        map_pointer->tile[tile_count].data = Tile_Type::TILE_FLOOR;
                    }
                }
                if (r1_x > r2_x)
                {
                    for (x_count = r1_x; x_count > r2_x; x_count--)
                    {
                        int tile_count = x_count + (map_pointer->w * (y_count));
                        map_pointer->tile[tile_count].data = Tile_Type::TILE_FLOOR;
                    }
                }
                else
                {
                    for (x_count = r1_x; x_count < r2_x; x_count++)
                    {
                        int tile_count = x_count + (map_pointer->w * (y_count));
                        map_pointer->tile[tile_count].data = Tile_Type::TILE_FLOOR;
                    }
                }
            }
        }
    }
}

int distance_manhattan(Map* map_pointer, int tile_start, int tile_end)
{
    return (abs(map_pointer->tile[tile_start].position.x - map_pointer->tile[tile_end].position.x) + abs(map_pointer->tile[tile_start].position.y - map_pointer->tile[tile_end].position.y));
}

void map_gen_room_connect (Map* map_pointer)
{
    if (map_pointer->no_of_rooms > 0)
    {
        for (int room_count_1 = 0; room_count_1 < map_pointer->no_of_rooms; room_count_1++)
        {
            int closest_room_1 = room_count_1;
            int closest_room_2 = room_count_1;
            int distance_closest_room_2 = map_pointer->size();
            for (int room_count_2 = 0; room_count_2 < map_pointer->no_of_rooms; room_count_2++)
            {
                if (room_count_1 != room_count_2)
                {
                    int distance_temp_1 = map_gen_room_check_path(map_pointer, room_count_1, room_count_2, true );
                    int distance_temp_2 = map_gen_room_check_path(map_pointer, room_count_1, room_count_2, false);
                    int distance_temp = (distance_temp_1 < distance_temp_2) ? distance_temp_1 : distance_temp_2;
                    if  ((distance_temp > 0) && (distance_temp < distance_closest_room_2))
                    {
                        closest_room_1          = closest_room_2;
                        distance_closest_room_2 = distance_temp;
                        closest_room_2          = room_count_2;
                    }
                }
            }
            if (map_gen_room_check_path(map_pointer, room_count_1, closest_room_2, true) > (map_gen_room_check_path(map_pointer, room_count_1, closest_room_2, false))) map_gen_room_path(map_pointer, room_count_1, closest_room_2, true);
            else map_gen_room_path(map_pointer, room_count_1, closest_room_2, false);
            if (map_pointer->room[closest_room_1].no_of_connected_rooms < 1)
            {
                if (map_gen_room_check_path(map_pointer, room_count_1, closest_room_1, true) > (map_gen_room_check_path(map_pointer, room_count_1, closest_room_1, false))) map_gen_room_path(map_pointer, room_count_1, closest_room_1, true);
                else map_gen_room_path(map_pointer, room_count_1, closest_room_1, false);
            }
        }
    }
}

int map_gen_get_room_no(Map* map_pointer, int tile_no)
{
    //map_pointer->tile[tile_no].data = 1024;
    int return_value = -1;
    int tile_x = tile_no % map_pointer->w;
    int tile_y = tile_no / map_pointer->w;
    for (int i = 0; i < ROOM_MAX; i++)
    {
        if (map_pointer->room[i].active)
        {
            if    ((tile_x >= (map_pointer->room[i].position.x-(map_pointer->room[i].w/2)))
                && (tile_x <= (map_pointer->room[i].position.x+(map_pointer->room[i].w/2)))
                && (tile_y >= (map_pointer->room[i].position.y-(map_pointer->room[i].h/2)))
                && (tile_y <= (map_pointer->room[i].position.y+(map_pointer->room[i].h/2)))) return_value = i;
                // could also check that tiles on side are floor tiles, so as to prevent generating paths on room edges;
        }
    }
    //if (return_value >= 0) std::cout << "found room = " << return_value << std::endl;
    return return_value;
}

void map_gen_connect_rooms(Map* map_pointer, int room_ID_1, int room_ID_2, int direction)
{
    bool connected = false;
    for (int i = 0; i < 4; i++)
    {
        if (map_pointer->room[room_ID_1].connected_room[i].room_no == room_ID_2) connected = true;
        if (map_pointer->room[room_ID_2].connected_room[i].room_no == room_ID_1) connected = true;
    }
    if (!connected)
    {
        bool room_1_done = false;
        bool room_2_done = false;
        for (int i = 0; i < 4; i++)
        {
            if ((!room_1_done)&&(!map_pointer->room[room_ID_1].connected_room[i].connected))
            {
                room_1_done = true;
                map_pointer->room[room_ID_1].connected_room[i].connected = true;
                map_pointer->room[room_ID_1].connected_room[i].room_no   = room_ID_2;
                map_pointer->room[room_ID_1].connected_room[i].direction = direction;
            }
            if ((!room_2_done)&&(!map_pointer->room[room_ID_2].connected_room[i].connected))
            {
                room_2_done = true;
                map_pointer->room[room_ID_2].connected_room[i].connected = true;
                map_pointer->room[room_ID_2].connected_room[i].room_no   = room_ID_1;
                map_pointer->room[room_ID_2].connected_room[i].direction = direction;
            }
        }
    }
    if (!connected)
    {
        enum   {DIRECTION_UP = 0,
                DIRECTION_DOWN,
                DIRECTION_LEFT,
                DIRECTION_RIGHT};
        int start_tile   = (map_pointer->room[room_ID_1].position.y*map_pointer->w)+map_pointer->room[room_ID_1].position.x;
        int end_tile     = start_tile;
        if ((direction == DIRECTION_LEFT) || (direction == DIRECTION_RIGHT))
            end_tile     = (map_pointer->room[room_ID_1].position.y*map_pointer->w)+map_pointer->room[room_ID_2].position.x;
        else
            end_tile     = (map_pointer->room[room_ID_2].position.y*map_pointer->w)+map_pointer->room[room_ID_1].position.x;
        int current_tile = start_tile;
        if (direction == DIRECTION_UP)
        {
            while (current_tile > end_tile)
            {
                map_pointer->tile[current_tile].data = Tile_Type::TILE_FLOOR;
                current_tile -= map_pointer->w;
            }
        }
        else if (direction == DIRECTION_DOWN)
        {
            while (current_tile < end_tile)
            {
                map_pointer->tile[current_tile].data = Tile_Type::TILE_FLOOR;
                current_tile += map_pointer->w;
            }
        }
        if (direction == DIRECTION_LEFT)
        {
            while (current_tile > end_tile)
            {
                map_pointer->tile[current_tile].data = Tile_Type::TILE_FLOOR;
                current_tile --;
            }
        }
        else if (direction == DIRECTION_RIGHT)
        {
            while (current_tile < end_tile)
            {
                map_pointer->tile[current_tile].data = Tile_Type::TILE_FLOOR;
                current_tile ++;
            }
        }
    }
}

void map_gen_room_connect_2 (Map* map_pointer)
{
    map_gen_room_find(map_pointer);
    enum   {DIRECTION_UP = 0,
            DIRECTION_DOWN,
            DIRECTION_LEFT,
            DIRECTION_RIGHT};
    struct temp_rooms_data
    {
        bool found;
        int  room_ID;
        int  direction;
        int  distance_m;
    };
    struct temp_rooms
    {
        temp_rooms_data data[4];
    }temp_rooms[ROOM_MAX];
    int no_rooms_found = 0;
    for (int j = 0; j < ROOM_MAX; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            temp_rooms[j].data[i].found      = false;
            temp_rooms[j].data[i].room_ID    = 0;
            temp_rooms[j].data[i].direction  = DIRECTION_BIAS_NONE;
            temp_rooms[j].data[i].distance_m = 0;
            map_pointer->room[j].connected_room[i].connected = false;
            map_pointer->room[j].connected_room[i].room_no   = -1;
        }
        if (map_pointer->room[j].active)
        {
            for (int d = DIRECTION_UP; d <= DIRECTION_RIGHT; d++)
            {
                int  temp_tile = (map_pointer->room[j].position.y * map_pointer->w)+map_pointer->room[j].position.x;
                int  distance  = 0;
                bool done      = false;
                bool fail      = false;
                if ((!done)&&(!fail))
                {
                    while ((!done)&&(!fail))
                    {
                        if      ((temp_tile < 0)||(temp_tile > map_pointer->size()))            fail = true;
                        else if (temp_tile % map_pointer->w == 0)                          fail = true;
                        else if (temp_tile % map_pointer->w >= map_pointer->w-2) fail = true;
                        else if (map_pointer->tile[temp_tile].data == Tile_Type::TILE_WALL)                     done = true;
                        else
                        {
                            //map_pointer->tile[temp_tile].data = 1024;
                            if      (d == DIRECTION_UP)    temp_tile-=map_pointer->w;
                            else if (d == DIRECTION_DOWN)  temp_tile+=map_pointer->w;
                            else if (d == DIRECTION_LEFT)  temp_tile--;
                            else if (d == DIRECTION_RIGHT) temp_tile++;
                            else fail = true;
                            distance++;
                        }
                    }
                    done = false;
                    while ((!done)&&(!fail))
                    {
                        if      ((temp_tile < 0)||(temp_tile > map_pointer->size()))            fail = true;
                        else if (temp_tile % map_pointer->w == 0)                          fail = true;
                        else if (temp_tile % map_pointer->w >= map_pointer->w-2) fail = true;
                        else if (map_pointer->tile[temp_tile].data == Tile_Type::TILE_FLOOR)                    done = true;
                        else
                        {
                            //map_pointer->tile[temp_tile].data = 1024;
                            if      (d == DIRECTION_UP)    temp_tile-=map_pointer->w;
                            else if (d == DIRECTION_DOWN)  temp_tile+=map_pointer->w;
                            else if (d == DIRECTION_LEFT)  temp_tile--;
                            else if (d == DIRECTION_RIGHT) temp_tile++;
                            else fail = true;
                            distance++;
                        }

                    }
                    if (!fail)
                    {
                        temp_rooms[j].data[d].room_ID    = map_gen_get_room_no(map_pointer,temp_tile);
                        if (temp_rooms[j].data[d].room_ID >= 0)
                        {
                            if      (d == DIRECTION_UP)    temp_rooms[j].data[d].direction  = DIRECTION_BIAS_NORTH;
                            else if (d == DIRECTION_DOWN)  temp_rooms[j].data[d].direction  = DIRECTION_BIAS_SOUTH;
                            else if (d == DIRECTION_LEFT)  temp_rooms[j].data[d].direction  = DIRECTION_BIAS_EAST;
                            else if (d == DIRECTION_RIGHT) temp_rooms[j].data[d].direction  = DIRECTION_BIAS_WEST;
                            temp_rooms[j].data[d].distance_m = distance_manhattan(map_pointer,(map_pointer->room[j].position.y * map_pointer->w)+map_pointer->room[j].position.x,(map_pointer->room[temp_rooms[j].data[d].room_ID].position.y * map_pointer->w)+map_pointer->room[temp_rooms[j].data[d].room_ID].position.x);
                            if (temp_rooms[j].data[d].distance_m < (ROOM_MAX_X*3))
                            {
                                no_rooms_found++;
                                temp_rooms[j].data[d].found = true;
                            }
                        }
                    }
                }
            }
        }
    }
    for (int j = 0; j < ROOM_MAX; j++)
    {
        for (int d = 0; d < 4; d++)
        {
            if ((temp_rooms[j].data[d].found)&&(25 > rand() % 100)) map_gen_connect_rooms(map_pointer, j, temp_rooms[j].data[d].room_ID, d);
        }
    }
    if (!map_gen_room_flood_fill(map_pointer))
    {
    //std::cout << "Flood fill check failed, using pathfinder to locate disconnected room...." << std::endl;
        for (int j = 0; j < ROOM_MAX; j++)
        {
            for (int d = 0; d < 4; d++)
            {
                if (temp_rooms[j].data[d].found)
                {
                    if (!path_find(map_pointer,(map_pointer->room[j].position.y * map_pointer->w)+map_pointer->room[j].position.x,(map_pointer->room[temp_rooms[j].data[d].room_ID].position.y * map_pointer->w)+map_pointer->room[temp_rooms[j].data[d].room_ID].position.x))
                    {
                        map_gen_connect_rooms(map_pointer, j, temp_rooms[j].data[d].room_ID,d);
                    }
                }
            }
        }
    }
    if (!map_gen_room_flood_fill(map_pointer))
    {
        //std::cout << "Second Flood fill check failed, trying to brute force connect all rooms...." << std::endl;
        for (int j = 0; j < ROOM_MAX; j++)
        {
            for (int d = 0; d < 4; d++)
            {
                if (temp_rooms[j].data[d].found) map_gen_connect_rooms(map_pointer, j, temp_rooms[j].data[d].room_ID, d);
            }
        }
    }
}

Room map_gen_room_find_stats (Map* map_pointer, FloodFill* fill_data, int tile_number)
{
    Room return_data;
    return_data.w = 0;
    return_data.h = 0;
    int tile_count          = tile_number;
    int tile_x              = tile_number % map_pointer->w;
    int tile_y              = tile_number / map_pointer->w;
    while (fill_data[tile_count].tile_data == Tile_Type::TILE_FLOOR)
    {
        return_data.w++;
        tile_count++;
    }
    tile_count = tile_number;
    while (fill_data[tile_count].tile_data == Tile_Type::TILE_FLOOR)
    {
        return_data.h++;
        tile_count += map_pointer->w;
    }

    return_data.position.x = tile_x + (return_data.w / 2);
    return_data.position.y = tile_y + (return_data.h / 2);
    map_gen_flood_fill_tile(map_pointer,fill_data,tile_number);
    return (return_data);
}

void map_gen_room_find (Map* map_pointer)
{
    Room room_data;
    FloodFill* fill_data = new FloodFill[map_pointer->size()];
    map_pointer->no_of_rooms = 0;
    for (int tile_count = 0; tile_count < map_pointer->size(); tile_count++)
    {
        fill_data[tile_count].tile_data       = map_pointer->tile[tile_count].data;
        fill_data[tile_count].processed       = false;
        fill_data[tile_count].adjoining_tile  = false;
    }
    for (int tile_count = 0; tile_count < map_pointer->size(); tile_count++)
    {
        if ((fill_data[tile_count].tile_data == Tile_Type::TILE_FLOOR) && (!fill_data[tile_count].processed))
        {
            room_data   = map_gen_room_find_stats (map_pointer,fill_data,tile_count);
            map_pointer->room[map_pointer->no_of_rooms].active                    = true;
            map_pointer->room[map_pointer->no_of_rooms].w               = room_data.w;
            map_pointer->room[map_pointer->no_of_rooms].h               = room_data.h;
            map_pointer->room[map_pointer->no_of_rooms].position.x                = room_data.position.x;
            map_pointer->room[map_pointer->no_of_rooms].position.y                = room_data.position.y;
            map_pointer->room[map_pointer->no_of_rooms].no_of_connected_rooms     = 0;
            map_pointer->no_of_rooms++;
            //std::cout << "Found room - " << map_pointer->no_of_rooms << " x - " << map_pointer->room[map_pointer->no_of_rooms].position.x <<" y - " << map_pointer->room[map_pointer->no_of_rooms].position.y << std::endl;
        }
    }
    delete[] fill_data;
}

void map_gen_redundant_tile_check (Map* map_pointer)
{
    for (int i = 0; i < map_pointer->size(); i++) if (map_pointer->tile[i].data == Tile_Type::TILE_WALL) map_pointer->tile[i].data = Tile_Type::TILE_NONE;
    for (int i = 0; i < map_pointer->size(); i++) if (map_pointer->tile[i].data == Tile_Type::TILE_FLOOR)
    {

        // add ((temp_tile >= 0)&&(temp_tile < map_pointer->size())&&

        if (map_pointer->tile[i-1].data == Tile_Type::TILE_NONE) map_pointer->tile[i-1].data = Tile_Type::TILE_WALL;
        if (map_pointer->tile[i+1].data == Tile_Type::TILE_NONE) map_pointer->tile[i+1].data = Tile_Type::TILE_WALL;
        if (map_pointer->tile[i+map_pointer->w-1].data == Tile_Type::TILE_NONE) map_pointer->tile[i+map_pointer->w-1].data = Tile_Type::TILE_WALL;
        if (map_pointer->tile[i+map_pointer->w+1].data == Tile_Type::TILE_NONE) map_pointer->tile[i+map_pointer->w+1].data = Tile_Type::TILE_WALL;
        if (map_pointer->tile[i+map_pointer->w].data == Tile_Type::TILE_NONE)   map_pointer->tile[i+map_pointer->w].data   = Tile_Type::TILE_WALL;
        if (map_pointer->tile[i-map_pointer->w-1].data == Tile_Type::TILE_NONE) map_pointer->tile[i-map_pointer->w-1].data = Tile_Type::TILE_WALL;
        if (map_pointer->tile[i-map_pointer->w+1].data == Tile_Type::TILE_NONE) map_pointer->tile[i-map_pointer->w+1].data = Tile_Type::TILE_WALL;
        if (map_pointer->tile[i-map_pointer->w].data == Tile_Type::TILE_NONE)   map_pointer->tile[i-map_pointer->w].data   = Tile_Type::TILE_WALL;
    }
}

void map_gen_map_exits (Map* map_pointer)
{
    struct
    {
        int d;
    } rd[ROOM_MAX][ROOM_MAX];
    path_type path_pointer;
    for (int i = 0; i < ROOM_MAX; i++)
    {
        for (int j = 0; j < ROOM_MAX; j++)
        {
            if ((i != j)&&(i<j)&&(map_pointer->room[i].active)&&(map_pointer->room[j].active))
            {
                std::cout << "checking path from room " << i << " to room " << j << std::endl;
                int t1 = (map_pointer->room[i].position.y*map_pointer->w)+map_pointer->room[i].position.x;
                int t2 = (map_pointer->room[j].position.y*map_pointer->w)+map_pointer->room[j].position.x;
                if (path_find(map_pointer,&path_pointer,t1,t2))
                {
                    rd[i][j].d = path_pointer.length;
                }
            }
        }
    }
    int fr1 = 0;
    int fr2 = 0;
    int td  = 0;
    for (int i = 0; i < ROOM_MAX; i++)
    {
        if (map_pointer->room[i].active)
        {
            for (int j = 0; j < ROOM_MAX; j++)
            {
                if ((i != j)&&(i<j)&&(map_pointer->room[j].active))
                {
                    if (rd[i][j].d > td)
                    {
                        td  = rd[i][j].d;
                        fr1 = (map_pointer->room[i].position.y*map_pointer->w)+map_pointer->room[i].position.x;
                        fr2 = (map_pointer->room[j].position.y*map_pointer->w)+map_pointer->room[j].position.x;
                    }
                }
            }
        }
    }
    std::cout << "Stairs generated on tile " << fr1 << " and tile " << fr2 << std::endl;
    map_pointer->tile[fr1].data = Tile_Type::TILE_EXIT;
    map_pointer->tile[fr2].data = Tile_Type::TILE_EXIT;
}

void map_gen_room_doors (Map* map_pointer)
{
    (void)map_pointer;
}
