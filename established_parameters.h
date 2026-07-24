#ifndef EST_PARAM
    #define EST_PARAM

    #define SCREEN_WIDTH 50
    #define SCREEN_HEIGHT 48

    #define PADDLE_WIDTH_0 12

    #define BRICK_ROWS 1 // We should make it that the number of rows increase with the level, but 1 is the initial value.
    #define BRICK_COLUMNS 8
    #define BRICK_WIDTH 6
    #define BRICK_HEIGHT 2

    #ifdef _WIN32 //FOR A WIN USER
        #include <windows.h>
        #include <pdcurses.h> //used to render and get inputs whitout stoping the program execution
        #define sleep_ms(ms) Sleep(ms) //unifies sleep functions
    #else
        #include <ncurses.h> //used to render and get inputs whitout stoping the program execution
        #include <unistd.h>
        #define sleep_ms(ms) usleep((ms) * 1000) //unifies sleep functions
    #endif
#endif