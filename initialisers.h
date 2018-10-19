/** @file   initialisers.h
    @authors Alina Phang, William Wallace
    @date   18 October 2018
    @brief  Module containing all initialisation
*/

#ifndef INITIALISERS_H
#define INITIALISERS_H

#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "display.h"
#include "tinygl.h"
#include "../fonts/font5x5_1.h"
#include "ir_uart.h"
#include "led.h"

#define PACER_RATE 500
#define MESSAGE_RATE 10


/** Initialises all of the drivers we will use */
void initialiseAll (void);


/** Initialises the font, speed and mode of text */
void tinygl_setUp (void);


#endif
