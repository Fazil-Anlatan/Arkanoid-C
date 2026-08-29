#ifndef PHYSICS
#define PHYSICS
#include "initialization_structures.h"
//-----------------FUCTION DECLARATIONS------------
void ball_update(Game *game);
void paddle_update(Game *game, int c);
void capsule_update(Game *game);
void activate_powerup(Game *game, int k);
void timers_update(Game *game);
#endif
