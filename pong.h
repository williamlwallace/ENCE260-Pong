/** @file   pong.h
    @authors Alina Phang, William Wallace
    @date   18 October 2018
    @brief  Module specific to the functioning of the game as a whole
*/

#ifndef PONG_H
#define PONG_H

#include "bar.h"
#include "ball.h"
#include "communication.h"

#define INITIAL_BALL_ROW 3
#define INITIAL_BALL_COL 2
#define CENTRE_LED 3
#define BALL_SPEED 90


/** Runs through the whole game */
void playGame (int* playing, int* rows);


/** Restarts the game */
void restartGame (int* playing, int* rows);


#endif
