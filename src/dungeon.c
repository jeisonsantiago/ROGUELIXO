#include "dungeon.h"

#include "factory_item.h"

Vector2i dungeon_random_position(Dungeon *self, int not_x, int not_y, int room)
{
    int room_w = self->map.rooms[room].w;
    int room_h = self->map.rooms[room].h;

    int random_x = GetRandomValue(1,room_w-1) + self->map.rooms[room].x;
    int random_y = GetRandomValue(1,room_h-1) + self->map.rooms[room].y;

    while ((random_x == not_x && random_y == not_y)) {
        random_x = GetRandomValue(1,room_w-1) + self->map.rooms[room].x;
        random_y = GetRandomValue(1,room_h-1) + self->map.rooms[room].y;
    }

    return (Vector2i){random_x, random_y};
}


// only works after all items ar on
Vector2i dungeon_random_position_room(Dungeon *self, int room){

    int room_w = self->map.rooms[room].w;
    int room_h = self->map.rooms[room].h;

    int random_x = GetRandomValue(1,room_w-1) + self->map.rooms[room].x;
    int random_y = GetRandomValue(1,room_h-1) + self->map.rooms[room].y;

    // check if it's floor
    while (!map_is_floor(&self->map, random_x, random_y)) {
        random_x = GetRandomValue(1,room_w-1) + self->map.rooms[room].x;
        random_y = GetRandomValue(1,room_h-1) + self->map.rooms[room].y;
    }

    // check if it's  there's player
    while (random_x == self->player_start_x && random_y == self->player_start_y) {
        random_x = GetRandomValue(1,room_w-1) + self->map.rooms[room].x;
        random_y = GetRandomValue(1,room_h-1) + self->map.rooms[room].y;
    }

    return (Vector2i){random_x, random_y};
}

Vector2i dungeon_random_position_cave(Dungeon *self)
{
    Vector2i point = self->map.can_respawn_point[GetRandomValue(0,self->map.can_respawn_point_count-1)];
    // TraceLog(LOG_INFO,"random:%i %i",point.x, point.y);
    return point;
}

static void distribute_items(Dungeon *self, int player_x, int player_y){
    int max_dungeon_items = 2;

    if(self->type == DUNGEON_TYPE_CAVE){
        for (int i = 0; i < max_dungeon_items; ++i) {

            Vector2i pos = dungeon_random_position_cave(self);

            for (int it = i; it < i; ++it) {
                if(self->item_count > 0){
                    Item *item = &self->items[it];
                    while (pos.x == item->x && pos.y == item->y) {
                        pos = dungeon_random_position_cave(self);
                    }
                }
            }

            int random_x = pos.x;
            int random_y = pos.y;

            if((i % 2) ==0){
                factory_item_potion(self,random_x, random_y, POTION_HEALING, 1);
            }else{
                // factory_item_weapon(self,random_x, random_y, WEAPON_MELEE, 1);
                factory_item_weapon(self,random_x, random_y, WEAPON_RANGED, 1);
            }
        }
    }else if(self->type == DUNGEON_TYPE_ROOMS){
        for (int i = 0; i < max_dungeon_items; ++i) {

            Vector2i pos = dungeon_random_position(self,player_x, player_y, 0);

            for (int it = 0; it < i; ++it) {
                Item *item = &self->items[it];
                while (pos.x == item->x && pos.y == item->y) {
                    pos = dungeon_random_position(self,item->x, item->y, 0);
                }
            }

            int random_x = pos.x;
            int random_y = pos.y;

            Item it ={
                .identified = true,
                .type = ITEM_TYPE_WEAPON,
                .x =random_x,
                .y = random_y,
                .data.weapon = (Weapon){.amount = 2, .type = WEAPON_MELEE} ,
                .sprite_index = (16*4)
            };

            Item it2 ={
                .identified = true,
                .type = ITEM_TYPE_ARMOR,
                .x =random_x,
                .y = random_y,
                .data.armor = (Armor){.amount = 1, .type = ARMOR_DEFAULT},
                .sprite_index = (33),
            };

            self->items[self->item_count++] = (i)?it2:it;
        }
    }
}

