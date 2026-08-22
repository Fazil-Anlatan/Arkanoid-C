#include "established_parameters.h"
#include "initialization_structures.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
//-------------GLOBAL VAR DECLARATIONS------------
Capsule capsule[MAX_CAPSULES_PER_LEVEL];
Ball ball;
Brick brick[BRICK_ROWS][BRICK_COLUMNS];
Paddle paddle;
int next_capsule; //global declaration of next_capsule for capsule spawn and keeping track.

const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH];
//-------------EXTERNAL VAR DECLARATIONS------------
extern int level;
//Initial values for game objects

void gameInicialization() {
   gameInicialization_ball();
   gameInicialization_paddle();
   gameInicialization_bricks();
   gameInicialization_capsules();
}

void gameInicialization_ball() { //isn't finished yed
    ball.x = SCREEN_WIDTH / 2;
    ball.y = SCREEN_HEIGHT / 2;

    ball.vx = ((time(NULL) % 5) + 1) / 10.0; //x velocity between 0.1 and 0.5
  
    //Total velocity is a vector of 0.75
    //Calculated using the pythagorean theorem
    ball.vy = sqrt(pow(0.75, 2) - pow(ball.vx, 2)); //We need to make another constant for the length of the vector.
    
    //random directions
    if (time(NULL) % 2 == 1) {
        ball.vx *= -1;
    }
    if (time(NULL) % 3 == 1) {
        ball.vy *= -1;
    }
}

void gameInicialization_paddle() {

    paddle.x = (int) (SCREEN_WIDTH / 2);
    paddle.size = PADDLE_WIDTH_0;
}

void gameInicialization_bricks() {
    int i, j;
    
    const int l1_pattern[BRICK_ROWS][BRICK_COLUMNS] = {
        {1, 1, 0, 1, 1, 0, 1, 1},
        {1, 1, 0, 1, 1, 0, 1, 1},
        {1, 1, 0, 1, 1, 0, 1, 1},
        {1, 1, 0, 1, 1, 0, 1, 1}
    };

    for (i = 0; i < BRICK_ROWS; i++) {
        for (j = 0; j < BRICK_COLUMNS; j++) {
           brick[i][j].health = 0;
        }
    }

    switch (level) {
        case 1:
            for (i = 0; i < BRICK_ROWS; i++) { // Use the predefined pattern for level 1 (similar to the real game)
                for (j = 0; j < BRICK_COLUMNS; j++) {
                    brick[i][j].health = l1_pattern[i][j];
                }
            }
            break;

        case 2:     //inverted triangle of bricks
            for (i = 0; i < BRICK_MAX_ROWS; i++) { 
                for (j = 0; j < BRICK_COLUMNS; j++) {
                    if (j >= i && j < BRICK_COLUMNS - i) {
                        brick[i][j].health = 1;
                    }
                }
            }
            break;
            
            break;

        case 3:
            for (i = 1; i < BRICK_ROWS+3; i++) { //square of bricks
                for (j = 1; j < BRICK_COLUMNS-1; j++) {
                    brick[i][j].health = 1;
                }
            }
            break;

        default: // For levels greater than 3, a random pattern of bricks will be generated
            char more_rows = (level -3)%BRICK_ROWS; // this will ad more rows of bricks if you pass level 3,
            for (i = 0; i < BRICK_ROWS + more_rows; i++) {
                for (j = 0; j < BRICK_COLUMNS; j++) {
                    brick[i][j].health = (rand() % 10 < 7) ? 1 : 0; //70 % of chace to generate a brick in 4 rows and 8 columns
                }
            }
            break;
    }
}


void gameInicialization_capsules(){
    int i,j,k;
    next_capsule = 0;
    k = 0;
    for (i = 0; i < BRICK_ROWS; i++) {
        for (j = 0; j < BRICK_COLUMNS; j++) {
            if ((brick[i][j].contains_capsule > 0) && (k < MAX_CAPSULES_PER_LEVEL)) {
                capsule[k].x = 0;
                capsule[k].y = 0;
                capsule[k].vy = 0;
                capsule[k].type = 0;
                k++;
            }
        }
    }
}

