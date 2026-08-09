#include <string.h>
#include "established_parameters.h"
#include "structures.h"
#include "application_state.h"


extern GameState game_state;
extern char username[MAX_USERNAME];

static int selected = 0;

//MENUS

static const MenuItem intro_menu[] = {
    { "Play", STATE_USERNAME }, // "Play" brings the name screen first
    { "Quit", STATE_QUIT     }
};
#define INTRO_MENU_COUNT (sizeof(intro_menu) / sizeof(intro_menu[0]))

static const MenuItem username_menu[] = {
    { "Play", STATE_PLAYING }, // "play" starts the game fr
    { "Quit", STATE_QUIT    }
};
#define USERNAME_MENU_COUNT (sizeof(username_menu) / sizeof(username_menu[0]))

// Scoreboard placeholder till implimented
static const MenuItem pause_menu[] = {
    { "Continue",   STATE_PLAYING },
    { "Quit",       STATE_QUIT    },
    { "Scoreboard", STATE_PAUSED  }
};
#define PAUSE_MENU_COUNT (sizeof(pause_menu) / sizeof(pause_menu[0]))

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
        default: // STATE_PLAYING and STATE_QUIT have no menu
            *count = 0;
            return NULL;
    }
}

int active_selection() {
    return selected;
}

//STATE CHANGES

void enter_state(GameState next) {

 
    if (next == STATE_PLAYING && game_state == STATE_USERNAME && username[0] == '\0') {
        strcpy(username, DEFAULT_USERNAME);
    }

    game_state = next;
    selected = 0;
}

//MENU INPUTS

static void menu_navigate(int c, const MenuItem *items, int count) {

    if (c == KEY_UP) {
        selected = (selected - 1 + count) % count;
    }
    else if (c == KEY_DOWN) {
        selected = (selected + 1) % count;
    }
    else if (c == '\n' || c == '\r' || c == KEY_ENTER) {
        enter_state(items[selected].target);
    }
}


static void username_type(int c) {

    int len = (int) strlen(username);

    // Backspace arrives with three different codes depending on the terminal.
    if ((c == KEY_BACKSPACE || c == 127 || c == 8) && len > 0) {
        username[len - 1] = '\0';
    }
    else if (c > 32 && c <= 126 && len < MAX_USERNAME - 1) {
        username[len] = (char) c;
        username[len + 1] = '\0'; // keep the string terminated
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