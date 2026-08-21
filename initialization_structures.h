#ifndef INI_STRUCT
#define INI_STRUCT
//------FUCTION DECLARATIONS-------------
void gameInicialization(); //Divided into 3 functions: ball, paddle and bricks
void gameInicialization_ball();
void gameInicialization_paddle();
void gameInicialization_bricks();
void gameInicialization_capsules();
//-------------STRUCTURES-----------
typedef struct{ //ball
    float x, y;
    float vx, vy;
}Ball;

typedef struct{ //bricks
    int health;
    int contains_capsule;
}Brick;

typedef struct{ //paddle
    int x;
    int size;
}Paddle;

typedef struct{
    int x;
    float vy, y;
    int type;
}Capsule;

#endif
