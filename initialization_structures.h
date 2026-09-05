#ifndef INI_STRUCT
#define INI_STRUCT

#define MAX_SCORES 5   // the scoreboard shows top 5 scores

struct Game; /* forward declaration only (defined below) to build the Game struct, so this header can't
              include structures.h itself without creating a circular include. A pointer to an incomplete type is all these declarations need. */

//------FUCTION DECLARATIONS-------------
void gameInicialization(struct Game *game); //Divided into 3 functions: ball, paddle and bricks
void gameInicialization_ball(struct Game *game);
void gameInicialization_paddle(struct Game *game);
void gameInicialization_bricks(struct Game *game);
void gameInicialization_capsules(struct Game *game);
void gameInicialization_timers(struct Game *game);
//-------------STRUCTURES-----------
typedef struct{ //ball
    float x, y;
    float vx, vy;
}Ball;

typedef struct{ //bricks
    int health;
    int contains_capsule;
    int color; //defined color for each block
}Brick;

typedef struct{ //paddle
    int x;
    int size;
}Paddle;

typedef enum {
    ENLARGE = 0,
    SLOW,
    BREAK,
    DISRUPTION,
    LASER,
    LIFE
} PowerUps;

typedef struct{ //capsules
    int x;
    float vy, y;
    PowerUps type;
}Capsule;

typedef struct{ //timers
    int enlarge;
    int slow;
}Timer;

//APPLICATION STATES
typedef enum {
    STATE_INTRO = 0,   // title screen: Play / Quit
    STATE_USERNAME,    // name entry: Play / Quit
    STATE_PLAYING,     // the game itself
    STATE_PAUSED,      // Continue / Quit / Scoreboard
    STATE_GAME_OVER,   // game over animation
    STATE_SCOREBOARD,  // top 5 scores
    STATE_QUIT         // main() leaves its loop
} GameState;

// GAME
// Everything that used to be a separate global variable (game_state, username,
// score, lives, level, ball, brick, paddle, capsule, next_capsule, screen) now
// lives in here. main() owns the single instance and every function that needs
// it receives a pointer, instead of reaching for an extern.
typedef struct Game {
    GameState game_state;
    char username[MAX_USERNAME];
    int score;
    short int lives;
    short int level;
    int combined_brick_health;

    Ball ball[MAX_BALLS];
    int active_balls;
    Brick brick[BRICK_ROWS][BRICK_COLUMNS];
    Paddle paddle;
    Capsule capsule[MAX_CAPSULES_PER_LEVEL];
    int next_capsule;
    Timer timer;
    int frame_counter;
    const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH];
} Game;

typedef struct { //score entry for the scoreboard
    char name[MAX_USERNAME];
    int  score;
    int  level;
} ScoreEntry;

#endif
