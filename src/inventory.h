#ifndef INVENTORY_H
#define INVENTORY_H

#include "item.h"

#define MAX_INVENTORY_SLOTS 20

typedef struct GamePlay GamePlay;

typedef struct InventorySlot{
    Item item;
    bool occupied;
}InventorySlot;

typedef struct Inventory{
    InventorySlot inventory[MAX_INVENTORY_SLOTS];
    int inventory_count;
    Rectangle rect_slots[MAX_INVENTORY_SLOTS];
    Rectangle rect_armor_slot;
    Rectangle rect_weapon_slot;
    int selected_slot;
}Inventory;

void inventory_init(Inventory *self);
int inventory_add_item(Inventory *self, Item item);
bool inventory_remove(Inventory *self, int slot);

int inventory_update(Inventory *self, GamePlay *gameplay);
void inventory_render(Inventory *self, Camera2D *camera);


#endif // INVENTORY_H

