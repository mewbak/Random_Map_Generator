
#include "map_gen_M1.h"

void map_gen_M1_init (Map* map_pointer)
{
    for (int i = 0; i < map_pointer->size(); i++)
    {
        map_pointer->tile[i].data      = TILE_WALL;
        map_pointer->tile[i].attribute = TILE_ATTRIBUTE_NONE;
    }
}

void map_gen_M1(Map* map_pointer, int seed)
{
    srand(seed);
    map_gen_M1(map_pointer);
}

bool map_gen_maze_check_tile(Map* map_pointer, int tile_count, int direction_bias)
{
    bool return_value = true;
    if (tile_count < map_pointer->w) return_value = false;
    if (tile_count > (map_pointer->size() - map_pointer->w-1)) return_value = false;
    if (!(tile_count % map_pointer->w)) return_value = false;
    if (!((tile_count+1) % map_pointer->w)) return_value = false;
    if (map_pointer->tile[tile_count].attribute == TILE_ATTRIBUTE_PROCESSED) return_value = false;
    if (return_value)
    {
        switch (direction_bias)
        {
            case DIRECTION_BIAS_NORTH:
                for (int wall_count = 1; wall_count <= WALL_WIDTH; wall_count++)
                {
                    if (((tile_count-wall_count) < map_pointer->size()) && ((tile_count-wall_count) > 0) && (map_pointer->tile[tile_count-wall_count].data != TILE_WALL)) return_value = false;
                    if (((tile_count+wall_count) < map_pointer->size()) && ((tile_count+wall_count) > 0) && (map_pointer->tile[tile_count+wall_count].data != TILE_WALL)) return_value = false;
                    if (((tile_count-map_pointer->w*wall_count) < map_pointer->size()) && ((tile_count-map_pointer->w*wall_count) > 0) && (map_pointer->tile[tile_count-map_pointer->w*wall_count].data != TILE_WALL)) return_value = false;
                    if (((tile_count-map_pointer->w-wall_count) < map_pointer->size()) && ((tile_count-map_pointer->w-wall_count) > 0) && (map_pointer->tile[tile_count-map_pointer->w-wall_count].data != TILE_WALL)) return_value = false;
                    if (((tile_count-map_pointer->w+wall_count) < map_pointer->size()) && ((tile_count-map_pointer->w+wall_count) > 0) && (map_pointer->tile[tile_count-map_pointer->w+wall_count].data != TILE_WALL)) return_value = false;
                }
            break;
            case DIRECTION_BIAS_SOUTH:
                for (int wall_count = 1; wall_count <= WALL_WIDTH; wall_count++)
                {
                    if (((tile_count-wall_count) < map_pointer->size()) && ((tile_count-wall_count) > 0) && (map_pointer->tile[tile_count-wall_count].data != TILE_WALL)) return_value = false;
                    if (((tile_count+wall_count) < map_pointer->size()) && ((tile_count+wall_count) > 0) && (map_pointer->tile[tile_count+wall_count].data != TILE_WALL)) return_value = false;
                    if (((tile_count+map_pointer->w*wall_count) < map_pointer->size()) && ((tile_count+map_pointer->w*wall_count) > 0) && (map_pointer->tile[tile_count+map_pointer->w*wall_count].data != TILE_WALL)) return_value = false;
                    if (((tile_count+map_pointer->w-wall_count) < map_pointer->size()) && ((tile_count+map_pointer->w-wall_count) > 0) && (map_pointer->tile[tile_count+map_pointer->w-wall_count].data != TILE_WALL)) return_value = false;
                    if (((tile_count+map_pointer->w+wall_count) < map_pointer->size()) && ((tile_count+map_pointer->w+wall_count) > 0) && (map_pointer->tile[tile_count+map_pointer->w+wall_count].data != TILE_WALL)) return_value = false;
                }
            break;
            case DIRECTION_BIAS_EAST:
                for (int wall_count = 1; wall_count <= WALL_WIDTH; wall_count++)
                {
                    if (((tile_count-wall_count) < map_pointer->size()) && ((tile_count-wall_count) > 0) && (map_pointer->tile[tile_count-wall_count].data != TILE_WALL)) return_value = false;
                    if (((tile_count-map_pointer->w*wall_count) < map_pointer->size()) && ((tile_count-map_pointer->w*wall_count) > 0) && (map_pointer->tile[tile_count-map_pointer->w*wall_count].data != TILE_WALL)) return_value = false;
                    if (((tile_count-map_pointer->w-wall_count) < map_pointer->size()) && ((tile_count-map_pointer->w-wall_count) > 0) && (map_pointer->tile[tile_count-map_pointer->w-wall_count].data != TILE_WALL)) return_value = false;
                    if (((tile_count+map_pointer->w*wall_count) < map_pointer->size()) && ((tile_count+map_pointer->w*wall_count) > 0) && (map_pointer->tile[tile_count+map_pointer->w*wall_count].data != TILE_WALL)) return_value = false;
                    if (((tile_count+map_pointer->w-wall_count) < map_pointer->size()) && ((tile_count+map_pointer->w-wall_count) > 0) && (map_pointer->tile[tile_count+map_pointer->w-wall_count].data != TILE_WALL)) return_value = false;
                }
            break;
            case DIRECTION_BIAS_WEST:
                for (int wall_count = 1; wall_count <= WALL_WIDTH; wall_count++)
                {
                    if (((tile_count+wall_count) < map_pointer->size()) && ((tile_count+wall_count) > 0) && (map_pointer->tile[tile_count+wall_count].data != TILE_WALL)) return_value = false;
                    if (((tile_count-map_pointer->w*wall_count) < map_pointer->size()) && ((tile_count-map_pointer->w*wall_count) > 0) && (map_pointer->tile[tile_count-map_pointer->w*wall_count].data != TILE_WALL)) return_value = false;
                    if (((tile_count-map_pointer->w+wall_count) < map_pointer->size()) && ((tile_count-map_pointer->w+wall_count) > 0) && (map_pointer->tile[tile_count-map_pointer->w+wall_count].data != TILE_WALL)) return_value = false;
                    if (((tile_count+map_pointer->w*wall_count) < map_pointer->size()) && ((tile_count+map_pointer->w*wall_count) > 0) && (map_pointer->tile[tile_count+map_pointer->w*wall_count].data != TILE_WALL)) return_value = false;
                    if (((tile_count+map_pointer->w+wall_count) < map_pointer->size()) && ((tile_count+map_pointer->w+wall_count) > 0) && (map_pointer->tile[tile_count+map_pointer->w+wall_count].data != TILE_WALL)) return_value = false;
                }
            break;
            case DIRECTION_BIAS_NONE:
            default:
                // throw error
                return_value = true;
            break;
        }
    }
    return (return_value);
};

