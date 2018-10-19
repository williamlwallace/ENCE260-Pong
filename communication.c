/** @file   communication.c
    @authors Alina Phang, William Wallace
    @date   18 October 2018
    @brief  Module containing functions for IR transmissions
*/

#include "communication.h"


/** Tries to receive and return a message from the other player */
char getMessage (void)
{
    if (ir_uart_read_ready_p ()) {

        return ir_uart_getc ();

    } else {

        return 0;
    }
}


/** Sends a message to the other player */
void sendMessage (char message)
{
    if (ir_uart_write_ready_p ()) {

        ir_uart_putc (message);
    }
}
