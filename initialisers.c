/** @file   initialisers.c
    @authors Alina Phang, William Wallace
    @date   18 October 2018
    @brief  Module containing initialising
*/

#include "initialisers.h"

/** Initialises all of the drivers we will use */
void initialiseAll (void)
{
    system_init ();
    display_init ();
    navswitch_init ();
    ir_uart_init ();
    pacer_init (PACER_RATE);
    tinygl_init (PACER_RATE);
    led_init ();
}


/** Initialises the font, speed and mode of text */
void tinygl_setUp (void)
{
    tinygl_font_set (&font5x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
}
