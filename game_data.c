#include "established_parameters.h"
#include "initialization_structures.h"
#include "physics.h"

//-------------EXTERNAL VAR DECLARATIONS------------
extern Ball ball;
extern Brick brick[BRICK_ROWS][BRICK_COLUMNS];
extern Paddle paddle;
extern const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH];


//-------------GLOBAL VAR DECLARATIONS------------
short int game_state = 1;
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
    }
}

//------- END LEVEL UP -------
