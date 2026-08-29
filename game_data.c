#include "established_parameters.h"
#include "initialization_structures.h"
#include "physics.h"
#include "application_state.h"

//-------------GAME STATE INITIALIZATION------------
// These used to be global variable declarations with their initial values.
// Now that they live inside the Game struct, they're set here once instead.
void game_data_init(Game *game) {
    game->game_state = STATE_INTRO; // was: short int = 1. Starts at the title screen now.
    game->username[0] = '\0'; // for the top score file later
    game->score = 0;
    game->lives = 3;
    game->level = 1;
}

int advance_score_calc(int brick_y) // for bonus
{
    (void)brick_y;
    return 1;
}

//------- LEVEL UP -------
void level_up(Game *game, int c) {
// if the combined health of all bricks is 0, level up.
    int j, i;
    game -> combined_brick_health = 0;
    for (i = 0; i < BRICK_ROWS; i++){
        for (j = 0; j < BRICK_COLUMNS; j++){
            game -> combined_brick_health += game->brick[i][j].health;
        }
    }
    if ((game -> combined_brick_health == 0)||(c=='l')||(c=='L')){ //falta la tecla para pasar de nivel (modo desarrollador )pero primero tengo que dividir el codigo
                game->level++;
                gameInicialization_bricks(game);
                gameInicialization_ball(game);
                gameInicialization_capsules(game);
    }
}

//------- END LEVEL UP -------
