#ifndef G_DATA
#define G_DATA
#include "application_state.h"
#include "initialization_structures.h"
//------FUCTION DECLARATIONS-------------
void game_data_init(GameState_t* game_state, PlayerStats_t* stats,
                    Objects_t* objects);  // sets the starting values that used
                                          // to be global variable initializers
void level_up(PlayerStats_t* stats, Objects_t* objects, Timer_t* timer, int c);
int score_calc(PlayerStats_t* stats, Objects_t* objects, int brick_x,
               int brick_y, Ball_t* hitting_ball);
int load_scores(ScoreEntry_t top[], int max);
void save_score(const PlayerStats_t* stats);  // adds one line to scores.txt
                                              // every time a game ends
void reset_game(PlayerStats_t* stats, Objects_t* objects, Timer_t* timer,
                int* frame_counter);
#endif