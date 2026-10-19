#include "map.h"
#include "helpers.h"
#include "room.h"

const int MAX_DEPTH    = 4;   // up to 2^4 = 16 leaf partitions
const int MIN_LEAF_W   = 12;  // a partition narrower than this won't split vertically
const int MIN_LEAF_H   = 10;   // a partition shorter than this won't split horizontally
const int ROOM_PADDING = 2;   // empty border kept inside each partition
const int ROOM_MIN     = 4;   // smallest room edge length


static void split(BSPNode *node, int depth, Map *map){
    if(depth >= MAX_DEPTH) return;

    // TraceLog(LOG_INFO,"depth:%i",depth);

    bool can_split_h = (node->h >= MIN_LEAF_H * 2);
    bool can_split_v = (node->w >= MIN_LEAF_W * 2);

    if(!can_split_h && !can_split_v) return;

    bool horizontal;
    if(!can_split_v) horizontal = true;
    else if(!can_split_h) horizontal = false;
    else horizontal = (node->h > node->w) || (node->h == node->w && GetRandomValue(0,1));

    node->node_left = map->nodes_count++;
    node->node_right = map->nodes_count++;

    if(horizontal){
        int split_y = GetRandomValue(node->y + MIN_LEAF_H, node->y + node->h - MIN_LEAF_H);

        BSPNode *node_left = &map->nodes[node->node_left];
        node_left->x = node->x;     node_left->y = node->y;
        node_left->w = node->w;     node_left->h = split_y - node->y;

        BSPNode *node_right = &map->nodes[node->node_right];
        node_right->x = node->x;    node_right->y = split_y;
        node_right->w = node->w;    node_right->h = ( node->y + node->h ) - split_y;
    }else{
        int split_x = GetRandomValue(node->x + MIN_LEAF_W, node->x + node->w - MIN_LEAF_W);

        BSPNode *node_left = &map->nodes[node->node_left];
        node_left->x = node->x;             node_left->y = node->y;
        node_left->w = split_x - node->x;   node_left->h = node->h;

        BSPNode *node_right = &map->nodes[node->node_right];
        node_right->x = split_x;    node_right->y = node->y;
        node_right->w = ( node->x+ node->w ) - split_x; node_right->h = node->h;
    }

    split(&map->nodes[node->node_left],depth +1,map);
    split(&map->nodes[node->node_right],depth +1,map);
}


bool map_is_inside(const Map *self, int x, int y){
    return (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT);
}

static void carve_tile(Map *self, int x, int y, TileType tile_type){
    if(x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT){
        // self->tiles[x][y].type = tile_type;
        self->tiles[y][x].type = tile_type;
    }
}

static void carve_h_line(Map* self, int x1, int x2, int y)
{
    if (x1 > x2) {
        // std::swap(x1, x2);
        int aux = x1;
        x1 = x2;
        x2 = aux;
    }

    for (int x = x1; x <= x2; ++x)
        carve_tile(self,x, y, TILE_TYPE_FLOOR);
}

static void carve_v_line(Map* self, int y1, int y2, int x)
{
    if (y1 > y2){
        // std::swap(y1, y2);
        int aux = y1;
        y1 = y2;
        y2 = aux;
    }

    for (int y = y1; y <= y2; ++y)
        carve_tile(self,x, y, TILE_TYPE_FLOOR);
}



static void build_rooms(Map *self, BSPNode *node){
    if(node->node_left != 0){
        build_rooms(self,&self->nodes[node->node_left]);
        build_rooms(self,&self->nodes[node->node_right]);
        return;
    }

    int max_w = node->w - ROOM_PADDING * 2;
    int max_h = node->h - ROOM_PADDING * 2;
    if(max_w < ROOM_MIN || max_h < ROOM_MIN) return;

    int rw = GetRandomValue(ROOM_MIN, max_w);
    int rh = GetRandomValue(ROOM_MIN, max_h);
    int rx = node->x + ROOM_PADDING + GetRandomValue(0, max_w - rw);
    int ry = node->y + ROOM_PADDING + GetRandomValue(0, max_h - rh);

    node->room = (Room){rx,ry,rw,rh};
    node->has_room = true;


    for (int y = ry; y < ry + rh; ++y){
        for (int x = rx; x < rx + rw; ++x){
            carve_tile(self,x,y,TILE_TYPE_FLOOR);
        }
    }

    self->rooms[self->rooms_count++] = node->room;

}

