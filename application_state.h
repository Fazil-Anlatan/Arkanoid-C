#ifndef APP_STATE
#define APP_STATE
#include "structures.h"

// Function Declarations 

void enter_state(GameState next);   // changes screen
void app_state_update(int c);    // feeds a key to the active menu
const MenuItem *active_menu(int *count); // menu of the current state, NULL if none
int active_selection();  // highlighted row, for render.c
#endif  