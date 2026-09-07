#ifndef EST_PARAM
    #define EST_PARAM

    #define SCREEN_WIDTH 50
    #define SCREEN_HEIGHT 48

    #define PADDLE_WIDTH_0 12

    #define ENLARGE_DURATION 400
    #define SLOW_DURATION 200

    #define MAX_CAPSULES_PER_LEVEL 8
    #define MAX_BALLS 3
    #define MAX_ANGLE 70.0f * 3.14159265f / 180.0f

    #define INITIAL_BRICK_ROWS 4
    #define MAX_BRICK_ROWS 10
    #define BRICK_COLUMNS 8
  
    #define BRICK_WIDTH 6
    #define BRICK_HEIGHT 2
    #define MAX_USERNAME 16 // buffer size for the username
    #define DEFAULT_USERNAME "Player" // used if Play is pressed with no name typed

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
