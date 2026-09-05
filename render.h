#ifndef RENDER
#define RENDER
#include "initialization_structures.h"

void set_blank_screen(Game *game);
void set_borders(Game *game);
void set_ball(Game *game);
void set_bricks(Game *game);
void set_paddle(Game *game);
void set_capsule(Game *game);
void draw_all(Game *game);
void draw_game_over(Game *game);
#endif

//MENU SCREENS
void draw_intro(Game *game);
void draw_username(Game *game);
void draw_pause(Game *game);
void draw_scoreboard(Game *game);