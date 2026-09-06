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

int advance_score_calc(Game *game, int brick_y) // for bonus
{
  /*  int total_rows = BRICK_ROWS; // initial value for levels 1 and 2, which have 4 rows of bricks

    // from level 3, we need to calculate the total number of rows based on the level
    if (game->level == 3) {
        total_rows = BRICK_ROWS + 3; //7rows
    } 
    else if (game->level > 3) {
        total_rows = BRICK_ROWS + ((game->level - 3) % BRICK_ROWS); //same logic we use to genereta the rows in the brick initialization function
    }

    // last rows =1 point +2 for each row above it
    int points = 1 + 2 * ((total_rows - 1) - brick_y);

    //just in case, and error there is an error in the code, we dont subtract points from the score
    if (points < 1) {
        points = 1;
    }

    return points;*/
    return 1; // for now, just return 1 point for each brick destroyed
}

//------- LEVEL UP -------
void level_up(Game *game, int c) {
// if the combined health of all bricks is 0, level up.
    int j, i;
    game->combined_brick_health = 0;
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



//------- Restart Function ----------
// basically resets the game, rebuilds the game board and stats as it was in the starting values and positions

void reset_game(Game *game) {
    game->score = 0;
    game->lives = 3;
    game->level = 1;
    gameInicialization(game);
}
//------- End Restart ---------
//----- History Log ------
// adds one line to scores.txt every time a game ends. Format is: username score level 

void save_score(Game *game) {

    FILE *fp;  

    fp = fopen("scores.txt", "a");   // "a" = adding mode

    if (fp == NULL) {      // fopen returns NULL if it failed so the game never crashes
        return;                        
    }

    fprintf(fp, "%s %d %d\n", game->username, game->score, game->level);

    fclose(fp);    // flushes the text to disk
}
// Reads scores.txt, sorts it best-first, and copies the top "max" lines into top[]

#define MAX_SAVED_SCORES 100   // safety cap

int load_scores(ScoreEntry top[], int max) {

    FILE *fp;
    ScoreEntry all[MAX_SAVED_SCORES];
    ScoreEntry temp;
    int count = 0;
    int i, j;

    fp = fopen("scores.txt", "r");

    if (fp == NULL) {   // when no score file has been cretaed yet
        return 0;
    }

    // one line = name, score, level. fscanf returns 3 when all three were read
    while (count < MAX_SAVED_SCORES &&
           fscanf(fp, "%15s %d %d", all[count].name, &all[count].score, &all[count].level) == 3) {
        count++;
    }

    fclose(fp);

    // highest score first
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - 1 - i; j++) {
            if (all[j].score < all[j + 1].score) {
                temp = all[j];
                all[j] = all[j + 1];
                all[j + 1] = temp;
            }
        }
    }

    if (count > max) {
        count = max;
    }

    for (i = 0; i < count; i++) {
        top[i] = all[i];
    }

    return count;
}

//---- End History Log-------