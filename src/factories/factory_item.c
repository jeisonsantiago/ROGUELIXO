#include "factory_item.h"
#include "dungeon.h"

Item *factory_item_potion(Dungeon *dungeon,int x, int y, PotionType type, int amount)
{
    Item it ={
        .identified = true,
        .type = ITEM_TYPE_POTION,
        .x = x,
        .y = y,
        .data.potion = (Potion){.amount = 2, .type = type},
        .sprite_index = (17)
    };

    switch (type) {
    case POTION_HEALING:{
        strcpy(it.description,TextFormat("Potion of Healing: %i HP",amount));
        break;
    }
    default:
        break;
    }

    int index = dungeon->item_count;
    dungeon->items[index] = it;
    dungeon->item_count++;

    return &dungeon->items[index];
};

Item *factory_item_weapon(Dungeon *dungeon,int x, int y, WeaponType type, int amount)
{
    Item it ={
        .identified = true,
        .type = ITEM_TYPE_WEAPON,
        .x = x,
        .y = y,
        .data.weapon = (Weapon){.amount = 2, .type = type},        
    };

    switch (type) {
    case WEAPON_MELEE:{
        it.sprite_index = (16*4);
        strcpy(it.description,TextFormat("Sword of Bundas: %i STR",amount));
        break;
    }
    case WEAPON_RANGED:{
        it.sprite_index = (16*4+9);
        strcpy(it.description,TextFormat("Bow of Fuckery: %i STR",amount));
        break;
    }
    default:
        it.sprite_index = (16*4);
        break;
    }

    int index = dungeon->item_count;
    dungeon->items[index] = it;
    dungeon->item_count++;

    return &dungeon->items[index];
};

Item *factory_item_armor(Dungeon *dungeon,int x, int y, ArmorType type, int amount)
{
    Item it ={
        .identified = true,
        .type = ITEM_TYPE_ARMOR,
        .x = x,
        .y = y,
        .data.armor = (Armor){.amount = 2, .type = type},
        .sprite_index = (33)
    };

    int index = dungeon->item_count;
    dungeon->items[index] = it;
    dungeon->item_count++;

    return &dungeon->items[index];
};

Item *factory_item_scroll(Dungeon *dungeon,int x, int y, ScrollType type, int amount)
{
    Item it ={
        .identified = true,
        .type = ITEM_TYPE_SCROLL,
        .x = x,
        .y = y,
        .data.scroll = (Scroll){.amount = 2, .type = type},
        .sprite_index = (16*4)
    };

    int index = dungeon->item_count;
    dungeon->items[index] = it;
    dungeon->item_count++;

    return &dungeon->items[index];
};