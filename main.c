#include <stdio.h>
#include <locale.h> //needed in linux for special characters
#include "established_parameters.h"
#include "structures.h"
#include "initialization_structures.h"
#include "physics.h"
#include "game_data.h"
#include "render.h"
#include "application_state.h"
//-------------EXTERNAL VAR DECLARATIONS------------
extern Ball ball;
extern Brick brick[BRICK_ROWS][BRICK_COLUMNS];
extern Paddle paddle;
extern GameState game_state;
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

    //------- MAIN LOOP -------
    // main() is no longer the game loop, it is a router. One frame is: read a key, hand it to whichever screen is active, draw that screen, sleep.
    while (game_state != STATE_QUIT) {

        // remembered so we can tell at the end of the frame whether this key caused a screen change
        GameState previous_state = game_state;

        int c = getch();

        switch (game_state) {

            case STATE_INTRO:
                app_state_update(c);
                if (game_state == STATE_INTRO) {
                    draw_intro();
                }
                break;

            case STATE_USERNAME:
                app_state_update(c);
                if (game_state == STATE_USERNAME) {
                    draw_username();
                }
                break;

            case STATE_PLAYING:
                if (c == 'p' || c == 'P') {
                    enter_state(STATE_PAUSED);
                    break;
                }
                draw_all();
                ball_update();
                paddle_update(c);
                capsule_update();
                level_up();
                break;

            case STATE_PAUSED:
                if (c == 'p' || c == 'P') {
                    enter_state(STATE_PLAYING);
                }
                else {
                    app_state_update(c);
                }
                if (game_state == STATE_PAUSED) {
                    draw_pause();
                }
                break;

            default:
                break;
        }

        if (game_state != previous_state) {
            flushinp();
        }

        sleep_ms(25); 
    }
    endwin();
    printf("Game Over!\n");
    
}










