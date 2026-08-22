#include <math.h>
#include "established_parameters.h"
#include "initialization_structures.h"
#include "physics.h"
#include "structures.h"
//-------------EXTERNAL VAR DECLARATIONS------------
extern Ball ball;
extern Brick brick[BRICK_ROWS][BRICK_COLUMNS];
extern Paddle paddle;
extern Capsule capsule[MAX_CAPSULES_PER_LEVEL];
extern int next_capsule;
extern GameState game_state;
extern int score;
extern short int lives;
extern short int level;
extern const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH];
//------- GAME UPDATES -------

void ball_update() {

    //position update
    ball.x += ball.vx;
    ball.y += ball.vy;

    if ((int)ball.x <= 1 || (int)ball.x >= SCREEN_WIDTH - 2) {
        ball.vx *= -1;
    }
    if ((int)ball.y <= 1) {
        ball.vy *= -1;
    }
    if (((int)ball.y >= SCREEN_HEIGHT - 1)) {
        if (lives >1){
        lives--;
        gameInicialization_ball();
        }
        else{
            game_state = STATE_QUIT; // 0 now means STATE_INTRO, so it has to be named
        }

    }

    //paddle collision
    if ((int)ball.y == (SCREEN_HEIGHT - 3)){
        if ((int)ball.x >= paddle.x && (int)ball.x <= (paddle.x + PADDLE_WIDTH_0)) {

            //advanced paddle bounce
            float speed = sqrtf(ball.vx * ball.vx + ball.vy * ball.vy);
            float center = paddle.x + paddle.size / 2.0f;
            float offset = (ball.x - center) / (paddle.size / 2.0f);

            //limit
            if (offset < -1.0f) {
                offset = -1.0f;}

                if (offset > 1.0f) {
                    offset = 1.0f;}

                    float angle = offset * MAX_ANGLE;
                    ball.vx = speed * sinf(angle);
                    ball.vy = -speed * cosf(angle);

                    //end advances paddle bounce
        }
    }

    //brick collision
    //create the indexes of the bricks that the ball is inside of.
    int brick_y = ((int)ball.y - 1) / BRICK_HEIGHT;
    int brick_x = ((int)ball.x - 1) / BRICK_WIDTH;

    if (brick_y < BRICK_ROWS && brick_x < BRICK_COLUMNS) {//only start if statement if the ball is inside a rendered or unrendered brick.

        if (brick[brick_y][brick_x].health > 0) {//only start if statement if the brick is alive
            brick[brick_y][brick_x].health -= 1;
            score++;
            //capsules in bricks
            if (brick[brick_y][brick_x].contains_capsule && next_capsule < MAX_CAPSULES_PER_LEVEL) {
                capsule[next_capsule].x = brick_x * BRICK_WIDTH + BRICK_WIDTH / 2;
                capsule[next_capsule].y = brick_y * BRICK_HEIGHT + BRICK_HEIGHT / 2;

                capsule[next_capsule].vy = 0.5f;
                next_capsule++;
            }

            
            //start collision test 
            float previous_ball_x = ball.x - ball.vx;
            float previous_ball_y = ball.y - ball.vy;

            int brick_left_limit = 1 + brick_x * BRICK_WIDTH;
            int brick_right_limit = brick_left_limit + BRICK_WIDTH - 1;

            int brick_top_limit = 1 + brick_y * BRICK_HEIGHT;
            int brick_bottom_limit = brick_top_limit + BRICK_HEIGHT - 1;

            if (previous_ball_x < brick_left_limit || previous_ball_x > brick_right_limit)
            {
                // Hit the side
                ball.vx *= -1;
            }
            if (previous_ball_y < brick_top_limit || previous_ball_y > brick_bottom_limit)
            {
                // Hit the top or bottom
                ball.vy *= -1;
            }
        }
    }
}

void paddle_update(int c)
{
    if ((c == 'a'|| c == 'A' || c == KEY_LEFT) && paddle.x > 1)
    {
        paddle.x -= 1; // Move paddle left
    }
    else if ((c == 'd' || c =='D' || c == KEY_RIGHT) && paddle.x < (SCREEN_WIDTH - paddle.size - 1))
    {
        paddle.x += 1; // Move paddle right
    }
}

void capsule_update() {
    int k;
    for (k = 0; k < MAX_CAPSULES_PER_LEVEL; k++) {
        if (capsule[k].vy > 0) {
            capsule[k].y += capsule[k].vy;
            if (capsule[k].y >= SCREEN_HEIGHT){
                capsule[k].vy = 0;
            }
        }
    }
}
