#include <stdio.h>
#include <locale.h> //needed in linux for special characters
#include <time.h>
#include "established_parameters.h"
#include "initialization_structures.h"
#include "physics.h"
#include "game_data.h"
#include "render.h"
#include "application_state.h"
//------- FUNCTION DECLARATIONS -------
void draw_game_over(Game *game);

//------- END FUNCTION DECLARATIONS -------
int main () {
    srand(time(NULL)); // seed the random number generator for capsules and brick patterns
    Game game = {0}; // the only instance of the game's state; everything below gets a pointer to it instead of using globals
    game_data_init(&game); // sets game_state, username, score, lives and level to their starting values

    setlocale(LC_ALL, ""); //for the characters
    //------inicialization of ncurses-----         
    initscr();

    start_color(); // initialize colors (ncurses)
        
        // Define color pairs init_pair (number, Text_Color, Background_Color)
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_BLUE, COLOR_BLACK);
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(6, COLOR_CYAN, COLOR_BLACK);

    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    gameInicialization(&game);

    //------- MAIN LOOP -------
    // main() is no longer the game loop, it is a router. One frame is: read a key, hand it to whichever screen is active, draw that screen, sleep.
    while (game.game_state != STATE_QUIT) {

        // remembered so we can tell at the end of the frame whether this key caused a screen change
        GameState previous_state = game.game_state;

        int c = getch();

        switch (game.game_state) {

            case STATE_INTRO:
                app_state_update(&game, c);
                if (game.game_state == STATE_INTRO) {
                    draw_intro(&game);
                }
                break;

            case STATE_USERNAME:
                app_state_update(&game, c);
                if (game.game_state == STATE_USERNAME) {
                    draw_username(&game);
                }
                break;

            case STATE_PLAYING:
                if (c == 'p' || c == 'P') {
                    enter_state(&game, STATE_PAUSED);
                    break;
                }
                draw_all(&game);
                ball_update(&game);
                paddle_update(&game, c);
                capsule_update(&game);
                level_up(&game, c);
                timers_update(&game);
                break;

            case STATE_GAME_OVER:
                draw_game_over(&game);
                game.frame_counter++;
                
                // A 25ms por frame, 120 frames son ~3 segundos. 
                // Luego de 3 segundos, volvemos a la pantalla de inicio.
                if (game.frame_counter > 120) {
                    enter_state(&game, STATE_INTRO);
                }
                break;



            case STATE_PAUSED:
                if (c == 'p' || c == 'P') {
                    enter_state(&game, STATE_PLAYING);
                }
                else {
                    app_state_update(&game, c);
                }
                if (game.game_state == STATE_PAUSED) {
                    draw_pause(&game);
                }
                break;

            default:
                break;
        }

        if (game.game_state != previous_state) {
            flushinp();
        }

        sleep_ms(25); 
    }
    endwin();
    printf("Game Over!\n");
    
}









