#ifndef SCREEN_H
#define SCREEN_H

typedef struct Game Game;

typedef struct ScreenShake{
    float time;
    float intensity;
}ScreenShake;

void screen_shake_start(ScreenShake *self, float duration, float intensity);
// void screen_shake_update(Game *game, float delta_time);

typedef enum TransitionPhase{
    TRANSITION_NONE,
    TRANSITION_FADE_OUT,
    TRANSITION_FADE_IN,
}TransitionPhase;

typedef struct ScreenTransition{
    TransitionPhase phase;
    float alpha;
    float speed;
}ScreenTransition;

ScreenTransition screen_transition_create(void);

void screen_transition_begin_fade_out(ScreenTransition *self);
void screen_transition_begin_fade_in(ScreenTransition *self);

void screen_transition_update(ScreenTransition *self, float delta_time);



#endif // SCREEN_H
