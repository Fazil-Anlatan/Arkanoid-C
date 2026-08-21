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
    ball.vy = sqrt(pow(1.5, 2) - pow(ball.vx, 2)); //We need to make another constant for the length of the vector.
    
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
    int i,j;
    for (i = 0; i < BRICK_ROWS; i++) {
        for (j = 0; j < BRICK_COLUMNS; j++) {
            brick[i][j].health = 1;
            brick[i][j].contains_capsule = 1; //JUST FOR TESTING
        }
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

