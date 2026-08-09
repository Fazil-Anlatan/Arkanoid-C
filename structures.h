
#ifndef STRUCTURES
#define STRUCTURES

//APPLICATION STATES
// Replaces the old "short int game_state" (1 = running, 0 = over), because the menu needs more than two values.
// STATE_INTRO must stay 0 so an uninitialized state means "title screen".

typedef enum {
    STATE_INTRO = 0,   // title screen: Play / Quit
    STATE_USERNAME,    // name entry: Play / Quit
    STATE_PLAYING,     // the game itself
    STATE_PAUSED,      // Continue / Quit / Scoreboard
    STATE_QUIT         // main() leaves its loop
} GameState;

// MENU ROW
// One line of a menu. Label and consequence sit together. A menu is an array of these
typedef struct {
    const char *label;   // text on screen
    GameState   target;  // state we go to when this row is confirmed
} MenuItem;
#endif