/** ENCE260 Assignment 2018
 * Game: Pong
 * Group number: 426
 * Student              ID
 * Alina Phang          35207642
 * William Wallace      */

#include "system.h"
#include "pacer.h"
#include "led.h"
#include "navswitch.h"
#include "display.h"
#include "tinygl.h"
#include "../fonts/font5x5_1.h"
#include "ir_uart.h"
#include "bar.h"

#define PACER_RATE 500
#define MESSAGE_RATE 10
#define LOST 'L'

void initialiseAll (void)
{
    system_init ();
    display_init ();
    navswitch_init ();
    ir_uart_init ();
    pacer_init (PACER_RATE);
    tinygl_init (PACER_RATE);
}


void tinygl_setUp (void) // initialises the font, speed and mode
{
    tinygl_font_set (&font5x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
}


uint8_t led_task (uint8_t state) // toggles LED
{
    led_set  (LED1, state);
    state = !state;
    return state;
}


void startScreen (void) // return state from here
{
    tinygl_setUp ();
    tinygl_text ("PONG:PUSH WHEN READY\0");
    led_init ();
    uint8_t state = 0;
    uint8_t led_tick = 0;

    while (navswitch_up_p (NAVSWITCH_PUSH)) { // push navswitch to indicate that players are ready
        pacer_wait ();

        led_tick++;
        if (led_tick >= 42) { // flash LED at 12 Hz
            led_tick = 0;
            state = led_task (state);
        }

        tinygl_update ();
        navswitch_update ();
    }

    led_set  (LED1, 0); // turn LED off
}


void loserScreen (void)
{
    tinygl_setUp ();
    tinygl_text ("GAME OVER\0");

    while (1) {
        pacer_wait ();

        tinygl_update ();
    }
}


void winnerScreen (void)
{
    tinygl_setUp ();
    tinygl_text ("YOU WIN!!!\0");
    led_init ();
    uint8_t state = 0;
    uint8_t led_tick = 0;

    while (1) {
        pacer_wait ();

        led_tick++;
        if (led_tick >= 42) { // flash LED at 12 Hz
            led_tick = 0;
            state = led_task (state);
        }

        tinygl_update ();
    }
}


void display_character (char character)
{
    char buffer[2];
    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}


void countdown (void)
{
    tinygl_font_set (&font5x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
    int count_tick = 0;

    while (count_tick < 1500)
    {
        pacer_wait();

        count_tick++;

        display_character ('3'); // start count immediately

        if (count_tick >= 500) { // after another second
            display_character ('2');
        }

        if (count_tick >= 1000) { // after another second
            display_character ('1');
        }

        tinygl_update ();
    }
}


char getMessage (void)
{
    if (ir_uart_read_ready_p ()) {
        return ir_uart_getc ();
    } else {
        return 0;
    }
}


void sendMessage (char message)
{
    ir_uart_putc (message);
}


// ball variables global so that they can be modified between functions
int row = 0;
int col = 0;
int rowinc = 0;
int colinc = 0;
int ball_tick = 0;


/** Ball movement code from Michael Hayes' bounce2.c */

void ball_task (void) // return state from here
{
    ball_tick = 0; // reset ticks

    display_pixel_set (col, row, 0); // erase previous position

    col += colinc; // moves diagonally
    row += rowinc;

    if (row > 6 || row < 0) // bounce if ball hits wall
    {
        row -= rowinc * 2;
        rowinc = -rowinc;
    }

    if (col == 4) // if at bottom of screen
    {
        if (display_pixel_get (col, row)) { // if bar in the right place, bounce
            col -= colinc * 2;
            colinc = -colinc;

        } else { // you missed
            display_clear ();
            loserScreen ();
            sendMessage (LOST); // send message to other player that they've won
        }
    }

    if (col < 0) // just bounces off the top for now
    {
        col -= colinc * 2;
        colinc = -colinc;
        // sendMessage (ROW); ROW should be 'A' for 0, 'B' for 1, 'C' for 2, 'D' for 3, 'E' for 4, 'F' for 5, 'G' for 6
        // display_pixel_set (col, row, 0);

    }

    // draw new position
    display_pixel_set (col, row, 1);
}


int main (void)
{
    initialiseAll ();

    int rows[3] = {3, 4, 5}; // initial LED positions of bar

    row = 3;
    col = 2;
    rowinc = 1;
    colinc = 1;
    ball_tick = 0; // initial values for ball_task ()

    startScreen ();
    tinygl_clear (); // clear the starting screen

    countdown (); // count down from 3 so the game doesn't start immediately
    tinygl_clear ();

    bar_lightUp (COL, rows); // initialise the bar

    while (1)
    {
        pacer_wait ();

        // if (getMessage () == ROW)

        ball_tick++;
        if (ball_tick >= 100) // ball moves at 5 Hz (every 100 loops)
        {
            display_pixel_set (col, row, 1); // initialise ball position
            ball_task ();
        }

        bar_task (rows);

        display_update ();
        navswitch_update ();

        if (getMessage () == LOST) { // other player lost
            display_clear ();
            winnerScreen ();
        }

    }
    return 0;
}
