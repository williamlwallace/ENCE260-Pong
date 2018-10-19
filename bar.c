/** @file   bar.c
    @authors Alina Phang, William Wallace
    @date   11 October 2018
    @brief  Pong bar module
*/

#include "bar.h"


/** Turns on the three LEDs for the bar */
void bar_lightUp (int col, int* rows)
{
    display_pixel_set (col, rows[0], 1);
    display_pixel_set (col, rows[1], 1);
    display_pixel_set (col, rows[2], 1);
}


/** Moves the bar left, wrapping around if it hits the edge */
void bar_moveLeft (int col, int* rows)
{
    int i = 0;

    display_pixel_set (col, rows[0], 0); // turns off rightmost LED of the bar

    if (rows[2] == LEFT_WALL) { // if bar is at the edge of the ledmat

        for (; i < 3; i++) {
            display_pixel_set (col, rows[i], 0);
            rows[i] -= 4; // update rows to be on the other side
        }

    } else {

        for (; i < 3; i++) {
            rows[i]++; // move LEDs one to the left
        }
    }
}


/** Moves the bar right, wrapping around if it hits the edge */
void bar_moveRight (int col, int* rows)
{
    int i = 0;

    display_pixel_set (col, rows[2], 0); // turns off leftmost LED of the bar

    if (rows[0] == RIGHT_WALL) { // if bar is at the edge of the ledmat

        for (; i < 3; i++) {
            display_pixel_set (col, rows[i], 0);
            rows[i] += 4; // update rows to be on the other side
        }

    } else {

        for (; i < 3; i++) {
            rows[i]--; // move LEDs one to the right
        }
    }
}


/** If navswitch north move bar right, if navswitch south move left */
void bar_task (int* rows)
{
    if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
        bar_moveRight (BAR_COL, rows);
        bar_lightUp (BAR_COL, rows);
    }

    if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
        bar_moveLeft (BAR_COL, rows);
        bar_lightUp (BAR_COL, rows);
    }
}
