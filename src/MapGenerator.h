

struct v2i
{
    int x;
    int y;
};

struct map_type
{
    v2i dimension;
    int no_of_tiles;
    int* layer_background;
    int* layer_object;
    int* layer_collision;
};
