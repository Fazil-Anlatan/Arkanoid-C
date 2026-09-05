#include "established_parameters.h"
#include "initialization_structures.h"
#include "application_state.h"
#include "game_data.h"

//-------------LOCAL FUCTION DECLARATIONS------------
void print_info(Game *game);
//------- RENDERS -------

void set_blank_screen(Game *game) {
    int i, j;
    for (i = 0; i < SCREEN_HEIGHT; i++) {
        for (j = 0; j < SCREEN_WIDTH ; j++) {
            game->screen[i][j] = " ";
        }
    }
}

void set_borders(Game *game) {

    //set roof
    int i;
    for (i = 1; i <= SCREEN_WIDTH; i++) {
        game->screen[0][i] = "═";
    }

    //set upper corners
    game->screen[0][0] = "╔";
    game->screen[0][SCREEN_WIDTH-1] = "╗";

    //set sides
    int k;
    for (k = 1; k < SCREEN_HEIGHT; k++) {
        game->screen[k][0] = "║";
        game->screen[k][SCREEN_WIDTH - 1] = "║";
    }

    //Bottom will remain uncovered
}

void set_paddle(Game *game) {
    int i;
    for (i = 0; i < game->paddle.size; i++) {
        int px = game->paddle.x + i;
        game->screen[SCREEN_HEIGHT - 2][px] = "▀";
    }
}

void set_ball (Game *game) {
    int i;
    for (i = 0; i < game->active_balls; i++) {
        game->screen[(int)game->ball[i].y][(int)game->ball[i].x] = "●";
    }
}



void set_bricks (Game *game) {
    int i, j;
    for (i = 0; i < BRICK_ROWS; i++) {
        for (j = 0; j < BRICK_COLUMNS; j++) {
            int start_y = 1 + i * BRICK_HEIGHT;
            int start_x = 1 + j * BRICK_WIDTH;
            
            int k, l;
            //draw the brick
            if (game->brick[i][j].health > 0) {
                for (k = 0; k < BRICK_HEIGHT; k++) {
                    for (l = 0; l < BRICK_WIDTH; l++) {
                        game->screen[start_y + k][start_x + l] = "─";
                    }
                }
                game->screen[start_y][start_x] = "┌";
                game->screen[start_y][start_x + BRICK_WIDTH - 1] = "┐";
                game->screen[start_y + BRICK_HEIGHT - 1][start_x] = "└";
                game->screen[start_y + BRICK_HEIGHT - 1][start_x + BRICK_WIDTH - 1] = "┘";
            }
        }
    }
}
void set_capsule(Game *game) {
    int k;
    for (k = 0; k < MAX_CAPSULES_PER_LEVEL; k++){
        if (game->capsule[k].vy > 0) {
            game->screen[(int)game->capsule[k].y][game->capsule[k].x] = "<";
            game->screen[(int)game->capsule[k].y][game->capsule[k].x + 1] = ">";
        }
    }
}

void draw_all(Game *game) {
    //set screen
    clear();
    set_blank_screen(game);
    set_borders(game);
    set_paddle(game);
    set_ball(game);
    set_bricks(game);
    set_capsule(game);
    print_info(game);
    
    //print screen
    int i, j;
    for (i=0; i < SCREEN_HEIGHT; i++) {
        for (j=0; j < SCREEN_WIDTH; j++) {
            mvprintw(i, j, "%s", game->screen[i][j]);
        }
    }
    // re-writes the bricks with colors, because the above code doesn't support colors. The color is stored in the brick structure.
    for (i = 0; i < BRICK_ROWS; i++) {
        for (j = 0; j < BRICK_COLUMNS; j++) {
            if (game->brick[i][j].health > 0) {
                int start_y = 1 + i * BRICK_HEIGHT;
                int start_x = 1 + j * BRICK_WIDTH;
                
                // turn on the color for this brick
                attron(COLOR_PAIR(game->brick[i][j].color));
                
                int k, l;
                for (k = 0; k < BRICK_HEIGHT; k++) {
                    for (l = 0; l < BRICK_WIDTH; l++) {
                        mvprintw(start_y + k, start_x + l, "─");
                    }
                }
                mvprintw(start_y, start_x, "┌");
                mvprintw(start_y, start_x + BRICK_WIDTH - 1, "┐");
                mvprintw(start_y + BRICK_HEIGHT - 1, start_x, "└");
                mvprintw(start_y + BRICK_HEIGHT - 1, start_x + BRICK_WIDTH - 1, "┘");
                
                // turn off the color for this brick
                attroff(COLOR_PAIR(game->brick[i][j].color));
            }
        }
    }
    refresh();//updates what's shown on console
}

