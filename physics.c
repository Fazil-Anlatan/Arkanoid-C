#include <math.h>
#include "established_parameters.h"
#include "initialization_structures.h"
#include "game_data.h"
#include "physics.h"
//------- GAME UPDATES -------

void ball_update(Game *game) {
    int i;

    for (i = 0; i < game->active_balls; i++) {
        Ball *ball = &game->ball[i];

        //position update
        ball->x += ball->vx;
        ball->y += ball->vy;

        if ((int)ball->x <= 1 || (int)ball->x >= SCREEN_WIDTH - 2) {
            ball->vx *= -1;
        }
        if ((int)ball->y <= 1) {
            ball->vy *= -1;
        }
        if ((int)ball->y >= SCREEN_HEIGHT - 1) {
            if (game->active_balls > 1) {
                game->ball[i] = game->ball[game->active_balls - 1];
                game->active_balls--;
                i--;
                continue;
            }
            if (game->lives > 1) {
                game->lives--;
                gameInicialization_ball(game);
                return;
            }
            game->game_state = STATE_GAME_OVER;
            game->frame_counter = 0;
            return;
        }

        //paddle collision
        if ((int)ball->y == (SCREEN_HEIGHT - 3)) {
            if ((int)ball->x >= game->paddle.x && (int)ball->x <= (game->paddle.x + game->paddle.size)) {

                //advanced paddle bounce
                float speed = sqrtf(ball->vx * ball->vx + ball->vy * ball->vy);
                float center = game->paddle.x + game->paddle.size / 2.0f;
                float offset = (ball->x - center) / (game->paddle.size / 2.0f);

                //limit
                if (offset < -1.0f) {
                    offset = -1.0f;
                }
                if (offset > 1.0f) {
                    offset = 1.0f;
                }

                float angle = offset * MAX_ANGLE;
                ball->vx = speed * sinf(angle);
                ball->vy = -speed * cosf(angle);
            }
        }

        //brick collision
        //create the indexes of the bricks that the ball is inside of.
        int brick_y = ((int)ball->y - 1) / BRICK_HEIGHT;
        int brick_x = ((int)ball->x - 1) / BRICK_WIDTH;

        if (brick_y < BRICK_ROWS && brick_x < BRICK_COLUMNS) {//only start if statement if the ball is inside a rendered or unrendered brick.

            if (game->brick[brick_y][brick_x].health > 0) {//only start if statement if the brick is alive
                game->brick[brick_y][brick_x].health -= 1;
                game->score += advance_score_calc(brick_y); //add score based on the row of the brick that was destroyed.
                //capsules in bricks
                if (game->brick[brick_y][brick_x].contains_capsule && game->next_capsule < MAX_CAPSULES_PER_LEVEL) {
                    game->capsule[game->next_capsule].x = brick_x * BRICK_WIDTH + BRICK_WIDTH / 2;
                    game->capsule[game->next_capsule].y = brick_y * BRICK_HEIGHT + BRICK_HEIGHT / 2;

                    game->capsule[game->next_capsule].vy = 0.5f;
                    game->next_capsule ++;
                }


                //start collision test
                float previous_ball_x = ball->x - ball->vx;
                float previous_ball_y = ball->y - ball->vy;

                int brick_left_limit = 1 + brick_x * BRICK_WIDTH;
                int brick_right_limit = brick_left_limit + BRICK_WIDTH - 1;

                int brick_top_limit = 1 + brick_y * BRICK_HEIGHT;
                int brick_bottom_limit = brick_top_limit + BRICK_HEIGHT - 1;

                if (previous_ball_x < brick_left_limit || previous_ball_x > brick_right_limit) {
                    // Hit the side
                    ball->vx *= -1;
                }
                if (previous_ball_y < brick_top_limit || previous_ball_y > brick_bottom_limit) {
                    // Hit the top or bottom
                    ball->vy *= -1;
                }
            }
        }
    }
}

