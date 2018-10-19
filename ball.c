/** @file   ball.c
    @authors Alina Phang, William Wallace
    @date   18 October 2018
    @brief  Pong ball module
*/

#include "ball.h"


/** Resets ticks, LED and previous position of ball to get it ready for its next position */
void ball_refresh (int* ball_tick, int col, int row)
{
    *ball_tick = 0;
    led_set  (LED1, 0);
    display_pixel_set (col, row, 0);
}


/** Updates row or column and reverses direction of ball */
void ball_bounce (int* axis, int* direction)
{
    *axis -= *direction * 2;
    *direction = -*direction;
}


/** Checks if ball collided with bar or if the player missed. If so, you lose */
void ball_collision (int* playing, int* rows, int* row, int* col, int* colinc)
{
    if (display_pixel_get (*col, *row)) { // if bar in the right place, bounce

        led_set (LED1, 1); // turn on LED upon bouncing
        ball_bounce (col, colinc);

    } else {

        sendMessage (LOST); // send message to other player that they've won
        display_clear ();
        loserScreen (playing, rows);
    }
}


/** Switches player that has the ball, returning the row it should be received at */
int ball_switch (int* playing, int row)
{
    char inverseRow[7] = {6, 5, 4, 3, 2, 1, 0}; // where the row is on the other side

    *playing = 0; // change current player to be not playing
    sendMessage (BALL); // let the other player know they have to receive a ball
    row = inverseRow[row];

    return row;
}


/** Moves ball in a certain direction, one LED at a time. Reference: ball movement code from Michael Hayes' bounce2.c */
void ball_task (int* playing, int* ball_tick, int* rows, int* row, int* col, int* rowinc, int* colinc)
{
    ball_refresh (ball_tick, *col, *row);

    *col += *colinc;
    *row += *rowinc; // always moves diagonally

    if (*row > LEFT_WALL || *row < RIGHT_WALL) // if ball hits wall, bounce
    {
        ball_bounce (row, rowinc);
    }

    if (*col == FLOOR) // if at bottom of screen, check for collision
    {
        ball_collision (playing, rows, row, col, colinc);
    }

    if (*col < TOP) // if past top of screen, switch players
    {
        *row = ball_switch (playing, *row);
    }

    display_pixel_set (*col, *row, 1); // update position of ball
}