void print_info(Game *game){
    char* lives_s= (game->lives==3?"♥ ♥ ♥":(game->lives==2?"♥ ♥":(game->lives==1?"♥":"0")));
        mvprintw(SCREEN_HEIGHT, 0, "╔═════════════════════════════════════╗");
        mvprintw(SCREEN_HEIGHT+1, 0, "║Score: %-30d║", game->score);
        mvprintw(SCREEN_HEIGHT+2, 0,"║");//finishes the rectangle 
        mvprintw(SCREEN_HEIGHT+2, 38,"║");//" 
        mvprintw(SCREEN_HEIGHT + 2, 1,"Lives: %s",lives_s);
        mvprintw(SCREEN_HEIGHT + 3, 0, "║Level: %-30d║", game->level);
        mvprintw(SCREEN_HEIGHT+4, 0,"╚═════════════════════════════════════╝");
    }
 

//------- END RENDER -------
//---------GAME OVER ANIMATION
void draw_game_over(Game *game) {
    clear();
    
    // Titileo: Si la división da par mostramos, si da impar ocultamos.
    // frame_counter / 10 hace que cambie la visibilidad cada 250ms aprox.
    if ((game->frame_counter / 10) % 2 == 0) {
        const char *art_game[5] = {
            "[][][]  [][]  []   [] [][][]",
            "[]     []  [] [][] [][] []    ",
            "[] [][] [][][] [] [] [] [][][]",
            "[]  [] []  [] []   [] []    ",
            "[][][] []  [] []   [] [][][]"
        };
        
        const char *art_over[5] = {
            "[][][] []  [] [][][] [][][]",
            "[]  [] []  [] []     []  []",
            "[]  [] []  [] [][][] [][][]",
            "[]  []  [][]  []     [] [] ",
            "[][][]   []   [][][] []  []"
        };

        // Centrado en la pantalla (SCREEN_WIDTH=50, SCREEN_HEIGHT=48)
        for (int i = 0; i < 5; i++) {
            mvprintw(15 + i, 10, "%s", art_game[i]);
            mvprintw(22 + i, 11, "%s", art_over[i]);
        }
    }
    
    refresh();
}

//MENU SCREENS
// Pure front-end: these ask application_state.c what to show and turn the answer into characters. They decide nothing.

// name of the game
#define TITLE_ROWS 5
#define TITLE_WIDTH 34
static const char *title_art[TITLE_ROWS] = {
    "████ █    ████ █  █ ████ ████ ███ ",
    "█    █    █  █ ██ █ █  █  ██  █  █",
    "█    █    █  █ █ ██ █  █  ██  █  █",
    "█    █    █  █ █  █ █  █  ██  █  █",
    "████ ████ ████ █  █ ████ ████ ███ "
};

// menu renderer
static void draw_menu_items(const MenuItem *items, int count, int y, int x) {
    int i;
    int sel = active_selection();

    if (items == NULL) {
        return;
    }

    for (i = 0; i < count; i++) {
        if (i == sel) {
            attron(A_REVERSE);
        }
    
        mvprintw(y + i * 2, x, "  %-12s  ", items[i].label);
        if (i == sel) {
            attroff(A_REVERSE);
        }
    }
}
void draw_intro(Game *game) {
    int count, i;
    const MenuItem *menu = active_menu(game, &count);

    erase();

    for (i = 0; i < TITLE_ROWS; i++) {
        mvprintw(6 + i, (SCREEN_WIDTH - TITLE_WIDTH) / 2, "%s", title_art[i]);
    }
    mvprintw(13, 14, "a brick breaker in C");
    mvprintw(15, 8, "──────────────────────────────────");

    draw_menu_items(menu, count, 19, 17);

    mvprintw(28, 6, "Up / Down to move, Enter to select");
    refresh();
}