void paddle_update(Game *game, int c)
{
    if ((c == 'a'|| c == 'A' || c == KEY_LEFT) && game->paddle.x > 1)
    {
        game->paddle.x -= 1; // Move paddle left
    }
    else if ((c == 'd' || c =='D' || c == KEY_RIGHT) && game->paddle.x < (SCREEN_WIDTH - game->paddle.size - 1))
    {
        game->paddle.x += 1; // Move paddle right
    }

}




float enlarge_timer;
float slow_timer;
//MAKE SURE TO ADD THEM TO THE VARIABLE POOL


void activate_powerup (Game *game, int k) {
    switch (game -> capsule[k].type) {
        case ENLARGE:
            game -> paddle.size += 4;
            game -> timer.enlarge = ENLARGE_DURATION; // 10 seconds
            break;

        case SLOW:
            for (int i = 0; i < game->active_balls; i++) {
                game->ball[i].vx *= 0.5f;
                game->ball[i].vy *= 0.5f;
            }
            game -> timer.slow = SLOW_DURATION;
            break;


        case BREAK:
            int i, j;
            game -> score += (game -> combined_brick_health);
            for (i = 0; i < BRICK_ROWS; i++) {
                for (j = 0; j < BRICK_COLUMNS; j++) {
                    game -> brick[i][j].health = 0;
                }
            }
            break;

        case DISRUPTION:
            if (game->active_balls == 0) {
                game->active_balls = 1;
            }

            if (game->active_balls >= MAX_BALLS) {
                break;
            }

            {
                Ball base_ball = game->ball[0];
                float speed = hypotf(base_ball.vx, base_ball.vy);
                float angle = atan2f(base_ball.vy, base_ball.vx);

                if (game->active_balls == 1) {
                    game->ball[1] = base_ball;
                    game->ball[1].vx = speed * cosf(angle - 0.5f);
                    game->ball[1].vy = speed * sinf(angle - 0.5f);

                    game->ball[2] = base_ball;
                    game->ball[2].vx = speed * cosf(angle + 0.5f);
                    game->ball[2].vy = speed * sinf(angle + 0.5f);

                    game->active_balls = 3;
                }
                else if (game->active_balls == 2) {
                    game->ball[2] = base_ball;
                    game->ball[2].vx = speed * cosf(angle + 0.5f);
                    game->ball[2].vy = speed * sinf(angle + 0.5f);
                    game->active_balls = 3;
                }
            }
            break;

        case LASER:
            break;

        case LIFE:
            game->lives++;
            break;
    }
}



void capsule_update(Game *game) {
    int k;
    for (k = 0; k < MAX_CAPSULES_PER_LEVEL; k++) {
        if (game->capsule[k].vy > 0) {
            game->capsule[k].y += game->capsule[k].vy;
            if (game->capsule[k].y >= SCREEN_HEIGHT){
                game->capsule[k].vy = 0;
                game->capsule[k].y = 0;
            }
        }
        if ((int)game->capsule[k].y == (SCREEN_HEIGHT - 2)) {
            if ((int)game->capsule[k].x >= game->paddle.x && (int)game->capsule[k].x <= (game->paddle.x + game -> paddle.size)) {
                game->capsule[k].vy = 0;
                game->capsule[k].y = 0;
                activate_powerup(game, k);


            }
        }
    }
}


void timers_update(Game *game)
{
    if (game->timer.enlarge > 0)
    {
        game->timer.enlarge--;

        if (game->timer.enlarge == 0)
            game->paddle.size = PADDLE_WIDTH_0;
    }

    if (game->timer.slow > 0)
    {
        game->timer.slow--;

        if (game->timer.slow == 0)
        {
            for (int i = 0; i < game->active_balls; i++) {
                game->ball[i].vx *= 2.0f;
                game->ball[i].vy *= 2.0f;
            }
        }
    }
}

