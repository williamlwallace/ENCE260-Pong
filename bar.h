/** @file   bar.h
    @authors Alina Phang, William Wallace
    @date   11 October 2018
    @brief  Pong bar module

    @defgroup bar Pong bar module
*/
#ifndef BAR_H
#define BAR_H

#include "system.h"
#include "navswitch.h"
#include "display.h"

#define COL 4

void bar_lightUp (int col, int rows[]);

void bar_moveLeft (int col, int rows[]);

void bar_moveRight (int col, int rows[]);

void bar_task (int rows[]);

#endif