void draw_username(Game *game) {
    int count;
    const MenuItem *menu = active_menu(game, &count);

    erase();

    mvprintw(6, 16, "WHO IS PLAYING?");
    mvprintw(8, 8, "──────────────────────────────────");
    mvprintw(11, 12, "Name: %s_", game->username);
    mvprintw(13, 12, "type it, Backspace to fix");

    draw_menu_items(menu, count, 18, 17);

    mvprintw(28, 6, "Leave it empty and you are %s", DEFAULT_USERNAME);
    refresh();
}

void draw_scoreboard(Game *game) {
    int count, i, j;
    const MenuItem *menu = active_menu(game, &count);

    ScoreEntry top[MAX_SCORES];
    int n = load_scores(top, MAX_SCORES);

    erase();

    mvprintw(6, 17, "TOP %d SCORES", MAX_SCORES);
    mvprintw(8, 8, "──────────────────────────────────");

    if (n == 0) {
        mvprintw(13, 14, "No games played yet");
    }
    else {
        for (i = 0; i < n; i++) {
            mvprintw(11 + i * 2, 9, "%d. %-16s %5d  (lvl %d)",
                     i + 1, top[i].name, top[i].score, top[i].level);
        }
    }

    draw_menu_items(menu, count, 24, 17);

    mvprintw(28, 6, "Enter to go back");
    refresh();
}


// Unlike the other two this one is an overlay: it redraws the frozen board first and then paints a panel on top, so the player keeps their read on where the ball and paddle were. draw_all() ends with its own refresh(); the second refresh() below is what shows the panel, and curses only pushes changed cells.
void draw_pause(Game *game) {
    int count, i, j;
    const MenuItem *menu = active_menu(game, &count);

   int top = 13;
    int left = 7;
    int width = 36;
    int height = 20;
    
    draw_all(game); // the frozen game underneath

    for (i = 1; i < height - 1; i++) {
        for (j = 1; j < width - 1; j++) {
            mvprintw(top + i, left + j, " ");
        }
    }

    for (j = 1; j < width - 1; j++) {
        mvprintw(top, left + j, "═");
        mvprintw(top + height - 1, left + j, "═");
    }
    for (i = 1; i < height - 1; i++) {
        mvprintw(top + i, left, "║");
        mvprintw(top + i, left + width - 1, "║");
    }
    mvprintw(top, left, "╔");
    mvprintw(top, left + width - 1, "╗");
    mvprintw(top + height - 1, left, "╚");
    mvprintw(top + height - 1, left + width - 1, "╝");

    mvprintw(top + 2, left + 15, "PAUSED");

    mvprintw(top + 4, left + 2, "%s!", game->username);
    mvprintw(top + 5, left + 2, "You are at the level %d and you", game->level);
    mvprintw(top + 6, left + 2, "have %d lives left, get back to", game->lives);
    mvprintw(top + 7, left + 2, "your ship and continue bouncing!");

    draw_menu_items(menu, count, top + 10, left + 9);

    mvprintw(top + height - 3, left + 4, "P or Continue to resume");
    refresh();
}

void draw_gameover() {
    int count;
    const MenuItem *menu = active_menu(&count);

    erase();

    mvprintw(6, 18, "GAME OVER");
    mvprintw(8, 8, "──────────────────────────────────");
    mvprintw(12, 12, "%s", username);
    mvprintw(14, 12, "Score: %d", score);
    mvprintw(15, 12, "Level: %d", level);

    draw_menu_items(menu, count, 20, 17);

    mvprintw(28, 6, "Up / Down to move, Enter to select");
    refresh();
}

//------- END MENU SCREENS -------