static void connect_rooms(Map *self, BSPNode *node){

    if(node->node_left == 0) return;

    connect_rooms(self,&self->nodes[node->node_left]);
    connect_rooms(self,&self->nodes[node->node_right]);

    BSPNode *l = &self->nodes[node->node_left];
    while(l->node_left != 0){
        l = coin_flip() ? &self->nodes[l->node_left] : &self->nodes[l->node_right];
    }

    BSPNode *r = &self->nodes[node->node_right];
    while(r->node_left != 0){
        r = coin_flip() ? &self->nodes[r->node_left] : &self->nodes[r->node_right];
    }

    if(!l->has_room || !r->has_room) return;

    int x1 = centre_x(&l->room); int y1 = centre_y(&l->room);
    int x2 = centre_x(&r->room); int y2 = centre_y(&r->room);

    if(coin_flip()){
        carve_h_line(self,x1,x2,y1);
        carve_v_line(self,y1,y2,x2);
    }else{
        carve_v_line(self,y1,y2,x1);
        carve_h_line(self,x1,x2,y2);
    }
}

// static void connect_rooms(Map *self, BSPNode *node){

//     if(node->node_left == 0) return;

//     connect_rooms(self,&self->nodes[node->node_left]);
//     connect_rooms(self,&self->nodes[node->node_right]);

//     BSPNode l = self->nodes[node->node_left];
//     while(l.node_left != 0){
//         l = coin_flip() ? self->nodes[l.node_left] : self->nodes[l.node_right];
//     }

//     BSPNode r = self->nodes[node->node_right];
//     while(r.node_left != 0){
//         r = coin_flip() ? self->nodes[r.node_left] : self->nodes[r.node_right];
//     }

//     if(!l.has_room || !r.has_room) return;

//     int x1 = centre_x(&l.room); int y1 = centre_y(&l.room);
//     int x2 = centre_x(&r.room); int y2 = centre_y(&r.room);

//     if(coin_flip()){
//         carve_h_line(self,x1,x2,y1);
//         carve_v_line(self,y1,y2,x2);
//     }else{
//         carve_v_line(self,y1,y2,x1);
//         carve_h_line(self,x1,x2,y2);
//     }
// }


void map_init(Map *self)
{

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            self->tiles[y][x] = (Tile){0};
            self->dist[y][x] = UNREACHABLE;
        }
    }

    self->rooms_count = 0;

    BSPNode root;
    root.x = 0;
    root.y = 0;
    root.w = MAP_WIDTH;
    root.h = MAP_HEIGHT;

    self->nodes_count = 1;

    memset(self->nodes,0,sizeof(self->nodes));
    memset(self->rooms,0,sizeof(self->rooms));

    // generate, TODO: separate aftwards
    split(&root,0,self);

    build_rooms(self,&root);
    // int aa = 1;

    connect_rooms(self,&root);
}


bool map_is_visible_edge(const Map *self, int x, int y){
    // has not visible neigbour

    // if(!self->tiles[y][x].visible) return false;

    bool has_not_visible = false;
    for (int i = 0; i < 4; ++i) {
        int nx = x + neighbours_4[i].x;
        int ny = y + neighbours_4[i].y;

        // if(!map_is_inside(self,nx,ny))continue;

        if(!self->tiles[ny][nx].visible){
            has_not_visible = true;
            // return true;
        }
    }

    return has_not_visible;
}
void map_compute_distance_from_goal(Map *self, int goal_x, int goal_y)
{
    Vector2i queue[MAP_WIDTH * MAP_HEIGHT];
    int counter = 0;
    for (int y = 0; y < MAP_HEIGHT; ++y){
        for (int x = 0; x < MAP_WIDTH; ++x){
            self->dist[y][x] = UNREACHABLE;
            queue[counter++] = (Vector2i){0,0};
        }
    }
    // if (self->tiles[goal_y][goal_x].type != TILE_TYPE_FLOOR) return;

    int head = 0;
    int tail = 0;

    self->dist[goal_y][goal_x] = 0;
    queue[tail++] = (Vector2i){goal_x, goal_y};

    // BFS search
    while(head < tail){
        Vector2i current = queue[head++];

        for (int i = 0; i < 4; ++i) {
            int nx = current.x + directions[i][0];
            int ny = current.y + directions[i][1];

            // inside the map?
            if(!map_is_inside(self,nx,ny)) continue;

            // if walls?
            if(self->tiles[ny][nx].type == TILE_TYPE_WALL) continue;

            // already visited
            if(self->dist[ny][nx] != UNREACHABLE) continue;


            // self->map_navigation[MAP_NAVIGATION_COST][index_nn] =
            int current_cost = self->dist[current.y][current.x];
            self->dist[ny][nx] = current_cost+1;

            queue[tail++] = (Vector2i){nx, ny};
        }
    }
}

void map_print(Map *self)
{
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (self->dist[y][x] == UNREACHABLE) {
                printf("%4s", "x");
            } else {
                printf("%4d", self->dist[y][x]);
            }
        }
        printf("\n");
    }
}
