#ifndef FACTORY_ITEM_H
#define FACTORY_ITEM_H

#include "item.h"

typedef struct Dungeon Dungeon;

Item *factory_item_potion(Dungeon *dungeon,int x, int y, PotionType type, int amount);
Item *factory_item_weapon(Dungeon *dungeon,int x, int y, WeaponType type, int amount);
Item *factory_item_armor(Dungeon *dungeon,int x, int y, ArmorType type, int amount);
Item *factory_item_scroll(Dungeon *dungeon,int x, int y, ScrollType type, int amount);

#endif // FACTORY_ITEM_H
