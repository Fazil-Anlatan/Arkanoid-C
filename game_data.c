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
void level_up(int c) {
// if the combined health of all bricks is 0, level up.
    int j, i;
    int combined_brick_health = 0;
    for (i = 0; i < BRICK_ROWS; i++){
        for (j = 0; j < BRICK_COLUMNS; j++){
            combined_brick_health += brick[i][j].health;
        }
    }
    if ((combined_brick_health == 0)||(c=='l')||(c=='L')){ //falta la tecla para pasar de nivel (modo desarrollador )pero primero tengo que dividir el codigo
                level++;
                gameInicialization_bricks();
                gameInicialization_ball();
                gameInicialization_capsules();
    }
}

//------- END LEVEL UP -------

//----- History Log ------
/* adds one line to scores.txt every time a game ends. Format is: username score level  */

void save_score() {

    FILE *fp;  

    fp = fopen("scores.txt", "a");   // "a" = adding mode

    if (fp == NULL) {      // fopen returns NULL if it failed so the game never crashes
        return;                        
    }

    fprintf(fp, "%s %d %d\n", username, score, level);

    fclose(fp);    // flushes the text to disk
}

//---- End History Log-------
