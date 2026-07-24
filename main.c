#include <stdio.h>
#include <locale.h> //needed in linux for special characters
#include "established_parameters.h"
#include "initialization_structures.h"
#include "physics.h"
#include "game_data.h"
#include "render.h"
//-------------EXTERNAL VAR DECLARATIONS------------
extern Ball ball;
extern Brick brick[BRICK_ROWS][BRICK_COLUMNS];
extern Paddle paddle;
extern short int game_state;
extern int score;
extern short int lives;
extern short int level;
extern const char* screen;
//------- FUNCTION DECLARATIONS -------

//------- END FUNCTION DECLARATIONS -------
int main () {

    setlocale(LC_ALL, ""); //for the characters
    //------inicialization of ncurses-----         (I'M NOT SURE IF IT SHOULD BE HERE OR MAYBE JUST ONE FUNCTION ON ESTABLISHED PARAMETERS?) we have to discuss that
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    gameInicialization();

    while (game_state == 1) {
        int c = getch();
        draw_all();
        ball_update();
        paddle_update(c);
        level_up();
        sleep_ms(50); //about 16 FPS. Maybe we can make it a modifiable variable later. (TEMPORARY CHAGED)OG=>64
    }
    endwin();
    printf("Game Over!\n");
    // should go to the menu (pause state)
}










