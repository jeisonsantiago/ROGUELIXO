#ifndef TRAIT_H
#define TRAIT_H

typedef enum TraitType{
    TRAIT_UNDEFINED = 0,
    TRAIT_VENOMOUS,
    TRAIT_COUNT,
}TraitType;

typedef struct Trait{
    TraitType type;
    int turns;
    int chance;
}Trait;

#endif // TRAIT_H
