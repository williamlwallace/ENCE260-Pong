/** @file   ball.h
    @authors Alina Phang, William Wallace
    @date   18 October 2018
    @brief  Pong ball module
*/

#ifndef BALL_H
#define BALL_H

#define LOST 'L'
#define BALL 'B'

#include "initialisers.h"
#include "communication.h"
#include "aesthetics.h"

/** Resets ticks, LED and previous position of ball to get it ready for its next position */
void ball_refresh (int* ball_tick, int col, int row);


/** Updates row or column and reverses direction of ball */
void ball_bounce (int* axis, int* direction);


void ball_collision (int* playing, int* rows, int* row, int* col, int* rowinc, int* colinc);


int ball_switch (int* playing, int row);


/** Moves ball in a certain direction, one LED at a time. Reference: ball movement code from Michael Hayes' bounce2.c */
void ball_task (int* playing, int* ball_tick, int* rows, int* row, int* col, int* rowinc, int* colinc);

#endif
