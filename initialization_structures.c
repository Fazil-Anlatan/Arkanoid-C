#include "initialization_structures.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "established_parameters.h"
// Initial values for game objects

void gameInicialization(PlayerStats_t* stats, Objects_t* objects,
                        Timer_t* timer) {
  gameInicialization_ball(stats, objects);
  gameInicialization_paddle(objects);
  gameInicialization_bricks(stats, objects);
  gameInicialization_capsules(objects);
  gameInicialization_timers(timer);
}

void gameInicialization_ball(PlayerStats_t* stats,
                             Objects_t* objects) {  // isn't finished yed
  time_t now = time(NULL);
  int i, j;
  float ball_speed_for_level = INITIAL_BALL_SPEED;
  if (stats->level <= MAX_LEVEL_FOR_SPEED_UP) {
    for (j = 0; j < (int)(stats->level / 4); j++) {
      ball_speed_for_level += 0.03;
    }
  } else {
    ball_speed_for_level = MAX_BALL_SPEED;
  }

  for (i = 0; i < MAX_BALLS; i++) {
    objects->ball[i].x = SCREEN_WIDTH / 2;
    objects->ball[i].y = SCREEN_HEIGHT / 2;
    objects->ball[i].vx = 0.0f;
    objects->ball[i].vy = 0.0f;
  }

  objects->ball[0].vx =
      ((now % 5) + 1) / 10.0f;  // x velocity between 0.1 and 0.5

  // Total initial velocity is a vector of 0.75
  // Calculated using the pythagorean theorem.
  //
  // After, the velocity will increase with the level
  // and will also be calculated with pythagoras.
  objects->ball[0].vy =
      -(sqrt(pow(ball_speed_for_level, 2) -
             pow(objects->ball[0].vx, 2)));  // We need to make another constant
                                             // for the length of the vector.

  // random directions
  if (now % 2 == 1) {
    objects->ball[0].vx *= -1;
  }

  objects->active_balls = 1;
}

void gameInicialization_paddle(Objects_t* objects) {
  objects->paddle.x = (int)(SCREEN_WIDTH / 2);
  objects->paddle.size = PADDLE_WIDTH_0;
}

void gameInicialization_bricks(PlayerStats_t* stats, Objects_t* objects) {
  int i, j;

  switch (stats->level) {
    case 1:
      for (i = 0; i < INITIAL_BRICK_ROWS; i++) {
        for (j = 0; j < BRICK_COLUMNS; j++) {
          if ((j + 1) % 3) {
            objects->brick[i][j].health = 1;
            objects->brick[i][j].contains_capsule = !(rand() % 10);
            objects->brick[i][j].color = (i % 6) + 1;
            objects->total_active_bricks += 1;
          } else {
            objects->brick[i][j].health = 0;
          }
        }
      }

      break;

    case 2:  // inverted triangle of bricks
      for (i = 0; i < INITIAL_BRICK_ROWS; i++) {
        for (j = 0; j < BRICK_COLUMNS; j++) {
          if (j >= i && j < BRICK_COLUMNS - i) {
            objects->brick[i][j].health = 1;
            objects->brick[i][j].contains_capsule = !(rand() % 10);
            objects->brick[i][j].color = (j % 6) + 1;
            objects->total_active_bricks += 1;
          } else {
            objects->brick[i][j].health = 0;
          }
        }
      }
      break;

    case 3:
      for (i = 0; i < INITIAL_BRICK_ROWS; i++) {  // square of bricks
        for (j = 0; j < BRICK_COLUMNS; j++) {
          if (j > 0 && i > 0 && j < (BRICK_COLUMNS - 1)) {
            objects->brick[i][j].health = 1;
            objects->brick[i][j].contains_capsule = !(rand() % 10);
            objects->brick[i][j].color = ((i+j) % 6) + 1;
            objects->total_active_bricks += 1;
          } else {
            objects->brick[i][j].health = 0;
          }
        }
      }
      break;

    default:  // For levels greater than 3, a random pattern of bricks will be
              // generated
      for (i = 0; i < objects->rows_for_level; i++) {
        for (j = 0; j < BRICK_COLUMNS; j++) {
          objects->brick[i][j].health =
              (rand() % 10 < 7) ? 1 : 0;  // 70 % of chace to generate a brick
          if (objects->brick[i][j].health == 1) {
            objects->brick[i][j].health +=
                (rand() % 20 < 1) ? 1
                                  : 0;  // out of the bricks being generated, 5%
                                        // chance that they will have 2 health.
            objects->brick[i][j].contains_capsule =
                !(rand() % 10);  // 10% chance of containing a capsule
            objects->brick[i][j].color = (rand() % 6) + 1;
            objects->total_active_bricks += 1;
            if (objects->brick[i][j].health == 2) {
              objects->brick[i][j].color = 7;
            }
          }
        }
      }
      break;
  }
}

void gameInicialization_capsules(Objects_t* objects) {
  int i, j, k;
  int disruption = rand() % MAX_CAPSULES_PER_LEVEL;
  objects->next_capsule = 0;
  k = 0;
  for (i = 0; i < objects->rows_for_level; i++) {
    for (j = 0; j < BRICK_COLUMNS; j++) {
      if ((objects->brick[i][j].contains_capsule > 0) &&
          (k < MAX_CAPSULES_PER_LEVEL)) {
        objects->capsule[k].x = 1 + j * BRICK_WIDTH;
        objects->capsule[k].y = 1 + i * BRICK_HEIGHT;
        objects->capsule[k].vy = 0;
        objects->capsule[k].type = DISRUPTION;
        if (k == disruption) {
          objects->capsule[k].type = DISRUPTION;
        } else {
          int type = rand() % 4;

          if (type >= DISRUPTION) type++;

          objects->capsule[k].type = type;
        }

        k++;
      }
    }
  }
}

void gameInicialization_timers(Timer_t* timer) {
  timer->enlarge = 0;
  timer->slow = 0;
}
