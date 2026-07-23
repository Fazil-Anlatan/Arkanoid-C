#include <stdio.h>
#include <locale.h> //needed in linux for special characters

#ifdef _WIN32 //FOR A WIN USER
#include <windows.h>
#include <pdcurses.h> //used to render and get inputs whitout stoping the program execution
#define sleep_ms(ms) Sleep(ms) //unifies sleep functions
#else
#include <ncurses.h> //used to render and get inputs whitout stoping the program execution
#include <unistd.h>
#define sleep_ms(ms) usleep((ms) * 1000) //unifies sleep functions
#endif

#define SCREEN_WIDTH 50
#define SCREEN_HEIGHT 48

#define PADDLE_WIDTH_0 12

#define BRICK_ROWS 1 // We should make it that the number of rows increase with the level, but 1 is the initial value.
#define BRICK_COLUMNS 8
#define BRICK_WIDTH 6
#define BRICK_HEIGHT 2


//------- STRUCTURES & VARIABLES -------

typedef struct{ //ball
    float x, y;
    float vx, vy;
}Ball;
Ball ball;

typedef struct{ //bricks
    int health;
}Brick;
Brick brick[BRICK_ROWS][BRICK_COLUMNS];

typedef struct{ //paddle
    int x;
    int size;
}Paddle;
Paddle paddle;


short int game_active = 1;
int score = 0;
short int lives = 3;
short int level = 1;

//------- FUNCTION DECLARATIONS -------

void set_ball();
void set_bricks();
void set_paddle();
void gameInicialization(); //Divided into 3 functions: ball, paddle and bricks
    void gameInicialization_ball();
    void gameInicialization_paddle();
    void gameInicialization_bricks();
void set_blank_screen();
void set_borders();
void draw_all();
void ball_update();
void paddle_update(int c);
void level_up();
const char* screen[SCREEN_HEIGHT][SCREEN_WIDTH];

int main () {

    setlocale(LC_ALL, ""); //for the characters
    //------inicialization of ncurses-----
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    gameInicialization();

    while (game_active == 1) {
        int c = getch();
        draw_all();
        ball_update();
        paddle_update(c);
        level_up();
        sleep_ms(50); //about 16 FPS. Maybe we can make it a modifiable variable later.
    }
    endwin();
    printf("Game Over!\n");
    // should go to the menu (pause state)
}

//Initial values for game objects

void gameInicialization() {
   gameInicialization_ball();
   gameInicialization_paddle();
   gameInicialization_bricks();
}

void gameInicialization_ball() {
    
    ball.x = SCREEN_WIDTH / 2;
    ball.y = SCREEN_HEIGHT / 2;
    ball.vx = 0.5f;
    ball.vy = 1.0f;
}

void gameInicialization_paddle() {

    paddle.x = (int) (SCREEN_WIDTH / 2);
    paddle.size = PADDLE_WIDTH_0;
}

void gameInicialization_bricks() {
    int i,j;
    for (i = 0; i < BRICK_ROWS; i++) {
        for (j = 0; j < BRICK_COLUMNS; j++) {
            brick[i][j].health = 1;
        }
    }

}




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


//------- GAME UPDATES -------

void ball_update() {

    //position update
    ball.x += ball.vx;
    ball.y += ball.vy;

    if ((int)ball.x <= 1 || (int)ball.x >= SCREEN_WIDTH - 2) {
        ball.vx *= -1;
    }
    if ((int)ball.y <= 1) {
        ball.vy *= -1;
    }
    if (((int)ball.y >= SCREEN_HEIGHT - 1)) {
        if (lives >1){
        lives--;
        gameInicialization_ball();
        }
        else{
            game_active = 0;
        }

    }

    //paddle collision
    if ((int)ball.y == (SCREEN_HEIGHT - 3)){
        if ((int)ball.x >= paddle.x && (int)ball.x <= (paddle.x + PADDLE_WIDTH_0)) {
            ball.vy *= -1;
        }
    }

    //brick collision
    //create the indexes of the bricks that the ball is inside of.
    int brick_y = ((int)ball.y - 1) / BRICK_HEIGHT;
    int brick_x = ((int)ball.x - 1) / BRICK_WIDTH;

    if (brick_y < BRICK_ROWS && brick_x < BRICK_COLUMNS) {//only start if statement if the ball is inside a rendered or unrendered brick.

        if (brick[brick_y][brick_x].health > 0) {//only start if statement if the brick is alive
            brick[brick_y][brick_x].health -= 1;
            score++;
            
            //start collision test 
            float previous_ball_x = ball.x - ball.vx;
            float previous_ball_y = ball.y - ball.vy;

            int brick_left_limit = 1 + brick_x * BRICK_WIDTH;
            int brick_right_limit = brick_left_limit + BRICK_WIDTH - 1;

            int brick_top_limit = 1 + brick_y * BRICK_HEIGHT;
            int brick_bottom_limit = brick_top_limit + BRICK_HEIGHT - 1;

            if (previous_ball_x < brick_left_limit || previous_ball_x > brick_right_limit)
            {
                // Hit the side
                ball.vx *= -1;
            }
            if (previous_ball_y < brick_top_limit || previous_ball_y > brick_bottom_limit)
            {
                // Hit the top or bottom
                ball.vy *= -1;
            }
        }
    }
}

void paddle_update(int c)
{
    if ((c == 'a'|| c == 'A' || c == KEY_LEFT) && paddle.x > 1)
    {
        paddle.x -= 1; // Move paddle left
    }
    else if ((c == 'd' || c =='D' || c == KEY_RIGHT) && paddle.x < (SCREEN_WIDTH - paddle.size - 1))
    {
        paddle.x += 1; // Move paddle right
    }
}




void level_up() {
//------- LEVEL UP -------

// if the combined health of all bricks is 0, level up.
    int j, i;
    int combined_brick_health = 0;
    for (i = 0; i < BRICK_ROWS; i++){
        for (j = 0; j < BRICK_COLUMNS; j++){
            combined_brick_health += brick[i][j].health;
        }
    }
    if ((combined_brick_health == 0)){ //falta la tecla para pasar de nivel (modo desarrollador )pero primero tengo que dividir el codigo
                level++;
                gameInicialization_bricks();
                gameInicialization_ball();
    }
}

//------- END LEVEL UP -------

