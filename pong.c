/** @file   pong.c
    @authors Alina Phang, William Wallace
    @date   18 October 2018
    @brief  Module specific to the functioning of the game as a whole
*/

#include "pong.h"


/** Runs through the whole game */
void playGame (int* playing, int* rows)
{
    int ball_tick = 0;
    char message = 0;
    int row = INITIAL_BALL_ROW;
    int col = INITIAL_BALL_COL;
    int rowinc = 1;
    int colinc = -1; // initially moves upwards and to the left

    startScreen (playing);
    tinygl_clear (); // clear the starting screen

    countdown (); // count down from 3 so the game doesn't start immediately
    tinygl_clear ();

    bar_lightUp (BAR_COL, rows); // initialise the bar

    while (1) {
        pacer_wait ();

        if (*playing == 1) { // at first this is only the player that pushed down

            ball_tick++;

            if (ball_tick >= BALL_SPEED) { // ball moves at 5.5 Hz (every 90 loops)

                display_pixel_set (col, row, 1); // initialise ball position
                ball_task (playing, &ball_tick, rows, &row, &col, &rowinc, &colinc);
            }
        }

        if (*playing == 0) {

            message = getMessage ();

            if (message == BALL) {
                *playing = 1; // now this player is active and other player is waiting
                col = -1; // reset col
                rowinc = -rowinc;
                colinc = -colinc; // reverse both directions for other players' side
            }

            if (message == LOST) { // the other player lost
                display_clear ();
                winnerScreen (playing, rows);
            }
        }

        bar_task (rows);

        display_update ();
        navswitch_update ();
    }
}


/** Restarts the game */
void restartGame (int* playing, int* rows)
{
    rows[0] = CENTRE_LED - 1;
    rows[1] = CENTRE_LED;
    rows[2] = CENTRE_LED + 1; // resetting where the bar should be

    playGame (playing, rows);
}
