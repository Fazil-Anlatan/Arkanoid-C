#include "application_state.h"
#include "established_parameters.h"
#include "game_data.h"
#include "initialization_structures.h"

//-------------LOCAL FUCTION DECLARATIONS------------
void print_info(const PlayerStats_t* stats, const Timer_t* timer);
//------- RENDERS -------

void set_blank_screen(const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH]) {
  int i, j;
  for (i = 0; i < SCREEN_HEIGHT; i++) {
    for (j = 0; j < SCREEN_WIDTH; j++) {
      screen[i][j] = " ";
    }
  }
}

void set_borders(const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH]) {
  // set roof
  int i;
  for (i = 1; i < SCREEN_WIDTH - 1; i++) {
    screen[0][i] = "═";
  }

  // set upper corners
  screen[0][0] = "╔";
  screen[0][SCREEN_WIDTH - 1] = "╗";

  // set sides
  int k;
  for (k = 1; k < SCREEN_HEIGHT; k++) {
    screen[k][0] = "║";
    screen[k][SCREEN_WIDTH - 1] = "║";
  }

  // Bottom will remain uncovered
}

void set_paddle(const Objects_t* objects,
                const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH]) {
  int i;
  for (i = 0; i < objects->paddle.size; i++) {
    int px = objects->paddle.x + i;
    screen[SCREEN_HEIGHT - 2][px] = "▀";
  }
}

void set_ball(const Objects_t* objects,
              const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH]) {
  int i;
  for (i = 0; i < objects->active_balls; i++) {
    screen[(int)objects->ball[i].y][(int)objects->ball[i].x] = "●";
  }
}

void set_bricks(const Objects_t* objects) {
  int i, j;
  for (i = 0; i < objects->rows_for_level; i++) {
    for (j = 0; j < BRICK_COLUMNS; j++) {
      if (objects->brick[i][j].health > 0) {
        int start_y = 1 + i * BRICK_HEIGHT;
        int start_x = 1 + j * BRICK_WIDTH;

        // turn on the color for this brick
        attron(COLOR_PAIR(objects->brick[i][j].color));

        int k, l;
        for (k = 0; k < BRICK_HEIGHT; k++) {
          for (l = 0; l < BRICK_WIDTH; l++) {
            mvprintw(start_y + k, start_x + l, "─");
          }
        }
        mvprintw(start_y, start_x, "┌");
        mvprintw(start_y, start_x + BRICK_WIDTH - 1, "┐");
        mvprintw(start_y + BRICK_HEIGHT - 1, start_x, "└");
        mvprintw(start_y + BRICK_HEIGHT - 1, start_x + BRICK_WIDTH - 1, "┘");

        // turn off the color for this brick
        attroff(COLOR_PAIR(objects->brick[i][j].color));
      }
    }
  }
}
void set_capsule(const Objects_t* objects) {
  int k;
  for (k = 0; k < MAX_CAPSULES_PER_LEVEL; k++) {
    if (objects->capsule[k].vy > 0) {
      attron(COLOR_PAIR(objects->capsule[k].type + 1));
      mvprintw((int)objects->capsule[k].y, objects->capsule[k].x, "<");
      mvprintw((int)objects->capsule[k].y, objects->capsule[k].x + 1, ">");
      attroff(COLOR_PAIR(objects->capsule[k].type + 1));
    }
  }
}

void draw_all(const PlayerStats_t* stats, const Objects_t* objects,
              const Timer_t* timer,
              const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH]) {
  // set screen
  set_blank_screen(screen);
  set_borders(screen);
  set_paddle(objects, screen);
  set_ball(objects, screen);
  print_info(stats, timer);

  // print screen
  int i, j;
  for (i = 0; i < SCREEN_HEIGHT; i++) {
    for (j = 0; j < SCREEN_WIDTH; j++) {
      mvprintw(i, j, "%s", screen[i][j]);
    }
  }
  // re-writes the bricks with colors, because the above code doesn't support
  // colors. The color is stored in the brick structure.
  set_bricks(objects);
  set_capsule(objects);
  refresh();  // updates what's shown on console
}

