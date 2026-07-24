#include "established_parameters.h"
#include "initialization_structures.h"
//-------------EXTERNAL VAR DECLARATIONS------------
extern Ball ball;
extern Brick brick[BRICK_ROWS][BRICK_COLUMNS];
extern Paddle paddle;
extern short int game_state;
extern int score;
extern short int lives;
extern short int level;
extern const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH];
//------- RENDERS -------

void set_blank_screen() {
    int i, j;
    for (i = 0; i < SCREEN_HEIGHT; i++) {
        for (j = 0; j < SCREEN_WIDTH ; j++) {
            screen[i][j] = " ";
        }
    }
}

void set_borders() {

    //set roof
    int i;
    for (i = 1; i <= SCREEN_WIDTH; i++) {
        screen[0][i] = "═";
    }

    //set upper corners
    screen[0][0] = "╔";
    screen[0][SCREEN_WIDTH-1] = "╗";

    //set sides
    int k;
    for (k = 1; k < SCREEN_HEIGHT; k++) {
        screen[k][0] = "║";
        screen[k][SCREEN_WIDTH - 1] = "║";
    }

    //Bottom will remain uncovered
}

void set_paddle() {
    int i;
    for (i = 0; i < paddle.size; i++) {
        int px = paddle.x + i;
        screen[SCREEN_HEIGHT - 2][px] = "▀";
    }
}

void set_ball () {
    screen[(int)ball.y][(int)ball.x] = "●";
}



void set_bricks () {
    int i, j;
    for (i = 0; i < BRICK_ROWS; i++) {
        for (j = 0; j < BRICK_COLUMNS; j++) {

            int start_y = 1 + i * BRICK_HEIGHT;
            int start_x = 1 + j * BRICK_WIDTH;

            int k, l;
            //draws a brick
            if (brick[i][j].health > 0) {
                for (k = 0; k < BRICK_HEIGHT; k++) {
                    for (l = 0; l < BRICK_WIDTH; l++) {
                        screen[start_y + k][start_x + l] = "─";
                    }
                }
                screen[start_y][start_x] = "┌";
                screen[start_y][start_x + BRICK_WIDTH - 1] = "┐";
                screen[start_y + BRICK_HEIGHT - 1][start_x] = "└";
                screen[start_y + BRICK_HEIGHT - 1][start_x + BRICK_WIDTH - 1] = "┘";
            }
        }
    }
}

void draw_all() {
    //set screen
    set_blank_screen();
    set_borders();
    set_paddle();
    set_ball();
    set_bricks();

    //print screen
    int i, j;
    for (i=0; i < SCREEN_HEIGHT; i++) {
        for (j=0; j < SCREEN_WIDTH; j++) {
            mvprintw(i, j, "%s", screen[i][j]);
        }
    }
    mvprintw(SCREEN_HEIGHT, 0, "Score: %d", score);
    mvprintw(SCREEN_HEIGHT + 1, 0, "Lives: %s",(lives==3?"♥ ♥ ♥":(lives==2?"♥ ♥":(lives==1?"♥":"0"))));
    mvprintw(SCREEN_HEIGHT + 2, 0, "Level: %d", level);

    refresh();//updates what's shown on console
}

//------- END RENDER -------