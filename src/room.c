#include "room.h"



void room_init(Room *self)
{
    self->x = 0;
    self->y = 0;
    self->w = 0;
    self->h = 0;
}


const int centre_x(Room *self){
    return self->x + self->w / 2;
};
const int centre_y(Room *self){
    return self->y + self->h / 2;
};