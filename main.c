#include <locale.h>  //needed in linux for special characters
#include <stdio.h>
#include <stdlib.h>  //srand() was not working in linnux without this
#include <time.h>

#include "application_state.h"
#include "established_parameters.h"
#include "game_data.h"
#include "initialization_structures.h"
#include "physics.h"
#include "render.h"

int main() {
  srand(time(NULL));  // seed the random number generator for capsules and brick
                      // patterns

  GameState_t game_state;
  PlayerStats_t stats = {0};
  Objects_t objects = {0};
  Timer_t timer = {0};
  int frame_counter = 0;
  const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH];

  game_data_init(&game_state, &stats,
                 &objects);  // sets game_state, username, score, lives and
                             // level to their starting values

  setlocale(LC_ALL, "");  // for the characters
  //------inicialization of ncurses-----
  initscr();

  start_color();  // initialize colors (ncurses)

  // Define color pairs init_pair (number, Text_Color, Background_Color)
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
  init_pair(7, COLOR_WHITE, COLOR_BLACK);

  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  gameInicialization(&stats, &objects, &timer);

  //------- MAIN LOOP -------
  // main() is no longer the game loop, it is a router. One frame is: read a
  // key, hand it to whichever screen is active, draw that screen, sleep.
  while (game_state != STATE_QUIT) {
    // remembered so we can tell at the end of the frame whether this key caused
    // a screen change
    GameState_t previous_state = game_state;

    int c = getch();

    switch (game_state) {
      case STATE_INTRO:
    app_state_update(&game_state, &stats, &objects, &timer, c);
    if (game_state == STATE_INTRO) {
        draw_intro_animated(game_state, frame_counter);
        frame_counter++; 
    } else {
        frame_counter = 0; 
    }
    break;

      case STATE_USERNAME:
        app_state_update(&game_state, &stats, &objects, &timer, c);
        if (game_state == STATE_USERNAME) {
          draw_username(game_state, &stats);
        }
        break;

      case STATE_PLAYING:
        if (c == 'p' || c == 'P') {
          enter_state(&game_state, &stats, &objects, &timer, STATE_PAUSED);
          break;
        }
        draw_all(&stats, &objects, &timer, screen);
        ball_update(&game_state, &stats, &objects, &frame_counter);
        paddle_update(&objects, c);
        capsule_update(&stats, &objects, &timer);
        level_up(&stats, &objects, &timer, c);
        timers_update(&objects, &timer);
        break;

      case STATE_PAUSED:
        if (c == 'p' || c == 'P') {
          enter_state(&game_state, &stats, &objects, &timer, STATE_PLAYING);
        } else {
          app_state_update(&game_state, &stats, &objects, &timer, c);
        }
        if (game_state == STATE_PAUSED) {
          draw_pause(game_state, &stats, &objects, &timer, screen);
        }
        break;

      case STATE_SCOREBOARD:
        app_state_update(&game_state, &stats, &objects, &timer, c);
        if (game_state == STATE_SCOREBOARD) {
          draw_scoreboard(game_state);
        }
        break;

      case STATE_GAMEOVER:
        if (frame_counter <= 120) {
          draw_game_over(frame_counter);  // ANIMATION: 120 frames = 3 seconds
                                          // at 25 ms per frame
          frame_counter++;
        } else {
          draw_game_over_stats(game_state, &stats);  // INFO
          app_state_update(&game_state, &stats, &objects, &timer, c);
        }
        break;

      default:
        break;
    }

    if (game_state != previous_state) {
      if (game_state == STATE_GAMEOVER && stats.score > 0) {
        save_score(&stats);
      }
      flushinp();
    }

    sleep_ms((int)(1000 / FPS));
  }
  endwin();
  printf("Game Over!\n");
}