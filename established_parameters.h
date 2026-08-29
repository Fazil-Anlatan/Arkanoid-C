#ifndef EST_PARAM
    #define EST_PARAM

    #define SCREEN_WIDTH 50
    #define SCREEN_HEIGHT 48

    #define PADDLE_WIDTH_0 12

    #define MAX_CAPSULES_PER_LEVEL 8
    #define MAX_ANGLE 70.0f * 3.14159265f / 180.0f

    #define BRICK_ROWS 1 // We should make it that the number of rows increase with the level, but 1 is the initial value.
    #define BRICK_COLUMNS 8
  
    #define BRICK_WIDTH 6
    #define BRICK_HEIGHT 2
    #define MAX_USERNAME 16 // max lenght for the username
    #define DEFAULT_USERNAME "Player" // In case no username has been typed

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