void print_info(const PlayerStats_t* stats, const Timer_t* timer) {
  // in order to avoid magic number when printing blank space, we need to
  // incorporate snprintf() plus, with this implementation we can do
  // right-justified text, which just looks way better.

  char buffer[TIMER_BOX_WIDTH];
  int start_column = SCREEN_WIDTH - TIMER_BOX_WIDTH - 1;

  mvprintw(SCREEN_HEIGHT, 0,
           "╔════════════════════════════════════════════════╗");

  // the blank spaces to be printed after the variables are the same, since
  // score, lives and level have the same amount of letters and they have the
  // same characters before and after.

  int blank_space = SCREEN_WIDTH - 9;

  mvprintw(SCREEN_HEIGHT + 1, 0, "║Score: %-*d║", blank_space, stats->score);
  mvprintw(
      SCREEN_HEIGHT + 2, 0, "║Lives: %-*c║", blank_space,
      ' ');  // prints blank space for correct rending of hearts and slow timer
  int i;
  for (i = 0; i < stats->lives; i++) {
    mvprintw(SCREEN_HEIGHT + 2, 8 + (2 * i), "♥ ");
  }

  mvprintw(SCREEN_HEIGHT + 3, 0, "║Level: %-*d║", blank_space, stats->level);
  mvprintw(SCREEN_HEIGHT + 4, 0,
           "╚════════════════════════════════════════════════╝");

  if (timer->enlarge > 0) {
    snprintf(buffer, sizeof(buffer), "Enlarge: %.1fs",
             (float)timer->enlarge / FPS);
  } else {
    buffer[0] = '\0';  // character that represents end of string
  }
  mvprintw(SCREEN_HEIGHT + 1, start_column, "%*s", TIMER_BOX_WIDTH, buffer);

  if (timer->slow > 0) {
    snprintf(buffer, sizeof(buffer), "Slow: %.1fs", (float)timer->slow / FPS);
  } else {
    buffer[0] = '\0';
  }
  mvprintw(SCREEN_HEIGHT + 2, start_column, "%*s", TIMER_BOX_WIDTH, buffer);
}

//------- END RENDER -------
//---------GAME OVER ANIMATION
void draw_game_over(int frame_counter) {
  clear();

  // Titileo: Si la división da par mostramos, si da impar ocultamos.
  // frame_counter / 10 hace que cambie la visibilidad cada 250ms aprox.
  if ((frame_counter / 10) % 2 == 0) {
    const char* art_game[5] = {
        "[][][]   [][]  []     [] [][][]", "[]      []  [] [][] [][] []    ",
        "[] [][] [][][] []  [] [] [][][]", "[]  []  []  [] []     [] []    ",
        "[][][]  []  [] []     [] [][][]"

    };

    const char* art_over[5] = {
        "[][][] []  [] [][][] [][][]", "[]  [] []  [] []     []  []",
        "[]  [] []  [] [][][] [][][]", "[]  []  [][]  []     [] [] ",
        "[][][]   []   [][][] []  []"};

    // Centrado en la pantalla (SCREEN_WIDTH=50, SCREEN_HEIGHT=48)
    for (int i = 0; i < 5; i++) {
      mvprintw(15 + i, 10, "%s", art_game[i]);
      mvprintw(22 + i, 11, "%s", art_over[i]);
    }
  }

  refresh();
}

// MENU SCREENS
//  Pure front-end: these ask application_state.c what to show and turn the
//  answer into characters. They decide nothing.

// name of the game
#define TITLE_ROWS 5
#define TITLE_WIDTH 34
static const char* title_art[TITLE_ROWS] = {
    "████ █    ████ █  █ ████ ████ ███ ", "█    █    █  █ ██ █ █  █  ██  █  █",
    "█    █    █  █ █ ██ █  █  ██  █  █", "█    █    █  █ █  █ █  █  ██  █  █",
    "████ ████ ████ █  █ ████ ████ ███ "};

// menu renderer
static void draw_menu_items(const MenuItem* items, int count, int y, int x) {
  int i;
  int sel = active_selection();

  if (items == NULL) {
    return;
  }

  for (i = 0; i < count; i++) {
    if (i == sel) {
      attron(A_REVERSE);
    }

    mvprintw(y + i * 2, x, "  %-12s  ", items[i].label);
    if (i == sel) {
      attroff(A_REVERSE);
    }
  }
}
void draw_intro_animated(GameState_t game_state, int frame_counter) {
    erase();

    int target_y = 6;
    int center_x = (SCREEN_WIDTH - TITLE_WIDTH) / 2;

    //animation for the title (falling movement)
    for (int i = 0; i < TITLE_ROWS; i++) {
        // delay for the new line
        int line_delay = i * 8; 
        
        if (frame_counter > line_delay) {
            // calculates de position of the title from y=0 up to target_y + i
            int current_y = (frame_counter - line_delay) / 2;
            if (current_y > target_y + i) {
                current_y = target_y + i;
            }
            mvprintw(current_y, center_x, "%s", title_art[i]);
        }
    }

    // now the msubtitle appears (waiting for te animation to finish)
    if (frame_counter > 50) {
        mvprintw(13, 14, "a brick breaker in C");
        mvprintw(15, 8, "──────────────────────────────────");
    }

    // the paddle moves 
    int paddle_x = (frame_counter / 2) % (SCREEN_WIDTH - 8);
    mvprintw(25, paddle_x + 1, "══════");

   
    // now the menu appears (delayed)
    if (frame_counter > 70) {
        int count;
        const MenuItem *menu = active_menu(game_state, &count);
        draw_menu_items(menu, count, 18, 17);

        if ((frame_counter / 15) % 3 >= 1) { //blinking text
            mvprintw(28, 6, "Up / Down to move, Enter to select");
        }
    }

    refresh();
}

