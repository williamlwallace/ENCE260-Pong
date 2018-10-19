/** @file   bar.h
    @authors Alina Phang, William Wallace
    @date   11 October 2018
    @brief  Pong bar module
*/

#ifndef BAR_H
#define BAR_H

#include "initialisers.h"

#define BAR_COL 4
#define LEFT_WALL 6
#define RIGHT_WALL 0


/** Turns on the three LEDs for the bar */
void bar_lightUp (int col, int* rows);


/** Moves the bar left, wrapping around if it hits the edge */
void bar_moveLeft (int col, int* rows);


/** Moves the bar right, wrapping around if it hits the edge */
void bar_moveRight (int col, int* rows);


/** If navswitch north move bar right, if navswitch south move left */
void bar_task (int* rows);


#endif
