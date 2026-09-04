#include "established_parameters.h"
#include "initialization_structures.h"
#include "physics.h"
#include "structures.h"
#include <stdio.h>

//-------------EXTERNAL VAR DECLARATIONS------------
extern Ball ball;
extern Brick brick[BRICK_ROWS][BRICK_COLUMNS];
extern Paddle paddle;
extern const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH];


//-------------GLOBAL VAR DECLARATIONS------------
GameState game_state = STATE_INTRO;
char username[MAX_USERNAME] = ""; // for the top score file later
int score = 0;
short int lives = 3;
short int level = 1;
void advance_score_calc() // for bonus
{}

//------- LEVEL UP -------
void level_up() {
// if the combined health of all bricks is 0, level up.
    int j, i;
    int combined_brick_health = 0;
    for (i = 0; i < BRICK_ROWS; i++){
        for (j = 0; j < BRICK_COLUMNS; j++){
            combined_brick_health += brick[i][j].health;
        }
    }
    if ((combined_brick_health == 0)){ //falta la tecla para pasar de nivel (modo desarrollador )pero primero tengo que dividir el codigo
                level++;
                gameInicialization_bricks();
                gameInicialization_ball();
                gameInicialization_capsules();
    }
}

//------- END LEVEL UP -------

//------- Restart Function ----------
// basically resets the game, rebuilds the game board and stats as it was in the starting values and positions

void reset_game() {
    score = 0;
    lives = 3;
    level = 1;
    gameInicialization();
}
//------- End Restart ---------
//----- History Log ------
// adds one line to scores.txt every time a game ends. Format is: username score level 

void save_score() {

    FILE *fp;  

    fp = fopen("scores.txt", "a");   // "a" = adding mode

    if (fp == NULL) {      // fopen returns NULL if it failed so the game never crashes
        return;                        
    }

    fprintf(fp, "%s %d %d\n", username, score, level);

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