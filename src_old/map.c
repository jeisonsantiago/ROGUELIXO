#include "map.h"
#include "helpers.h"
#include "room.h"

const int MAX_DEPTH    = 4;   // up to 2^4 = 16 leaf partitions
const int MIN_LEAF_W   = 12;  // a partition narrower than this won't split vertically
const int MIN_LEAF_H   = 10;   // a partition shorter than this won't split horizontally
const int ROOM_PADDING = 2;   // empty border kept inside each partition
const int ROOM_MIN     = 4;   // smallest room edge length

// const int MAX_DEPTH    = 1;   // up to 2^4 = 16 leaf partitions
// const int MIN_LEAF_W   = 4;  // a partition narrower than this won't split vertically
// const int MIN_LEAF_H   = 2;   // a partition shorter than this won't split horizontally
// const int ROOM_PADDING = 2;   // empty border kept inside each partition
// const int ROOM_MIN     = 2;   // smallest room edge length


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
    self->can_respawn_point_count = 0;

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

void map_init_cave(Map *self)
{
    // for (int y = 0; y < MAP_HEIGHT; ++y) {
    //     for (int x = 0; x < MAP_WIDTH; ++x) {
    //         self->tiles[y][x] = (Tile){0};
    //         self->dist[y][x] = UNREACHABLE;
    //     }
    // }

    // self->rooms_count = 0;

    // set random noise
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {

            bool border = (x == 0 || y == 0 || (x == MAP_WIDTH-1) || (y == MAP_HEIGHT -1));

            self->tiles[y][x].type = (border || (GetRandomValue(1,100) < 45))?TILE_TYPE_WALL : TILE_TYPE_FLOOR;

            // self->tiles[y][y].type = TILE_TYPE_FLOOR;
        }
    }

    // smooth with the 406 rule, fise passes
    for (int iter = 0; iter < 2; ++iter)
    {
        TileType next[MAP_HEIGHT][MAP_WIDTH];
        for (int y = 0; y < MAP_HEIGHT; ++y){
            for (int x = 0; x < MAP_WIDTH; ++x)
            {
                bool border = (x == 0 || y == 0 || x == MAP_WIDTH -1 || y == MAP_HEIGHT -1);
                if (border) { next[y][x] = TILE_TYPE_WALL; continue; }
                int walls = 0;
                for (int dy = -1; dy <= 1; ++dy)
                    for (int dx = -1; dx <= 1; ++dx)
                        if (!(dx == 0 && dy == 0) && self->tiles[y+dy][x+dx].type == TILE_TYPE_WALL)
                            ++walls;
                next[y][x] = (walls >= 5) ? TILE_TYPE_WALL : TILE_TYPE_FLOOR;
            }
        }

        for (int y = 0; y < MAP_HEIGHT; ++y)
            for (int x = 0; x < MAP_WIDTH; ++x)
                self->tiles[y][x].type = next[y][x];
    }
}

void map_init_village(Map *self){
    // set random noise
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {

            bool border = (x == 0 || y == 0 || (x == MAP_WIDTH-1) || (y == MAP_HEIGHT -1));

            // self->tiles[y][x].type = (border || (GetRandomValue(1,100) < 45))?TILE_TYPE_WALL : TILE_TYPE_FLOOR;

            self->tiles[y][x].type = (border)?TILE_TYPE_WALL : TILE_TYPE_FLOOR;
            self->tiles[y][x].visual_decoration_index = -1;

            if(border && y != MAP_HEIGHT-1){
                self->tiles[y][x].visual_decoration_index = 261;
            }

            self->tiles[y][x].visual_index = 88;
            if(y == MAP_HEIGHT-1){
                self->tiles[y][x].visual_index = 95;
            }
        }
    }

    // draw road
    self->tiles[5][3].visual_index = 256+8;
    for (int i = 3; i < 18; ++i) {
        self->tiles[6][i].visual_index = 256+8;
    }
    self->tiles[5][17].visual_index = 256+8;

    // draw house
    self->tiles[4][15].visual_index = 188;
    self->tiles[4][16].visual_index = 188;
    self->tiles[4][17].visual_index = 273;
    self->tiles[4][18].visual_index = 188; // front
    self->tiles[3][15].visual_index = 275; // roof
    self->tiles[3][16].visual_index = 275;
    self->tiles[3][17].visual_index = 275;
    self->tiles[3][18].visual_index = 275;
    self->tiles[2][15].visual_index = 277;
    self->tiles[2][16].visual_index = 277;
    self->tiles[2][17].visual_index = 277;
    self->tiles[2][18].visual_index = 277;

    self->tiles[4][15].type = TILE_TYPE_STATIC;
    self->tiles[4][16].type = TILE_TYPE_STATIC;
    self->tiles[4][17].type = TILE_TYPE_FLOOR;
    self->tiles[4][18].type = TILE_TYPE_STATIC;
    self->tiles[3][15].type = TILE_TYPE_STATIC;
    self->tiles[3][16].type = TILE_TYPE_STATIC;
    self->tiles[3][17].type = TILE_TYPE_STATIC;
    self->tiles[3][18].type = TILE_TYPE_STATIC;
    self->tiles[2][15].type = TILE_TYPE_WALL;
    self->tiles[2][16].type = TILE_TYPE_WALL;
    self->tiles[2][17].type = TILE_TYPE_WALL;
    self->tiles[2][18].type = TILE_TYPE_WALL;
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