void dungeon_set(Dungeon *self, DungeonType type)
{
    // memset(self,0,sizeof *self);
    self->type = type;
}


void dungeon_init(Dungeon *self, int player_x, int player_y, DungeonType type){

    memset(self,0,sizeof(Dungeon));
    self->type = type;

    Vector2i pos = {0};

    if(type == DUNGEON_TYPE_VILLAGE){
        map_init_village(&self->map);
        pos = (Vector2i){4,4};

        self->tile_index_floor = 80;
        self->tile_index_wall = 64;
        self->monster_count = 0;
        self->item_count = 0;

        self->player_start_x = pos.x;
        self->player_start_y = pos.y;

        self->stairs_down_x = 3;
        self->stairs_down_y = 3;

        // add NPC
        int monster_index = self->monster_count;
        // TraceLog(LOG_INFO, "monster created index: %i",monster_index);
        Entity *entity = &self->monster[monster_index];
        self->monster_count = 1;
        entity_init(entity,ENTITY_TYPE_NPC);
        entity_set_sprite(entity,41);
        entity_set_position(entity,2,2);
        // npc_entity_setup(entity,"Entra la fera...! teu cu nao sera perdoado!"
        //                  );

        npc_entity_setup(entity,"Dae malandrao entra no buraco ae!!!");

        return;
    }else if(type == DUNGEON_TYPE_ROOMS){
        map_init(&self->map);
        pos = dungeon_random_position_room(self,0);

        // if max number of



        // set stairs

        self->stairs_up_x = centre_x(&self->map.rooms[0]);
        self->stairs_up_y = centre_y(&self->map.rooms[0]);

        self->stairs_down_x = centre_x(&self->map.rooms[self->map.rooms_count-1]);
        self->stairs_down_y = centre_y(&self->map.rooms[self->map.rooms_count-1]);

    }else if (type == DUNGEON_TYPE_CAVE){
        map_init_cave(&self->map);
        pos = (Vector2i){4,4};
    }

    self->map.can_respawn_point_count = 0;
    map_update_respawn_points(&self->map);

    pos = map_get_first_respawnable_point(&self->map);

    self->tile_index_floor = 80;
    self->tile_index_wall = 64;
    self->monster_count = 0;
    self->item_count = 0;

    distribute_items(self, pos.x, pos.y);

    self->player_start_x = pos.x;
    self->player_start_y = pos.y;
}

void dungeon_remove_item(Dungeon *self, int x, int y)
{
    //get item index
    for (int i = 0; i < self->item_count; ++i) {
        if(self->items[i].x == x && self->items[i].y == y){

            if(i == (self->item_count-1)){
                self->item_count--;
                return;
            }

            TraceLog(LOG_INFO,"REMOVE %i",i);
            self->items[i] = self->items[--self->item_count];
            return;
        }
    }
}

static void dungeon_populate(Dungeon *self, int player_x, int player_y, DungeonType type)
{
    dungeon_init(self, player_x, player_y, type);
}

void dungeon_remove_monster(Dungeon *self, int x, int y)
{
    //get item index
    for (int i = 0; i < self->monster_count; ++i) {
        if(self->monster[i].x == x && self->monster[i].y == y){

            TraceLog(LOG_INFO,"REMOVE MONSTER %i",i);

            if(i == (self->monster_count-1)){
                self->monster_count--;
                return;
            }


            self->monster[i] = self->monster[--self->monster_count];
            return;
        }
    }
}

void dungeon_remove_monster_by_index(Dungeon *self, int index){
    if(index == (self->monster_count-1)){
        self->monster_count--;
        return;
    }

    self->monster[index] = self->monster[--self->monster_count];
    return;
}

bool dungeon_has_stairs_up(Dungeon *self){
    return self->stairs_up_x != 0 && self->stairs_up_y != 0;
}

bool dungeon_has_stairs_down(Dungeon *self){
    return self->stairs_down_x != 0 && self->stairs_down_y != 0;
}

