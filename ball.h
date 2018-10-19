/** @file   ball.h
    @authors Alina Phang, William Wallace
    @date   18 October 2018
    @brief  Pong ball module
*/

#ifndef BALL_H
#define BALL_H

#include "initialisers.h"
#include "communication.h"
#include "aesthetics.h"

#define LOST 'L'
#define BALL 'B'
#define LEFT_WALL 6
#define RIGHT_WALL 0
#define FLOOR 4
#define TOP 0


/** Resets ticks, LED and previous position of ball to get it ready for its next position */
void ball_refresh (int* ball_tick, int col, int row);


/** Updates row or column and reverses direction of ball */
void ball_bounce (int* axis, int* direction);


/** Checks if ball collided with bar or if the player missed. If so, you lose */
void ball_collision (int* playing, int* rows, int* row, int* col, int* colinc);


/** Switches player that has the ball, returning the row it should be received at */
int ball_switch (int* playing, int row);


/** Moves ball in a certain direction, one LED at a time. Reference: ball movement code from Michael Hayes' bounce2.c */
void ball_task (int* playing, int* ball_tick, int* rows, int* row, int* col, int* rowinc, int* colinc);


#endif
