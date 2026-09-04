#include <string.h>
#include "established_parameters.h"
#include "structures.h"
#include "application_state.h"
#include "game_data.h"


extern GameState game_state;
extern char username[MAX_USERNAME];

static int selected = 0;

// ----- Menus ---------
/* Here is the undelying structure of the menu system. 
Each menu is a table of rows, "selected" remembers which row is lit up, and enter_state() is the only place that switches screens so the highlight always resets. 
The name is typed one letter per loop instead of using getnstr(), because it would freeze the whole program until Enter is pressed */


// This is the first manu that comes up, play brings the username input menu up
static const MenuItem intro_menu[] = {
    { "Play", STATE_USERNAME }, 
    { "Quit", STATE_QUIT     }
};
#define INTRO_MENU_COUNT (sizeof(intro_menu) / sizeof(intro_menu[0]))


// Pressing play, now starts the game
static const MenuItem username_menu[] = {
    { "Play", STATE_PLAYING }, 
    { "Quit", STATE_QUIT    }
};
#define USERNAME_MENU_COUNT (sizeof(username_menu) / sizeof(username_menu[0]))


//Scoreboard system 
static const MenuItem pause_menu[] = {
    { "Continue",   STATE_PLAYING    },
    { "Quit",       STATE_QUIT       },
    { "Scoreboard", STATE_SCOREBOARD },
};
#define PAUSE_MENU_COUNT (sizeof(pause_menu) / sizeof(pause_menu[0]))

// Restarting Menu 
static const MenuItem gameover_menu[] = {
    { "Play again", STATE_PLAYING },
    { "Quit",       STATE_QUIT    }
};
#define GAMEOVER_MENU_COUNT (sizeof(gameover_menu) / sizeof(gameover_menu[0]))


static const MenuItem scoreboard_menu[] = {
    { "Back", STATE_PAUSED }
};
#define SCOREBOARD_MENU_COUNT (sizeof(scoreboard_menu) / sizeof(scoreboard_menu[0]))

//Playing and quit states doesnt have a menu naturally
const MenuItem *active_menu(int *count) {
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

int active_selection() {
    return selected;
}

// State Changes

void enter_state(GameState next) {

 
    if (next == STATE_PLAYING && game_state == STATE_USERNAME && username[0] == '\0') {
        strcpy(username, DEFAULT_USERNAME);
    }
    if (next == STATE_PLAYING && game_state == STATE_GAMEOVER) {
        reset_game();
    }
    game_state = next;
    selected = 0;
}

// Menu Inputs


// the + count is needed because in C -1 % 3 gives -1, not 2, and we would read outside the array
static void menu_navigate(int c, const MenuItem *items, int count) {

    if (c == KEY_UP) {
        selected = (selected - 1 + count) % count;
    }
    else if (c == KEY_DOWN) {
        selected = (selected + 1) % count;
    }

    // Enter sends a different code on Linux and on Windows, so we check both
    else if (c == '\n' || c == '\r' || c == KEY_ENTER) {
        enter_state(items[selected].target);
    }
}


static void username_type(int c) {

    int len = (int) strlen(username);

    if ((c == KEY_BACKSPACE || c == 127 || c == 8) && len > 0) {
        username[len - 1] = '\0';
    }
    else if (c > 32 && c <= 126 && len < MAX_USERNAME - 1) {
        username[len] = (char) c;
        username[len + 1] = '\0';
    }
}

void app_state_update(int c) {

    int count;
    const MenuItem *menu = active_menu(&count);

  
    if (c == ERR || menu == NULL) {
        return;
    }

    if (game_state == STATE_USERNAME) {
        username_type(c);
    }

    menu_navigate(c, menu, count);
}