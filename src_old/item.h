#ifndef ITEM_H
#define ITEM_H

typedef enum ItemType{
    ITEM_TYPE_UNDEFINED,
    ITEM_TYPE_POTION,
    ITEM_TYPE_WEAPON,
    ITEM_TYPE_ARMOR,
    ITEM_TYPE_SCROLL,
    ITEM_TYPE_COUNT,
}ItemType;

//-----------------------------------------------
typedef enum PotionType{
    POTION_HEALING,
    POTION_POISON,
    POTION_STRENGHT,
}PotionType;
typedef struct Potion{
    PotionType type;
    int amount;
}Potion;
//-----------------------------------------------
typedef enum WeaponType{
    WEAPON_MELEE,
    WEAPON_RANGED,
}WeaponType;
typedef struct Weapon{
    WeaponType type;
    int amount;
}Weapon;
//-----------------------------------------------
typedef enum ArmorType{
    ARMOR_DEFAULT,
}ArmorType;
typedef struct Armor{
    ArmorType type;
    int amount;
}Armor;
//-----------------------------------------------
typedef enum ScrollType{
    SCROLL_COLD,
    SCROLL_ELECTRICITY,
    SCROLL_HEAT,
    SCROLL_TOXIN,
}ScrollType;
typedef struct Scroll{
    ScrollType type;
    int amount;
}Scroll;

static const char *item_names[] = {
    "UNDEFINED,",
    "POTION OF HEALING",
    "POTION OF POISON",
    "POTION OF STRENGTH",
    "WEAPON",
    "ARMOR",
    "SCROLL",
};

typedef struct Item{

    int x;
    int y;
    bool identified;
    bool equipped;
    char name[STR_LENGHT];
    char description[STR_LENGHT];
    int sprite_index;

    ItemType type;
    union{
        Potion potion;
        Weapon weapon;
        Armor armor;
        Scroll scroll;
    }data;
}Item;

#endif // ITEM_H
