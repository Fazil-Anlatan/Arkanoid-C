#include "application_state.h"

#include <string.h>

#include "established_parameters.h"
#include "game_data.h"
#include "initialization_structures.h"

static int selected = 0;

// ----- Menus ---------
/* Here is the undelying structure of the menu system.
Each menu is a table of rows, "selected" remembers which row is lit up, and
enter_state() is the only place that switches screens so the highlight always
resets. The name is typed one letter per loop instead of using getnstr(),
because it would freeze the whole program until Enter is pressed */

// This is the first menu that comes up, play brings the username input menu up
static const MenuItem intro_menu[] = {
    {"Play", STATE_USERNAME},  // "Play" brings the name screen first
    {"Quit", STATE_QUIT}};
#define INTRO_MENU_COUNT (sizeof(intro_menu) / sizeof(intro_menu[0]))

static const MenuItem username_menu[] = {
    {"Play", STATE_PLAYING},  // "play" starts the game fr
    {"Quit", STATE_QUIT}};
#define USERNAME_MENU_COUNT (sizeof(username_menu) / sizeof(username_menu[0]))

// Scoreboard system
static const MenuItem pause_menu[] = {
    {"Continue", STATE_PLAYING},
    {"Quit", STATE_QUIT},
    {"Scoreboard", STATE_SCOREBOARD},
};
#define PAUSE_MENU_COUNT (sizeof(pause_menu) / sizeof(pause_menu[0]))

// Restarting Menu
static const MenuItem gameover_menu[] = {{"Play again", STATE_PLAYING},
                                         {"Quit", STATE_QUIT}};
#define GAMEOVER_MENU_COUNT (sizeof(gameover_menu) / sizeof(gameover_menu[0]))

static const MenuItem scoreboard_menu[] = {{"Back", STATE_PAUSED}};
#define SCOREBOARD_MENU_COUNT \
  (sizeof(scoreboard_menu) / sizeof(scoreboard_menu[0]))
// Playing and quit states doesnt have a menu naturally
const MenuItem* active_menu(GameState_t game_state, int* count) {
  switch (game_state) {
    case STATE_INTRO:
      *count = INTRO_MENU_COUNT;
      return intro_menu;
    case STATE_USERNAME:
      *count = USERNAME_MENU_COUNT;
      return username_menu;
    case STATE_PAUSED:
      *count = PAUSE_MENU_COUNT;
      return pause_menu;
    case STATE_SCOREBOARD:
      *count = SCOREBOARD_MENU_COUNT;
      return scoreboard_menu;
    case STATE_GAMEOVER:
      *count = GAMEOVER_MENU_COUNT;
      return gameover_menu;
    default:
      *count = 0;
      return NULL;
  }
}

int active_selection() { return selected; }

// STATE CHANGES

void enter_state(GameState_t* game_state, PlayerStats_t* stats,
                 Objects_t* objects, Timer_t* timer, GameState_t next) {
  if (next == STATE_PLAYING && *game_state == STATE_USERNAME &&
      stats->username[0] == '\0') {
    strcpy(stats->username, DEFAULT_USERNAME);
  }
  if (next == STATE_PLAYING && *game_state == STATE_GAMEOVER) {
    reset_game(stats, objects, timer, NULL);
  }

  *game_state = next;
  selected = 0;
}

// MENU INPUTS

static void menu_navigate(GameState_t* game_state, PlayerStats_t* stats,
                          Objects_t* objects, Timer_t* timer, int c,
                          const MenuItem* items, int count) {
  if (c == KEY_UP) {
    selected = (selected - 1 + count) % count;
  } else if (c == KEY_DOWN) {
    selected = (selected + 1) % count;
  } else if (c == '\n' || c == '\r' || c == KEY_ENTER) {
    enter_state(game_state, stats, objects, timer, items[selected].target);
  }
}

static void username_type(PlayerStats_t* stats, int c) {
  int len = (int)strlen(stats->username);

  // Backspace arrives with three different codes depending on the terminal.
  if ((c == KEY_BACKSPACE || c == 127 || c == 8) && len > 0) {
    stats->username[len - 1] = '\0';
  } else if (c > 32 && c <= 126 && len < MAX_USERNAME - 1) {
    stats->username[len] = (char)c;
    stats->username[len + 1] = '\0';  // keep the string terminated
  }
}

void app_state_update(GameState_t* game_state, PlayerStats_t* stats,
                      Objects_t* objects, Timer_t* timer, int c) {
  int count;
  const MenuItem* menu = active_menu(*game_state, &count);

  if (c == ERR || menu == NULL) {
    return;
  }

  if (*game_state == STATE_USERNAME) {
    username_type(stats, c);
  }

  menu_navigate(game_state, stats, objects, timer, c, menu, count);
}