#ifndef G_DATA
#define G_DATA
#include "application_state.h"
#include "initialization_structures.h"
//------FUCTION DECLARATIONS-------------
void game_data_init(Game *game); // sets the starting values that used to be global variable initializers
void level_up(Game *game, int c);
int advance_score_calc(int brick_y);
int  load_scores(ScoreEntry top[], int max);
void reset_game();
#endif
