/** @file   bar.h
    @authors Alina Phang, William Wallace
    @date   11 October 2018
    @brief  Pong bar module
*/

#include "bar.h"

void bar_lightUp (int col, int rows[]) // turns on three LEDs for the bar
{
    display_pixel_set (col, rows[0], 1);
    display_pixel_set (col, rows[1], 1);
    display_pixel_set (col, rows[2], 1);
}


void bar_moveLeft (int col, int rows[])
{
    int i = 0;
    display_pixel_set (col, rows[0], 0); // turns off rightmost LED of the bar
    if (rows[2] == 6) { // if bar is at the edge of the ledmat
        for (; i < 3; i++) {
            display_pixel_set (col, rows[i], 0); // turn all LEDs off
            rows[i] -= 4; // update rows to be on the other side
        }
    } else {
        for (; i < 3; i++) {
            rows[i]++; // move LEDs one to the left
        }
    }
}


void bar_moveRight (int col, int rows[])
{
    int i = 0;
    display_pixel_set (col, rows[2], 0); // turns off leftmost LED of the bar
    if (rows[0] == 0) { // if bar is at the edge of the ledmat
        for (; i < 3; i++) {
            display_pixel_set (col, rows[i], 0); // turn all LEDs off
            rows[i] += 4; // update rows to be on the other side
        }
    } else {
        for (; i < 3; i++) {
            rows[i]--; // move LEDs one to the right
        }
    }
}


void bar_task (int rows[])
{
    if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
        bar_moveRight (COL, rows);
        bar_lightUp (COL, rows);
    }

    if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
        bar_moveLeft (COL, rows);
        bar_lightUp (COL, rows);
    }
}

