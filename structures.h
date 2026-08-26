
#ifndef STRUCTURES
#define STRUCTURES

// ----- Application States ----

/* I put the list of screens and the menu row type here. 
Discovered enum`s, they are just names that correspond to a list of numbers, makes the code easier to read and understand i think. */

typedef enum {
    STATE_INTRO = 0,   
    STATE_USERNAME,    
    STATE_PLAYING,    
    STATE_PAUSED,     
    STATE_QUIT     
} GameState;


typedef struct {
    const char *label;   
    GameState   target;  
} MenuItem;
#endif