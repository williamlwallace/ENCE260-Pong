/** ENCE260 Assignment 2018
 * Game: Pong
 * Group number: 426
 * Student              Usercode
 * Alina Phang          aph78
 * William Wallace      wwa52 */


#include "bar.h"
#include "ball.h"
#include "initialisers.h"
#include "communication.h"
#include "aesthetics.h"


bool not_break = true; // makes sure the game does not break please

void playGame (int* playing, int* rows, int* row, int* col, int* rowinc, int* colinc)
{
    int ball_tick = 0;
    char message = 0;

    startScreen (playing);
    tinygl_clear (); // clear the starting screen

    countdown (); // count down from 3 so the game doesn't start immediately
    tinygl_clear ();

    bar_lightUp (BAR_COL, rows); // initialise the bar

    while (1)
    {
        pacer_wait ();

        if (*playing == 1) { // at first this is only the player that pushed down
            ball_tick++;

            if (ball_tick >= 90) { // ball moves at 5.5 Hz (every 90 loops)
                display_pixel_set (*col, *row, 1); // initialise ball position
                ball_task (playing, &ball_tick, rows, row, col, rowinc, colinc);
            }
        }

        if (*playing == 0) {
            message = getMessage ();

            if (message == BALL) {
                *playing = 1; // now other player joins
                *col = -1;
                *rowinc = -*rowinc;
                *colinc = -*colinc; // reverse both directions for other players' side
            }

            if (message == LOST) { // the other player lost
                display_clear ();
                winnerScreen (playing, rows, row, col, rowinc, colinc);
            }
        }

        bar_task (rows);

        display_update ();
        navswitch_update ();
    }
}


void restartGame (int* playing, int* rows, int* row, int* col, int* rowinc, int* colinc)
{
    rows[0] = 2;
    rows[1] = 3;
    rows[2] = 4; // resetting where the bar should be

    *row = 3;
    *col = 2;
    *rowinc = 1;
    *colinc = -1;

    // same as initial values
    playGame (playing, rows, row, col, rowinc, colinc);
}


int main (void)
{
    int rows[3] = {2, 3, 4}; // initial LED positions of bar
    int playing = 0;

    int row = 3;
    int col = 2;
    int rowinc = 1;
    int colinc = -1; // initially moves upwards

    initialiseAll ();

    playGame (&playing, rows, &row, &col, &rowinc, &colinc);

    return 0;
}
