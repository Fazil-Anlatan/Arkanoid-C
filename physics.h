#ifndef PHYSICS
#define PHYSICS
#include "initialization_structures.h"
//-----------------FUCTION DECLARATIONS------------
void ball_update(GameState_t* game_state, PlayerStats_t* stats,
                 Objects_t* objects, int* frame_counter);
void paddle_update(Objects_t* objects, int c);
void capsule_update(PlayerStats_t* stats, Objects_t* objects, Timer_t* timer);
void activate_powerup(PlayerStats_t* stats, Objects_t* objects, Timer_t* timer,
                      int k);
void timers_update(Objects_t* objects, Timer_t* timer);
#endif