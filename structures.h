
#ifndef STRUCTURES
#define STRUCTURES
#include "established_parameters.h" 

// ----- Application States ----

/* I put the list of screens and the menu row type here. 
Discovered enum`s, they are just names that correspond to a list of numbers, makes the code easier to read and understand i think. */

typedef enum {
    STATE_INTRO = 0,   
    STATE_USERNAME,    
    STATE_PLAYING,    
    STATE_PAUSED,
    STATE_SCOREBOARD,
    STATE_GAMEOVER,
    STATE_QUIT     
} GameState;    


typedef struct {
    const char *label;   
    GameState   target;  
} MenuItem;
#define MAX_SCORES 5   // the scoreboard shows top 5 scores

typedef struct {
    char name[MAX_USERNAME];
    int  score;
    int  level;
} ScoreEntry;


#endif