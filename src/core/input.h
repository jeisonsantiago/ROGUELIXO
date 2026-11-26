#ifndef INPUT_H
#define INPUT_H


typedef enum {
    ACTION_UP, ACTION_DOWN, ACTION_LEFT, ACTION_RIGHT,
    ACTION_FIRE,
    ACTION_COUNT
} Action;

void    input_update(Vector2 virtual_mouse); /* once per frame */
void    input_end_step(void);                /* after a fixed step */
bool    input_down(Action a);
bool    input_pressed(Action a);
Vector2 input_move_dir(void);                /* normalized, 8 directions */
Vector2 input_move_dir_raw(void);            /* not normalized, 8 directions */
Vector2 input_mouse(void);                   /* in game coords */


#endif // INPUT_H
