#include "camera_system.h"
#include "map.h"

void camera_system(Camera2D *camera, Vector2 target, float delta_time){
    // float smoothness = 2.0f; // Higher = follows faster
    // float t = 1.0f - expf(-smoothness * delta_time);

    // camera->target.x += ((target.x * TILE_SIZE) - camera->target.x) * t;
    // camera->target.y += ((target.y * TILE_SIZE) - camera->target.y) * t;

    const float smoothness = 2.f;
    float t = 1.0f - expf(-smoothness * delta_time);

    // Smooth in world coordinates
    camera->target.x += (target.x * TILE_SIZE - camera->target.x) * t;
    camera->target.y += (target.y * TILE_SIZE - camera->target.y) * t;

    // Snap to the nearest screen pixel (assuming zoom == 1)
    camera->target.x = roundf(camera->target.x);
    camera->target.y = roundf(camera->target.y);
}