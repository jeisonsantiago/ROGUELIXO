#ifndef ROOM_H
#define ROOM_H

typedef struct Room{
    int x;
    int y;
    int w;
    int h;
}Room;

void room_init(Room *self);
const int centre_x(Room *self);
const int centre_y(Room *self);

#endif // ROOM_H
