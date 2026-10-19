#ifndef ITEM_H
#define ITEM_H



typedef enum ItemType{
    ITEM_TYPE_UNDEFINED,
    ITEM_TYPE_POTION_HEALING,
    ITEM_TYPE_POTION_POISON,
    ITEM_TYPE_POTION_STRENGTH,
    ITEM_TYPE_WEAPON,
    ITEM_TYPE_ARMOR,
    ITEM_TYPE_COUNT,
}ItemType;

static const char *item_names[] = {
    "UNDEFINED,",
    "POTION OF HEALING",
    "POTION OF POISON",
    "POTION OF STRENGTH",
    "WEAPON",
    "ARMOR",
};

typedef struct Item{
    ItemType type;
    int x;
    int y;
    bool identified;
    bool equipped;
    char name[STR_LENGHT];
    int sprite_index;

    int amount;
}Item;

#endif // ITEM_H
