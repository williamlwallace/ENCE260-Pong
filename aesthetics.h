/** @file   aesthetics.h
    @authors Alina Phang, William Wallace
    @date   16 October 2018
    @brief  Module containing starting/winning/losing/countdown screens as well as their supplementary functions

*/
#ifndef AESTHETICS_H
#define AESTHETICS_H

#include "initialisers.h"

/** Toggles LED */
uint8_t led_task (uint8_t state);


/** Starting screen: Push to be the player with the ball first, NAVSWITCH_EAST otherwise */
void startScreen (int* playing);


/** Screen shown if you lose */
void loserScreen (int* playing, int* rows, int* row, int* col, int* rowinc, int* colinc);


/** Screen shown if you win */
void winnerScreen (int* playing, int* rows, int* row, int* col, int* rowinc, int* colinc);


/** Function for displaying a character, taken from lab3-ex3.c */
void display_character (char character);


/** Counts down from 3, half a second at a time */
void countdown (void);

#endif
