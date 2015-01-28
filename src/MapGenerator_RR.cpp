
#include "map_gen_RR.h"

void map_gen_RR (Map* map_pointer)
{
    (void)map_pointer;
}

void map_gen_RR (Map* map_pointer, int seed)
{
    srand(seed);
    map_gen_RR(map_pointer);
}


/*
In this algorithm a "feature" is taken to mean any kind of map component e.g. large room, small room, corridor, circular arena, vault etc.
Fill the whole map with solid earth
Dig out a single room in the centre of the map
Pick a wall of any room
Decide upon a new feature to build
See if there is room to add the new feature through the chosen wall
If yes, continue. If no, go back to step 3
Add the feature through the chosen wall
Go back to step 3, until the dungeon is complete
Add the up and down staircases at random points in map
Finally, sprinkle some monsters and items liberally over dungeon
*/
