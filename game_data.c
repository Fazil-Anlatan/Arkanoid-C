#include "game_data.h"

#include <math.h>
#include <stdio.h>

#include "application_state.h"
#include "established_parameters.h"
#include "initialization_structures.h"
#include "physics.h"

//-------------GAME STATE INITIALIZATION------------
// These used to be global variable declarations with their initial values.
// Now that they live inside the Game struct, they're set here once instead.
void game_data_init(GameState_t* game_state, PlayerStats_t* stats,
                    Objects_t* objects) {
  *game_state =
      STATE_INTRO;  // was: short int = 1. Starts at the title screen now.
  stats->username[0] = '\0';  // for the top score file later
  stats->score = 0;
  stats->lives = 3;
  stats->level = 1;
  objects->rows_for_level = INITIAL_BRICK_ROWS;
}

int score_calc(PlayerStats_t* stats, Objects_t* objects, int brick_x,
               int brick_y, Ball_t* hitting_ball) {  // for bonus
  // how do we calculate this
  Brick_t* brick = &objects->brick[brick_y][brick_x];

  // vector speed of the ball hitting the brick. Not all balls in the same level
  // have the same vector speed because of disruption. Disruption makes balls
  // faster or slower, depending on the angle, and does not mantain vector
  // velocity across all balls.
  float speed = sqrtf(pow(hitting_ball->vx, 2) + pow(hitting_ball->vy, 2));

  // Base score tied to color
  int base_score;
  if (brick->color <= 6) {
    base_score = (int)(50 + (10 * (brick->color - 1)));
  }

  // this happens if the color is white, which requires 2 hits to break
  else {
    base_score = 50 * ((stats->level / 8) + 1);
  }

  // kill to birds with one stone. Velocity mult will take into account levels,
  // since speed increases as levels do.
  float speed_bonus_mult = speed / INITIAL_BALL_SPEED;
  if (speed_bonus_mult > 1)
    return (int)(((float)base_score) * speed_bonus_mult);

  else {
    return base_score;
  }
}

//------- LEVEL UP -------
void level_up(PlayerStats_t* stats, Objects_t* objects, Timer_t* timer, int c) {
  // if total active bricks is 0, level up.

  if ((objects->total_active_bricks == 0) ||
      (((c == 'l') || (c == 'L')) && DEVELOPER_TOOLS_ACTIVE)) {
    objects->total_active_bricks = 0;

    // Multi-ball multiplier logic
    if (objects->active_balls > 1) {
      stats->score += (stats->level_score * (objects->active_balls - 1));
    }

    // Reset the tracker for the next stage
    stats->level_score = 0;

    // reset paddle size
    objects->paddle.size = PADDLE_WIDTH_0;

    stats->level++;
    gameInicialization_timers(timer);
    gameInicialization_bricks(stats, objects);
    gameInicialization_ball(stats, objects);
    gameInicialization_capsules(objects);
    if (stats->level > 3 && objects->rows_for_level < MAX_BRICK_ROWS)
      objects->rows_for_level++;
  }
}

//------- END LEVEL UP -------

//------- Restart Function ----------
// basically resets the game, rebuilds the game board and stats as it was in the
// starting values and positions

void reset_game(PlayerStats_t* stats, Objects_t* objects, Timer_t* timer,
                int* frame_counter) {
  stats->score = 0;
  stats->lives = 3;
  stats->level = 1;
  if (frame_counter != NULL) {
    *frame_counter = 0;
  }
  objects->total_active_bricks = 0;
  objects->rows_for_level = INITIAL_BRICK_ROWS;
  gameInicialization(stats, objects, timer);
}
//------- End Restart ---------
//----- History Log ------
// adds one line to scores.txt every time a game ends. Format is: username score
// level

void save_score(const PlayerStats_t* stats) {
  FILE* fp;

  fp = fopen("scores.txt", "a");  // "a" = adding mode

  if (fp ==
      NULL) {  // fopen returns NULL if it failed so the game never crashes
    return;
  }

  fprintf(fp, "%s %d %d\n", stats->username, stats->score, stats->level);

  fclose(fp);  // flushes the text to disk
}
// Reads scores.txt, sorts it best-first, and copies the top "max" lines into
// top[]

#define MAX_SAVED_SCORES 100  // safety cap

int load_scores(ScoreEntry_t top[], int max) {
  FILE* fp;
  ScoreEntry_t all[MAX_SAVED_SCORES];
  ScoreEntry_t temp;
  int count = 0;
  int i, j;

  fp = fopen("scores.txt", "r");

  if (fp == NULL) {  // when no score file has been cretaed yet
    return 0;
  }

  // one line = name, score, level. fscanf returns 3 when all three were read
  while (count < MAX_SAVED_SCORES &&
         fscanf(fp, "%15s %d %d", all[count].name, &all[count].score,
                &all[count].level) == 3) {
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