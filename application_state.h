#ifndef APP_STATE
#define APP_STATE
#include "established_parameters.h"
#include "initialization_structures.h"

// MENU ROW
// One line of a menu. Label and consequence sit together. A menu is an array of
// these
typedef struct {
  const char* label;   // text on screen
  GameState_t target;  // state we go to when this row is confirmed
} MenuItem;

//-----------------FUCTION DECLARATIONS------------
void enter_state(GameState_t* game_state, PlayerStats_t* stats,
                 Objects_t* objects, Timer_t* timer,
                 GameState_t next);  // change screen
void app_state_update(GameState_t* game_state, PlayerStats_t* stats,
                      Objects_t* objects, Timer_t* timer,
                      int c);  // feed a key to the active menu
const MenuItem* active_menu(
    GameState_t game_state,
    int* count);         // menu of the current state, NULL if none
int active_selection();  // highlighted row, for render.c

#endif