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
#define COL 4

void tinygl_setUp (void) // initialises the font, speed and mode
{
    tinygl_font_set (&font5x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
}


void initialiseAll (void)
{
    system_init ();
    display_init ();
    navswitch_init ();
    pacer_init (PACER_RATE);
    tinygl_init (PACER_RATE);
}


uint8_t led_task (uint8_t state) // toggles LED
{
    led_set  (LED1, state);
    state = !state;
    return state;
}


void startScreen (void)
{
    tinygl_setUp ();
    tinygl_text ("PONG\0");
    led_init ();
    uint8_t state = 0;
    uint8_t led_tick = 0;

    while (navswitch_up_p (NAVSWITCH_PUSH)) {
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

/*
void choosePlayer (void)
{
    // navswitch to decide player A and player B?
}


void barCollision (int col, int row)
{
}
*/

// globals so that they can be modified
int row = 0;
int col = 0;
int rowinc = 0;
int colinc = 0;
int ball_tick = 0;


void ball_task (void)
{
    row = 3;
    col = 2;
    rowinc = 1;
    colinc = 1;
    ball_tick = 0;

    display_pixel_set (col, row, 1);

    while (1)
    {
        pacer_wait ();

        ball_tick++;
        if (ball_tick >= 25)
        {
            ball_tick = 0;

            // erase previous position
            display_pixel_set (col, row, 0);

            col += colinc; // moves diagonally
            row += rowinc;

            if (row > 6 || row < 0) // if ball hits wall
            {
                row -= rowinc * 2; // reverse
                rowinc = -rowinc;
            }

            if (col > 4 || col < 0) // modify to check for bar collision here (use display_pixel_get (uint8_t col, uint8_t row) to see if the LED is on)???
            {
                col -= colinc * 2; // reverse
                colinc = -colinc;
                // else loses and sends message to other AVR that they've won
            }

            // draw new position
            display_pixel_set (col, row, 1);
        }
        display_update ();
    }
}


void bar_task (int rows[]) // put into bar.c
{

    if (navswitch_push_event_p (NAVSWITCH_NORTH))
        bar_moveRight (COL, rows);
        bar_lightUp (COL, rows);

    if (navswitch_push_event_p (NAVSWITCH_SOUTH))
        bar_moveLeft (COL, rows);
        bar_lightUp (COL, rows);

    if (navswitch_push_event_p (NAVSWITCH_EAST)) // testing
        winnerScreen ();

    if (navswitch_push_event_p (NAVSWITCH_WEST)) // testing
        loserScreen ();
}



int main (void)
{
    initialiseAll ();
    startScreen ();
    int rows[3] = {2, 3, 4}; // initial LED positions of bar

    tinygl_clear (); // clear the starting screen
    bar_lightUp (COL, rows); // initialise the bar

    row = 3;
    col = 2;
    rowinc = 1;
    colinc = 1;
    ball_tick = 0;

    while(1)
    {
        pacer_wait ();
        ball_tick++;

        if (ball_tick >= 350) // ball moves at 3 Hz (every 150 loops)
        {
            ball_tick = 0;
            display_pixel_set (col, row, 1);
            display_pixel_set (col, row, 0); // erase previous position

            col += colinc; // moves diagonally
            row += rowinc;

            if (row > 6 || row < 0) // if ball hits wall
            {
                row -= rowinc * 2; // reverse
                rowinc = -rowinc;
            }

            if (col == 4)
            {
                if (display_pixel_get (col, row)) {
                    col -= colinc * 2; // reverse
                    colinc = -colinc;
                } else {
                    display_clear ();
                    loserScreen ();
                // else loses and sends message to other AVR that they've won
                }
            }

            if (col < 0) // just bounces off the top for now
            {
                col -= colinc * 2; // reverse
                colinc = -colinc;
            }

            // draw new position
            display_pixel_set (col, row, 1);
        }

        bar_task (rows);

        display_update ();
        navswitch_update ();

    }
    return 0;
}
