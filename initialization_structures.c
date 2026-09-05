#include "established_parameters.h"
#include "initialization_structures.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
//Initial values for game objects

void gameInicialization(Game *game) {
   gameInicialization_ball(game);
   gameInicialization_paddle(game);
   gameInicialization_bricks(game);
   gameInicialization_capsules(game);
}

void gameInicialization_ball(Game *game) { //isn't finished yed
    time_t now = time(NULL);
    int i;

    for (i = 0; i < MAX_BALLS; i++) {
        game->ball[i].x = SCREEN_WIDTH / 2;
        game->ball[i].y = SCREEN_HEIGHT / 2;
        game->ball[i].vx = 0.0f;
        game->ball[i].vy = 0.0f;
    }

    game->ball[0].vx = ((now % 5) + 1) / 10.0; //x velocity between 0.1 and 0.5
   
    //Total velocity is a vector of 0.75
    //Calculated using the pythagorean theorem
    game->ball[0].vy = sqrt(pow(0.75, 2) - pow(game->ball[0].vx, 2)); //We need to make another constant for the length of the vector.
    
    //random directions
    if (now % 2 == 1) {
        game->ball[0].vx *= -1;
    }
    if (now % 3 == 1) {
        game->ball[0].vy *= -1;
    }

    game->active_balls = 1;
}

void gameInicialization_paddle(Game *game) {

    game->paddle.x = (int) (SCREEN_WIDTH / 2);
    game->paddle.size = PADDLE_WIDTH_0;
}

void gameInicialization_bricks(Game *game) {
    int i, j;
    
    const int l1_pattern[BRICK_ROWS][BRICK_COLUMNS] = {
        {1, 1, 0, 1, 1, 0, 1, 1},
        {1, 1, 0, 1, 1, 0, 1, 1},
        {1, 1, 0, 1, 1, 0, 1, 1},
        {1, 1, 0, 1, 1, 0, 1, 1}
    };

    for (i = 0; i < BRICK_ROWS; i++) {
    for (j = 0; j < BRICK_COLUMNS; j++) {
        game->brick[i][j].health = 0;
        game->brick[i][j].contains_capsule = 0;
        game->brick[i][j].color = 0; 
    }
}

    switch (game->level) {
        case 1:
            for (i = 0; i < BRICK_ROWS; i++) { // Use the predefined pattern for level 1 (similar to the real game)
                for (j = 0; j < BRICK_COLUMNS; j++) {
                    game->brick[i][j].health = l1_pattern[i][j];
                    if (game->brick[i][j].health > 0) {
                       // game->brick[i][j].contains_capsule = !(rand() % 10);
                       game->brick[i][j].contains_capsule = 1;                 // JUST TO TEST THE CAPSULES
                        game->brick[i][j].color = (rand() % 6) + 1;
                    }
                }
            }

            break;

        case 2:     //inverted triangle of bricks
            for (i = 0; i < BRICK_ROWS; i++) { 
                for (j = 0; j < BRICK_COLUMNS; j++) {
                    if (j >= i && j < BRICK_COLUMNS - i) {
                        game->brick[i][j].health = 1;
                        game->brick[i][j].contains_capsule = !(rand() % 10);
                        game->brick[i][j].color = (rand() % 6) + 1;
                    }
                }
            }
            break;
            
            break;

        case 3:
            for (i = 1; i < BRICK_ROWS+3; i++) { //square of bricks
                for (j = 1; j < BRICK_COLUMNS-1; j++) {
                    game->brick[i][j].health = 1;
                    game->brick[i][j].contains_capsule = !(rand() % 10);
                    game->brick[i][j].color = (rand() % 6) + 1;
                }
            }
            break;

        default: // For levels greater than 3, a random pattern of bricks will be generated
            char more_rows = (game->level -3)%BRICK_ROWS; // this will ad more rows of bricks if you pass level 3,
            for (i = 0; i < BRICK_ROWS + more_rows; i++) {
                for (j = 0; j < BRICK_COLUMNS; j++) {
                    game->brick[i][j].health = (rand() % 10 < 7) ? 1 : 0; //70 % of chace to generate a brick in 4 rows and 8 columns
                    if (game->brick[i][j].health > 0) {
                        game->brick[i][j].contains_capsule = !(rand() % 10);
                        game->brick[i][j].color = (rand() % 6) + 1;
                    }
                }
            }
            break;
    }
}


void gameInicialization_capsules(Game *game){
    int i,j,k;
    int disruption = rand() % MAX_CAPSULES_PER_LEVEL;
    game->next_capsule = 0;
    k = 0;
    for (i = 0; i < BRICK_ROWS; i++) {
        for (j = 0; j < BRICK_COLUMNS; j++) {
            if ((game->brick[i][j].contains_capsule > 0) && (k < MAX_CAPSULES_PER_LEVEL)) {
                game->capsule[k].x = 1 + j * BRICK_WIDTH; 
                game->capsule[k].y = 1 + i * BRICK_HEIGHT; 
                game->capsule[k].vy = 0;
                game->capsule[k].type = DISRUPTION;
               /* if (k == disruption)
                {
                    game->capsule[k].type = DISRUPTION;
                }
                else
                {
                    int type = rand() % 5;

                    if (type >= DISRUPTION)
                        type++;

                    game->capsule[k].type = type;
                }

                k++;*/
            }
        }
    }
}
void gameInicialization_timers(Game *game) {
    game -> timer.enlarge = 0;
    game -> timer.slow = 0;
}
