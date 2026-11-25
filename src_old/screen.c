#include "screen.h"

void screen_shake_start(ScreenShake *self, float duration, float intensity){
    if(duration > self->time){
        self->time = duration;
    }

    if(intensity > self->intensity){
        self->intensity = intensity;
    }
}

ScreenTransition screen_transition_create(void){
    ScreenTransition transition = (ScreenTransition){
            .phase = TRANSITION_NONE,
            .alpha = 0.0f,
            .speed = 3.0f,
            };
    return transition;
};
void screen_transition_begin_fade_out(ScreenTransition *self){
    self->phase = TRANSITION_FADE_OUT;
    self->alpha = 0.0f;
}

void screen_transition_begin_fade_in(ScreenTransition *self){
    self->phase = TRANSITION_FADE_IN;
}

void screen_transition_update(ScreenTransition *transition, float delta_time) {

    float amount = transition->speed * delta_time;

    switch (transition->phase) {
        case TRANSITION_FADE_OUT:
            transition->alpha += amount;

            if (transition->alpha >= 1.0f) {
                transition->alpha = 1.0f;
                transition->phase = TRANSITION_FADE_IN;
            }
            break;

        case TRANSITION_FADE_IN:
            transition->alpha -= amount;

            if (transition->alpha <= 0.0f) {
                transition->alpha = 0.0f;
                transition->phase = TRANSITION_NONE;
            }
            break;

        case TRANSITION_NONE:
            break;
    }
}