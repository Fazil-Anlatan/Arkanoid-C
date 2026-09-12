#ifndef RENDER
#define RENDER
#include "initialization_structures.h"

void set_blank_screen(const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH]);
void set_borders(const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH]);
void set_ball(const Objects_t* objects,
              const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH]);
void set_bricks(const Objects_t* objects);
void set_paddle(const Objects_t* objects,
                const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH]);
void set_capsule(const Objects_t* objects,
                 const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH]);
void draw_all(const PlayerStats_t* stats, const Objects_t* objects,
              const Timer_t* timer,
              const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH]);
void draw_game_over(int frame_counter);
void draw_game_over_stats(GameState_t game_state, const PlayerStats_t* stats);
#endif

// MENU SCREENS
void draw_intro_animated(GameState_t game_state, int frame_counter);
void draw_username(GameState_t game_state, const PlayerStats_t* stats);
void draw_pause(GameState_t game_state, const PlayerStats_t* stats,
                const Objects_t* objects, const Timer_t* timer,
                const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH]);
void draw_scoreboard(GameState_t game_state);