/** @file   aesthetics.h
    @authors Alina Phang, William Wallace
    @date   16 October 2018
    @brief  Module containing the various user screens as well as their supplementary functions

*/

#ifndef AESTHETICS_H
#define AESTHETICS_H

#include "initialisers.h"
#include "pong.h"

#define HALF_SECOND 250
#define FLASHING_SPEED 42


/** Starting screen: Push to be the player with the ball first, NAVSWITCH_EAST otherwise */
void startScreen (int* playing);


/** Screen shown if you lose */
void loserScreen (int* playing, int* rows);


/** Screen shown if you win */
void winnerScreen (int* playing, int* rows);


/** Toggles LED */
uint8_t led_task (uint8_t state);


/** Function for displaying a character, taken from lab3-ex3.c */
void display_character (char character);


/** Counts down from 3, half a second at a time */
void countdown (void);


#endif
