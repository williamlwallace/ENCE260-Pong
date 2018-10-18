/** @file   communication.h
    @authors Alina Phang, William Wallace
    @date   18 October 2018
    @brief  Module containing functions for IR transmissions
*/

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "initialisers.h"

/** Tries to receive a message from the other player */
char getMessage (void);


/** Sends a message to the other player */
void sendMessage (char message);

#endif

