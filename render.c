#include "established_parameters.h"
#include "structures.h"
#include "initialization_structures.h"
#include "application_state.h"
#include "game_data.h"

//-------------EXTERNAL VAR DECLARATIONS------------
extern Ball ball;
extern Brick brick[BRICK_ROWS][BRICK_COLUMNS];
extern Paddle paddle;
extern Capsule capsule[MAX_CAPSULES_PER_LEVEL];
extern GameState game_state;
extern int score;
extern short int lives;
extern short int level;
extern const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH];
extern char username[MAX_USERNAME];
//-------------LOCAL FUCTION DECLARATIONS------------
void print_info();
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

void set_capsule() {
    int k;
    for (k = 0; k < MAX_CAPSULES_PER_LEVEL; k++){
        if (capsule[k].vy > 0) {
            screen[(int)capsule[k].y][capsule[k].x] = "<";
            screen[(int)capsule[k].y][capsule[k].x + 1] = ">";
        }
    }
}

void draw_all() {
    //set screen
    clear();
    set_blank_screen();
    set_borders();
    set_paddle();
    set_ball();
    set_bricks();
    set_capsule();
    print_info();
    
    //print screen
    int i, j;
    for (i=0; i < SCREEN_HEIGHT; i++) {
        for (j=0; j < SCREEN_WIDTH; j++) {
            mvprintw(i, j, "%s", screen[i][j]);
        }
    }
    refresh();//updates what's shown on console
}

void print_info(){
    char* lives_s= (lives==3?"♥ ♥ ♥":(lives==2?"♥ ♥":(lives==1?"♥":"0")));
        mvprintw(SCREEN_HEIGHT, 0, "╔═════════════════════════════════════╗");
        mvprintw(SCREEN_HEIGHT+1, 0, "║Score: %-30d║", score);
        mvprintw(SCREEN_HEIGHT+2, 0,"║");//finishes the rectangle 
        mvprintw(SCREEN_HEIGHT+2, 38,"║");//" 
        mvprintw(SCREEN_HEIGHT + 2, 1,"Lives: %s",lives_s);
        mvprintw(SCREEN_HEIGHT + 3, 0, "║Level: %-30d║", level);
        mvprintw(SCREEN_HEIGHT+4, 0,"╚═════════════════════════════════════╝");
    }
 

//------- END RENDER -------

// -------- Menu Screens -------

/* Three Functions to draw the menu screens, they ask which menu is active and print it.
The intro and username screens wipe the screen first, but the pause screen draws the game board and then puts a box on top, so it can still be seen where the ball was frozen. 
The sentence is split into four lines by hand because mvprintw does not wrap text, it just cuts it off at the edge */

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
void draw_intro() {
    int count, i;
    const MenuItem *menu = active_menu(&count);

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


void draw_username() {
    int count;
    const MenuItem *menu = active_menu(&count);

    erase();

    mvprintw(6, 16, "WHO IS PLAYING?");
    mvprintw(8, 8, "──────────────────────────────────");
    mvprintw(11, 12, "Name: %s_", username);
    mvprintw(13, 12, "type it, Backspace to fix");

    draw_menu_items(menu, count, 18, 17);

    mvprintw(28, 6, "Leave it empty and you are %s", DEFAULT_USERNAME);
    refresh();
}

void draw_scoreboard() {
    int count, i;
    const MenuItem *menu = active_menu(&count);

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


void draw_pause() {
    int count, i, j;
    const MenuItem *menu = active_menu(&count);

    int top = 13;
    int left = 7;
    int width = 36;
    int height = 20;
    
    draw_all(); // the frozen game underneath

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

    mvprintw(top + 4, left + 2, "%s!", username);
    mvprintw(top + 5, left + 2, "You are at the level %d and you", level);
    mvprintw(top + 6, left + 2, "have %d lives left, get back to", lives);
    mvprintw(top + 7, left + 2, "your ship and continue bouncing!");

    draw_menu_items(menu, count, top + 10, left + 9);

    mvprintw(top + height - 3, left + 4, "P or Continue to resume");
    refresh();
}

//------- End Menu Screens ---------
