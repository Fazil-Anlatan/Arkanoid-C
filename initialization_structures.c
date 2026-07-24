#include "established_parameters.h"
#include "initialization_structures.h"
//-------------GLOBAL VAR DECLARATIONS------------
Ball ball;
Brick brick[BRICK_ROWS][BRICK_COLUMNS];
Paddle paddle;

const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH];

//Initial values for game objects

void gameInicialization() {
   gameInicialization_ball();
   gameInicialization_paddle();
   gameInicialization_bricks();
}

void gameInicialization_ball() {
    
    ball.x = SCREEN_WIDTH / 2;
    ball.y = SCREEN_HEIGHT / 2;
    ball.vx = 0.5f;
    ball.vy = 1.0f;
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
        }
    }

}