void map_gen_maze(Map* map_pointer, int tile_count, int direction_bias)
{
    map_pointer->tile[tile_count].data      = TILE_FLOOR;
    map_pointer->tile[tile_count].attribute = TILE_ATTRIBUTE_PROCESSED;
    int  tiles_available    = 1;
    bool new_direction_bias = false;
    int tile_direction      = 0;
    while (tiles_available > 0)
    {
        tiles_available = 0;
        int  tile_north      = tile_count-map_pointer->w;
        bool tile_north_ok   = map_gen_maze_check_tile(map_pointer,tile_north,DIRECTION_BIAS_NORTH);
        if (tile_north_ok) tiles_available++;
        int  tile_south      = tile_count+map_pointer->w;
        bool tile_south_ok   = map_gen_maze_check_tile(map_pointer,tile_south,DIRECTION_BIAS_SOUTH);
        if (tile_south_ok) tiles_available++;
        int  tile_east       = tile_count-1;
        bool tile_east_ok    = map_gen_maze_check_tile(map_pointer,tile_east,DIRECTION_BIAS_EAST);
        if (tile_east_ok) tiles_available++;
        int  tile_west       = tile_count+1;
        bool tile_west_ok    = map_gen_maze_check_tile(map_pointer,tile_west,DIRECTION_BIAS_WEST);
        if (tile_west_ok) tiles_available++;
        if ((new_direction_bias) || (direction_bias == DIRECTION_BIAS_NONE)) tile_direction = (rand() % 4) + 1;
        else
        {
            tile_direction = (rand()%(4+DIRECTION_BIAS_THRESHOLD))+1;
            if (tile_direction > 4) tile_direction = direction_bias;
            else
            {
                direction_bias = tile_direction;
                new_direction_bias = true;
            }
        }
        switch (tile_direction)
        {
            case DIRECTION_BIAS_NORTH:
                if (tile_north_ok)
                {
                    tiles_available--;
                    tile_north_ok = false;
                    map_gen_maze(map_pointer,tile_north,direction_bias);
                }
            break;
            case DIRECTION_BIAS_SOUTH:
                if (tile_south_ok)
                {
                    tiles_available--;
                    tile_south_ok = false;
                    map_gen_maze(map_pointer,tile_south,direction_bias);
                }
            break;
            case DIRECTION_BIAS_EAST:
                if (tile_east_ok)
                {
                    tiles_available--;
                    tile_east_ok = false;
                    map_gen_maze(map_pointer,tile_east,direction_bias);
                }
            break;
            case DIRECTION_BIAS_WEST:
                if (tile_west_ok)
                {
                    tiles_available--;
                    tile_west_ok = false;
                    map_gen_maze(map_pointer,tile_west,direction_bias);
                }
            break;
            default:
                // throw error
                tiles_available--;
            break;
        }
    }
}

void map_gen_maze(Map* map_pointer, int tile_x, int tile_y, int direction_bias)
{
    map_gen_maze(map_pointer, ((tile_y * map_pointer->w) + tile_x),direction_bias);
}

void map_gen_M1 (Map* map_pointer)
{
    map_gen_M1_init (map_pointer);
    Room room_data;
    room_data.position.x  = 0;
    room_data.position.y  = 0;
    room_data.w = 0;
    room_data.h = 0;
    map_gen_room(map_pointer,room_data);
    map_gen_room(map_pointer,room_data);
    map_gen_room(map_pointer,room_data);
    map_gen_room(map_pointer,room_data);
    map_gen_maze(map_pointer,map_pointer->w/2,map_pointer->h/2,DIRECTION_BIAS_NORTH);
    map_gen_room_exits(map_pointer);
    //map_gen_maze_check(map_pointer);
}
