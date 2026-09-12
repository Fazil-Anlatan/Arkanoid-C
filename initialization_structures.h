#ifndef INI_STRUCT
#define INI_STRUCT
#include "established_parameters.h"
#define MAX_SCORES 5  // the scoreboard shows top 5 scores
//-------------STRUCTURES-----------
typedef struct {  // ball
  float x, y;
  float vx, vy;
} Ball_t;

typedef struct {  // bricks
  int health;
  int contains_capsule;
  int color;  // defined color for each block
} Brick_t;

typedef struct {  // paddle
  int x;
  int size;
} Paddle_t;

typedef enum { ENLARGE = 0, SLOW, BREAK, DISRUPTION, LIFE } PowerUps_t;

typedef struct {  // capsules
  int x;
  float vy, y;
  PowerUps_t type;
} Capsule_t;

typedef struct {  // timers
  int enlarge;
  int slow;
} Timer_t;

// APPLICATION STATES
typedef enum {
  STATE_INTRO = 0,   // title screen: Play / Quit
  STATE_USERNAME,    // name entry: Play / Quit
  STATE_PLAYING,     // the game itself
  STATE_PAUSED,      // Continue / Quit / Scoreboard
  STATE_GAMEOVER,    // game over animation
  STATE_SCOREBOARD,  // top 5 scores
  STATE_QUIT         // main() leaves its loop
} GameState_t;

typedef struct {
  char username[MAX_USERNAME];
  int score;
  int level_score;  // keeps track of score in the current level.
  short int lives;
  short int level;
} PlayerStats_t;

typedef struct {
  Ball_t ball[MAX_BALLS];
  int active_balls;
  Brick_t brick[MAX_BRICK_ROWS][BRICK_COLUMNS];
  Paddle_t paddle;
  Capsule_t capsule[MAX_CAPSULES_PER_LEVEL];
  int next_capsule;
  int total_active_bricks;
  int rows_for_level;
} Objects_t;

typedef struct {  // score entry for the scoreboard
  char name[MAX_USERNAME];
  int score;
  int level;
} ScoreEntry_t;

//------FUCTION DECLARATIONS-------------
void gameInicialization(
    PlayerStats_t* stats, Objects_t* objects,
    Timer_t* timer);  // Divided into 3 functions: ball, paddle and bricks
void gameInicialization_ball(PlayerStats_t* stats, Objects_t* objects);
void gameInicialization_paddle(Objects_t* objects);
void gameInicialization_bricks(PlayerStats_t* stats, Objects_t* objects);
void gameInicialization_capsules(Objects_t* objects);
void gameInicialization_timers(Timer_t* timer);

#endif