bool map_is_floor(const Map *self, int x, int y)
{
    return (self->tiles[y][x].type == TILE_TYPE_FLOOR);
}

Vector2i map_get_first_respawnable_point(Map *self){
    Vector2i point = {-1,-1};

    //set up start point
    int walk_x = 0;
    bool all_floors = false;
    for (int walk_y = 0; walk_y < MAP_HEIGHT; ++walk_y){
        // then we check neighbours if all of them are floor we found our point

        // TraceLog(LOG_INFO,"walk:%i %i",walk_x, walk_y);

        for (int i = 0; i < 8; ++i) {
            int nx = walk_x + neighbours_8[i].x;
            int ny = walk_y + neighbours_8[i].y;

            //check where I'm at
            if(!map_is_inside(self,nx,ny)) continue;

            // if walls?
            if(self->tiles[ny][nx].type == TILE_TYPE_WALL){
                all_floors = false;
            };

            if(self->tiles[ny][nx].type == TILE_TYPE_FLOOR){
                all_floors = true;
                // TraceLog(LOG_INFO,"floor:%i:%i %i",i,start_x, start_y);
            };
        }

        if(all_floors){
            point.x = walk_x;
            point.y = walk_y;
            break;
        }

        walk_x++;
    }

    return point;
}

void map_update_respawn_points(Map *self){
    bool visited[MAP_HEIGHT][MAP_WIDTH] = {0};

    Vector2i point = map_get_first_respawnable_point(self);

    Vector2i queue[MAP_WIDTH * MAP_HEIGHT];
    int counter = 0;
    // get all the floor tiles
    for (int y = 0; y < MAP_HEIGHT; ++y){
        for (int x = 0; x < MAP_WIDTH; ++x){
            if(self->tiles[y][x].type == TILE_TYPE_FLOOR){
                queue[counter++] = (Vector2i){0,0};
            }
        }
    }

    int head = 0;
    int tail = 0;

    queue[tail++] = (Vector2i){point.x, point.y};
    self->can_respawn_point[tail++] = (Vector2i){point.x, point.y};
    visited[point.y][point.x] = true;

    self->can_respawn_point_count = 0;

    // BFS search
    while(head < tail){
        Vector2i current = queue[head++];

        for (int i = 0; i < 4; ++i) {
            int nx = current.x + directions[i][0];
            int ny = current.y + directions[i][1];

            // inside the map?self->map.visited_count
            if(!map_is_inside(self,nx,ny)) continue;

            // if walls?
            if(self->tiles[ny][nx].type == TILE_TYPE_WALL) continue;

            // already visited
            if(visited[ny][nx]) continue;

            // self->map_navigation[MAP_NAVIGATION_COST][index_nn] =
            // int current_cost = self->dist[current.y][current.x];
            // self->dist[ny][nx] = current_cost+1;

            queue[tail++] = (Vector2i){nx, ny};
            visited[ny][nx] = true;
            self->can_respawn_point[self->can_respawn_point_count++] = (Vector2i){nx, ny};

        }
    }
}

bool map_is_visible(const Map *self, int x, int y)
{
    return self->tiles[y][x].visible;
}
