
#include "MapGenerator_D2.h"
#include "MapHelper.h"

void map_gen_RC_internal (Map* map_pointer)
{
    maprow *current_layer = new maprow[map_pointer->w];
    map_pointer->layers.push_back(current_layer);
    map_pointer->layernames.push_back("intermediate");
    for (int j = 0; j < map_pointer->h; j++)
    {
        for (int i = 0; i < map_pointer->w; i++)
            (current_layer)[i][j] = Tile_Type::TILE_WALL;
    }
    struct room_struct
    {
        bool valid;
        int x;
        int y;
        int w;
        int h;
    };
    int max_r = (int)sqrt((ROOM_MAX_X*ROOM_MAX_X)+(ROOM_MAX_Y*ROOM_MAX_Y));
    int max_rooms = map_pointer->size() / (ROOM_MIN_X*ROOM_MIN_Y);
    room_struct room[max_rooms];
    for (int i = 0; i < max_rooms; i++)
    {
        room[i].valid = true;
        room[i].w = ROOM_MIN_X + rand() % (ROOM_MAX_X-ROOM_MIN_X);
        room[i].h = ROOM_MIN_Y + rand() % (ROOM_MAX_Y-ROOM_MIN_Y);
        room[i].x = (room[i].w/2) + rand() % (int)(map_pointer->w - room[i].w)-1;
        room[i].y = (room[i].h/2) + rand() % (int)(map_pointer->h - room[i].h)-1;
    }
    for (int i = 0; i < max_rooms; i++)
    {
        for (int j = 0; j < max_rooms; j++)
        {
            if ((room[i].valid && room[j].valid)&&(i!=j))
            {
                if (max_r > (int)sqrt(((room[i].x-room[j].x)*(room[i].x-room[j].x))+((room[i].y-room[j].y)*(room[i].y-room[j].y))))
                    room[j].valid = false;
            }
        }
    }
    for (int i = 0; i < max_rooms; i++)
    {
        if (room[i].valid)
        {
            for (int j = 1; j < room[i].w; j++)
            {
                for (int k = 1; k < room[i].h; k++)
                    (current_layer)[room[i].x-(room[i].w/2)+j][room[i].y-(room[i].h/2)+k] = Tile_Type::TILE_FLOOR;
            }
        }
    }
    for (int i = 0; i < max_rooms; i++)
    {
        if (room[i].valid)
        {
            for (int j = 0; j < max_rooms; j++)
            {
                if ((room[j].valid)&&(i != j))
                {
                    bool path_found = false;
                    if (!path_found)
                    {
                        int previous_tile = Tile_Type::TILE_FLOOR;
                        int transitions = 0;
                        int current_x = room[i].x;
                        int current_y = room[i].y;
                        for (current_x = room[i].x; current_x != room[j].x;)
                        {
                            if (previous_tile != (current_layer)[current_x][current_y])
                                transitions++;
                            if ((current_layer)[current_x][current_y] == Tile_Type::TILE_PATH)
                                transitions++;
                            previous_tile = (current_layer)[current_x][current_y];
                            if (room[i].x < room[j].x) current_x++;
                                    else current_x--;
                        }
                        for (current_y = room[i].y; current_y != room[j].y;)
                        {
                            if (previous_tile != (current_layer)[current_x][current_y])
                                transitions++;
                            if ((current_layer)[current_x][current_y] == Tile_Type::TILE_PATH)
                                transitions++;
                            previous_tile = (current_layer)[current_x][current_y];
                            if (room[i].y < room[j].y) current_y++;
                                    else current_y--;
                        }
                        if ((!path_found)&&(transitions == 2))
                        {
                            path_found = true;
                            current_x = room[i].x;
                            current_y = room[i].y;
                            for (current_x = room[i].x; current_x != room[j].x;)
                            {
                                (current_layer)[current_x][current_y] = Tile_Type::TILE_PATH;
                                if (room[i].x < room[j].x) current_x++;
                                        else current_x--;
                            }
                            for (current_y = room[i].y; current_y != room[j].y;)
                            {
                                (current_layer)[current_x][current_y] = Tile_Type::TILE_PATH;
                                if (room[i].y < room[j].y) current_y++;
                                        else current_y--;
                            }
                        }
                    }
                    if (!path_found)
                    {
                        int previous_tile = Tile_Type::TILE_FLOOR;
                        int transitions = 0;
                        int current_x = room[i].x;
                        int current_y = room[i].y;
                        for (current_y = room[i].y; current_y != room[j].y;)
                        {
                            if (previous_tile != (current_layer)[current_x][current_y])
                                transitions++;
                            if ((current_layer)[current_x][current_y] == Tile_Type::TILE_PATH)
                                transitions++;
                            previous_tile = (current_layer)[current_x][current_y];
                            if (room[i].y < room[j].y) current_y++;
                                    else current_y--;
                        }
                        for (current_x = room[i].x; current_x != room[j].x;)
                        {
                            if (previous_tile != (current_layer)[current_x][current_y])
                                transitions++;
                            if ((current_layer)[current_x][current_y] == Tile_Type::TILE_PATH)
                                transitions++;
                            previous_tile = (current_layer)[current_x][current_y];
                            if (room[i].x < room[j].x) current_x++;
                                    else current_x--;
                        }
                        if ((!path_found)&&(transitions == 2))
                        {
                            path_found = true;
                            current_x = room[i].x;
                            current_y = room[i].y;
                            for (current_y = room[i].y; current_y != room[j].y;)
                            {
                                (current_layer)[current_x][current_y] = Tile_Type::TILE_PATH;
                                if (room[i].y < room[j].y) current_y++;
                                        else current_y--;
                            }
                            for (current_x = room[i].x; current_x != room[j].x;)
                            {
                                (current_layer)[current_x][current_y] = Tile_Type::TILE_PATH;
                                if (room[i].x < room[j].x) current_x++;
                                        else current_x--;
                            }
                        }
                    }
                }
            }
        }
    }
    for (int j = 0; j < map_pointer->h; j++)
    {
        for (int i = 0; i < map_pointer->w; i++)
            if ((current_layer)[i][j] == Tile_Type::TILE_PATH)
                (current_layer)[i][j] = Tile_Type::TILE_FLOOR;
    }
}

void MapGenerator_D2::Generate (Map* map_pointer, MapProperties properties)
{
    Prepare(map_pointer, properties);
    GenerateMap(map_pointer);
    applyTileset(map_pointer, properties.tile_set);
}

void MapGenerator_D2::GenerateMap(Map* map_pointer)
{
    for (int i = 0; i < map_pointer->size(); i++)
    {
        map_pointer->tile[i].data      = Tile_Type::TILE_WALL;
        map_pointer->tile[i].attribute = TILE_ATTRIBUTE_NONE;
    }
    map_gen_RC_internal(map_pointer);
    if (!map_gen_room_flood_fill(map_pointer)) map_gen_RC_internal(map_pointer);
}