void draw_username(GameState_t game_state, const PlayerStats_t* stats) {
  int count;
  const MenuItem* menu = active_menu(game_state, &count);

  erase();

  mvprintw(6, 16, "WHO IS PLAYING?");
  mvprintw(8, 8, "──────────────────────────────────");
  mvprintw(11, 12, "Name: %s_", stats->username);
  mvprintw(13, 12, "type it, Backspace to fix");

  draw_menu_items(menu, count, 18, 17);

  mvprintw(28, 6, "Leave it empty and you are %s", DEFAULT_USERNAME);
  refresh();
}

void draw_scoreboard(GameState_t game_state) {
  int count, i;
  const MenuItem* menu = active_menu(game_state, &count);

  ScoreEntry_t top[MAX_SCORES];
  int n = load_scores(top, MAX_SCORES);

  erase();

  mvprintw(6, 17, "TOP %d SCORES", MAX_SCORES);
  mvprintw(8, 8, "──────────────────────────────────");

  if (n == 0) {
    mvprintw(13, 14, "No games played yet");
  } else {
    for (i = 0; i < n; i++) {
      mvprintw(11 + i * 2, 9, "%d. %-16s %5d  (lvl %d)", i + 1, top[i].name,
               top[i].score, top[i].level);
    }
  }

  draw_menu_items(menu, count, 24, 17);

  mvprintw(28, 6, "Enter to go back");
  refresh();
}

// Unlike the other two this one is an overlay: it redraws the frozen board
// first and then paints a panel on top, so the player keeps their read on where
// the ball and paddle were. draw_all() ends with its own refresh(); the second
// refresh() below is what shows the panel, and curses only pushes changed
// cells.
void draw_pause(GameState_t game_state, const PlayerStats_t* stats,
                const Objects_t* objects, const Timer_t* timer,
                const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH]) {
  int count, i, j;
  const MenuItem* menu = active_menu(game_state, &count);

  int top = 13;
  int left = 7;
  int width = 36;
  int height = 20;

  draw_all(stats, objects, timer, screen);  // the frozen game underneath

  for (i = 1; i < height - 1; i++) {
    for (j = 1; j < width - 1; j++) {
      mvprintw(top + i, left + j, " ");
    }
  }

  for (j = 1; j < width - 1; j++) {
    mvprintw(top, left + j, "═");
    mvprintw(top + height - 1, left + j, "═");
  }
  for (i = 1; i < height - 1; i++) {
    mvprintw(top + i, left, "║");
    mvprintw(top + i, left + width - 1, "║");
  }
  mvprintw(top, left, "╔");
  mvprintw(top, left + width - 1, "╗");
  mvprintw(top + height - 1, left, "╚");
  mvprintw(top + height - 1, left + width - 1, "╝");

  mvprintw(top + 2, left + 15, "PAUSED");

  mvprintw(top + 4, left + 2, "%s!", stats->username);
  mvprintw(top + 5, left + 2, "You are at the level %d and you", stats->level);
  mvprintw(top + 6, left + 2, "have %d lives left, get back to", stats->lives);
  mvprintw(top + 7, left + 2, "your ship and continue bouncing!");

  draw_menu_items(menu, count, top + 10, left + 9);

  mvprintw(top + height - 3, left + 4, "P or Continue to resume");
  refresh();
}

void draw_game_over_stats(GameState_t game_state, const PlayerStats_t* stats) {
  int count;
  const MenuItem* menu = active_menu(game_state, &count);

  erase();

  mvprintw(6, 18, "GAME OVER");
  mvprintw(8, 8, "──────────────────────────────────");
  mvprintw(12, 12, "%s", stats->username);
  mvprintw(14, 12, "Score: %d", stats->score);
  mvprintw(15, 12, "Level: %d", stats->level);

  draw_menu_items(menu, count, 19, 17);

  mvprintw(28, 6, "Up / Down to move, Enter to select");
  refresh();
}

//------- END MENU SCREENS -------
