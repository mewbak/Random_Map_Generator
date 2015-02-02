
#include "MapGenerator_D2.h"
#include "MapHelper.h"

bool  RC_circle_collision(float x1, float y1, float r1, float x2, float y2, float r2)
{
    if (((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)) < ((r1+r2)*(r1+r2))) return(true);
    else return(false);
}

void map_gen_RC_internal (Map* map_pointer)
{
    const int number_of_circles = ROOM_MAX;
    Room room_node[number_of_circles];
    for (int node_count = 0; node_count < number_of_circles; node_count++)
    {
        int map_boundry = 4;
        room_node[node_count].active      = true;
        room_node[node_count].w = ROOM_MIN_X + rand() % (ROOM_MAX_X-ROOM_MIN_X);
        room_node[node_count].h = ROOM_MIN_Y + rand() % (ROOM_MAX_Y-ROOM_MIN_X);
        room_node[node_count].position.x  = map_boundry + (room_node[node_count].w/2) + rand() % (map_pointer->w - room_node[node_count].w - (map_boundry*2));
        room_node[node_count].position.y  = map_boundry + (room_node[node_count].h/2) + rand() % (map_pointer->h - room_node[node_count].h - (map_boundry*2));
    }
    for (int node_count_1 = 0; node_count_1 < number_of_circles; node_count_1++)
    {
        if (room_node[node_count_1].active)
        {
            for (int node_count_2 = 0; node_count_2 < number_of_circles; node_count_2++)
            {
                if ((room_node[node_count_2].active) && (node_count_1 != node_count_2))
                {
                    if (RC_circle_collision(room_node[node_count_1].position.x,room_node[node_count_1].position.y,ROOM_MAX_R,room_node[node_count_2].position.x,room_node[node_count_2].position.y,ROOM_MAX_R))
                    {
                        room_node[node_count_2].active = false;
                    }
                }
            }
        }
    }
    for (int node_count = 0; node_count < number_of_circles; node_count++)
    {
        if (room_node[node_count].active)
        {
            map_gen_room(map_pointer,room_node[node_count]);
        }
    }
    map_gen_room_connect(map_pointer);
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
    //map_check(map_pointer,intermediate_layer);
}
