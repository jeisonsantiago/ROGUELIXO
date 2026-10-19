#include "dungeon.h"


static void distribute_items(Dungeon *self, int player_x, int player_y){
    int max_dungeon_items = 2;

    for (int i = 0; i < max_dungeon_items; ++i) {
        // get first room dimensions
        int room_w = self->map.rooms[0].w;
        int room_h = self->map.rooms[0].h;

        int random_x = GetRandomValue(1,room_w-1) + self->map.rooms[0].x;
        int random_y = GetRandomValue(1,room_h-1) + self->map.rooms[0].y;

        while ((random_x == player_x && random_y == player_y)) {
            random_x = GetRandomValue(1,room_w-1) + self->map.rooms[0].x;
            random_y = GetRandomValue(1,room_h-1) + self->map.rooms[0].y;
        }

        random_x = GetRandomValue(1,room_w-1) + self->map.rooms[0].x;
        random_y = GetRandomValue(1,room_h-1) + self->map.rooms[0].y;

        // Item it ={
        //     .identified = true,
        //     .type = ITEM_TYPE_POTION_HEALING,
        //     .x =random_x,
        //     .y = random_y,
        //     .amount = 1,
        //     .sprite_index = 17
        // };

        Item it ={
            .identified = true,
            .type = ITEM_TYPE_WEAPON,
            .x =random_x,
            .y = random_y,
            .amount = 2,
            .sprite_index = (16*4)
        };

        Item it2 ={
            .identified = true,
            .type = ITEM_TYPE_ARMOR,
            .x =random_x,
            .y = random_y,
            .amount = 1,
            .sprite_index = (33),
        };

        self->items[self->item_count++] = (i)?it2:it;
    }
}

void dungeon_init(Dungeon *self, int player_x, int player_y){
    map_init(&self->map);

    self->tile_index_floor = 80;
    self->tile_index_wall = 64;
    self->monster_count = 0;

    distribute_items(self, player_x, player_y);

    // set stairs
    self->stairs_x = centre_x(&self->map.rooms[self->map.rooms_count]);
    self->stairs_y = centre_y(&self->map.rooms[self->map.rooms_count]);
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
