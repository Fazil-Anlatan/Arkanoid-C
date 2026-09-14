#include "physics.h"

#include <math.h>

#include "established_parameters.h"
#include "game_data.h"
#include "initialization_structures.h"
//------- GAME UPDATES -------

void ball_update(GameState_t* game_state, PlayerStats_t* stats,
                 Objects_t* objects, int* frame_counter) {
  int i;

  for (i = 0; i < objects->active_balls; i++) {
    Ball_t* ball = &objects->ball[i];

    // position update
    ball->x += ball->vx;
    ball->y += ball->vy;

    if ((int)ball->x <= 1) {
      ball->vx = fabsf(ball->vx);
    }

    if ((int)ball->x >= SCREEN_WIDTH - 2) {
      ball->vx = -fabsf(ball->vx);
    }

    if ((int)ball->y <= 1) {
      ball->vy = fabsf(ball->vy);
    }

    if ((int)ball->y >= SCREEN_HEIGHT - 1) {
      if (objects->active_balls > 1) {
        objects->ball[i] = objects->ball[objects->active_balls - 1];
        objects->active_balls--;
        i--;
        continue;
      }
      if (stats->lives > 1) {
        stats->lives--;
        gameInicialization_ball(stats, objects);
        return;
      }
      *game_state = STATE_GAMEOVER;
      if (frame_counter != NULL) {
        *frame_counter = 0;
      }
      return;
    }

    // paddle collision
    if ((int)ball->y == (SCREEN_HEIGHT - 3)) {
      if ((int)ball->x >= objects->paddle.x &&
          (int)ball->x <= (objects->paddle.x + objects->paddle.size)) {
        // advanced paddle bounce
        float speed = sqrtf(ball->vx * ball->vx + ball->vy * ball->vy);
        float center = objects->paddle.x + objects->paddle.size / 2.0f;
        float offset = (ball->x - center) / (objects->paddle.size / 2.0f);

        // limit
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

    // brick collision
    // create the indexes of the bricks that the ball is inside of.
    int brick_y = ((int)ball->y - 1) / BRICK_HEIGHT;
    int brick_x = ((int)ball->x - 1) / BRICK_WIDTH;

    if (brick_y < objects->rows_for_level &&
        brick_x < BRICK_COLUMNS) {  // only start if statement if the ball is
                                    // inside a rendered or unrendered brick.

      if (objects->brick[brick_y][brick_x].health > 0) {
        if (objects->brick[brick_y][brick_x].health == 1) {
          objects->total_active_bricks -= 1;

          // increase both score counts
          int points_earned =
              score_calc(stats, objects, brick_x, brick_y, ball);
          stats->score += points_earned;
          stats->level_score += points_earned;

          // capsules in bricks
          if (objects->brick[brick_y][brick_x].contains_capsule &&
              objects->next_capsule < MAX_CAPSULES_PER_LEVEL) {
            objects->capsule[objects->next_capsule].x =
                brick_x * BRICK_WIDTH + BRICK_WIDTH / 2;
            objects->capsule[objects->next_capsule].y =
                brick_y * BRICK_HEIGHT + BRICK_HEIGHT / 2;

            objects->capsule[objects->next_capsule].vy = 0.35f;
            objects->next_capsule++;
          }
        }

        objects->brick[brick_y][brick_x].health -= 1;

        // start collision test

        float previous_x = ball->x - ball->vx;
        float previous_y = ball->y - ball->vy;
        int left_brick_limit = 1 + brick_x * BRICK_WIDTH;
        int right_brick_limit = left_brick_limit + BRICK_WIDTH - 1;
        int top_brick_limit = 1 + brick_y * BRICK_HEIGHT;
        int bottom_brick_limit = top_brick_limit + BRICK_HEIGHT - 1;


        int hit_x = (previous_x < left_brick_limit && ball->vx > 0) || (previous_x > right_brick_limit && ball->vx < 0);
        int hit_y = (previous_y < top_brick_limit && ball->vy > 0) || (previous_y > bottom_brick_limit && ball->vy < 0);

        if (hit_x ^ hit_y) {
          // = side or pure top/bottom bounce
          if (hit_x) ball->vx *= -1;
          if (hit_y) ball->vy *= -1;
        }
        else if (hit_x && hit_y) {
          // corner hit: The ball is actively moving towards the corner on both axes
          int adjacent_horizontal_brick = brick_x + (previous_x < left_brick_limit ? -1 : 1);
          int adjacent_vertical_brick = brick_y + (previous_y < top_brick_limit ? -1 : 1);

          // adjacent brick health. If its outside the screen, set to 0.
          int adj_h_brick_hp = (adjacent_horizontal_brick >= 0 && adjacent_horizontal_brick < BRICK_COLUMNS) ? objects->brick[brick_y][adjacent_horizontal_brick].health : 0;
          int adj_v_brick_hp = (adjacent_vertical_brick >= 0 && adjacent_vertical_brick < objects->rows_for_level) ? objects->brick[adjacent_vertical_brick][brick_x].health : 0;

          if (adj_h_brick_hp == 0 && adj_v_brick_hp == 0) {//corner available for 180 bounce
            ball->vx = -ball->vx;
            ball->vy = -ball->vy;
          }
          else if (adj_h_brick_hp > 0 && adj_v_brick_hp == 0) {//bottom or top free, but not the side. 90 bounce
            ball->vy = -ball->vy;
          }
          else if (adj_v_brick_hp > 0 && adj_h_brick_hp == 0) {//side free, but not the bottom or top. 90 bounce
            ball->vx = -ball->vx;
          }
          else {//it can squeeze through the gap of the bricks and break the brick surrounded. 180 bounce.
            ball->vx = -ball->vx;
            ball->vy = -ball->vy;
          }
      }
    }
  }
}

void paddle_update(Objects_t* objects, int c) {
  if ((c == 'a' || c == 'A' || c == KEY_LEFT) && objects->paddle.x > 1) {
    objects->paddle.x -= 1;  // Move paddle left
  } else if ((c == 'd' || c == 'D' || c == KEY_RIGHT) &&
             objects->paddle.x < (SCREEN_WIDTH - objects->paddle.size - 1)) {
    objects->paddle.x += 1;  // Move paddle right
  }
}

void activate_powerup(PlayerStats_t* stats, Objects_t* objects, Timer_t* timer,
                      int k) {
  int i, j;  // local variable declaration outside switch statement for good
             // practice.

  switch (objects->capsule[k].type) {
    case ENLARGE:
      objects->paddle.size += 4;
      timer->enlarge += ENLARGE_DURATION;  // 10 seconds
      break;

    case SLOW:
      for (i = 0; i < objects->active_balls; i++) {
        objects->ball[i].vx *= 0.5f;
        objects->ball[i].vy *= 0.5f;
      }
      timer->slow += SLOW_DURATION;  // 5 seconds
      break;

    case BREAK:
      for (i = 0; i < objects->rows_for_level; i++) {
        for (j = 0; j < BRICK_COLUMNS; j++) {
          // Only score and destroy bricks that are actually alive
          if (objects->brick[i][j].health > 0) {
            objects->brick[i][j].health = 0;
            objects->total_active_bricks--;  // Safely count down active bricks

            int points_earned =
                score_calc(stats, objects, j, i,
                           &objects->ball[0]);  // Note: check your param order
                                                // (x, y) vs (y, x)

            stats->score += points_earned;
            stats->level_score += points_earned;
          }
        }
      }
      objects->total_active_bricks =
          0;  // Is this needed? Maybe not. For safety?
      break;

    case DISRUPTION:
      objects->ball[1] = objects->ball[0];
      objects->ball[1].vx = objects->ball[0].vx -
                            0.1f;  // What if I just make them go in the same
                                   // direction but with little variation.

      objects->ball[2] = objects->ball[0];
      objects->ball[2].vx = objects->ball[0].vy + 0.1f;

      objects->active_balls = 3;

      break;

    case LIFE:
      stats->lives++;
      break;
  }
}

void capsule_update(PlayerStats_t* stats, Objects_t* objects, Timer_t* timer) {
  int k;
  for (k = 0; k < MAX_CAPSULES_PER_LEVEL; k++) {
    if (objects->capsule[k].vy > 0) {
      objects->capsule[k].y += objects->capsule[k].vy;
      if (objects->capsule[k].y >= SCREEN_HEIGHT) {
        objects->capsule[k].vy = 0;
        objects->capsule[k].y = 0;
      }
    }
    if ((int)objects->capsule[k].y == (SCREEN_HEIGHT - 2)) {
      if ((int)objects->capsule[k].x >= objects->paddle.x &&
          (int)objects->capsule[k].x <=
              (objects->paddle.x + objects->paddle.size)) {
        objects->capsule[k].vy = 0;
        objects->capsule[k].y = 0;
        activate_powerup(stats, objects, timer, k);
      }
    }
  }
}

void timers_update(Objects_t* objects, Timer_t* timer) {
  if (timer->enlarge > 0) {
    timer->enlarge--;
    if (timer->enlarge % ENLARGE_DURATION == 0) objects->paddle.size -= 4;
  }

  if (timer->slow > 0) {
    timer->slow--;
    if (timer->slow % SLOW_DURATION == 0) {
      for (int i = 0; i < objects->active_balls; i++) {
        objects->ball[i].vx *= 2.0f;
        objects->ball[i].vy *= 2.0f;
      }
    }
  }
}
