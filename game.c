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
    ir_uart_init ();
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


void bar_task (int rows[]) // put into bar.c
{
    if (navswitch_push_event_p (NAVSWITCH_NORTH))
        bar_moveRight (COL, rows);
        bar_lightUp (COL, rows);

    if (navswitch_push_event_p (NAVSWITCH_SOUTH))
        bar_moveLeft (COL, rows);
        bar_lightUp (COL, rows);
}


typedef struct player_s { // ???????
    char player; // A or B?
    int col; // position of ball that needs to be transmitted
    int row;
} Player;


void choosePlayer (void)
{
    // navswitch to decide player A and player B?
}


// ball variables global so that they can be modified between functions
int row = 0;
int col = 0;
int rowinc = 0;
int colinc = 0;
int ball_tick = 0;


void ball_task (void)
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
            loserScreen (); // need to send message to other AVR that they've won
        }
    }

    if (col < 0) // just bounces off the top for now, should be received on the other AVR as (row+-1, 0)? If rowinc is negative then row-1, if rowinc is positive then row+1 for continuity?
    {
        col -= colinc * 2;
        colinc = -colinc;
    }

    // draw new position
    display_pixel_set (col, row, 1);
}


int main (void)
{
    initialiseAll ();
    startScreen ();
    int rows[3] = {2, 3, 4}; // initial LED positions of bar

    row = 3;
    col = 2;
    rowinc = 1;
    colinc = 1;
    ball_tick = 0; // initial values for ball_task ()

    tinygl_clear (); // clear the starting screen
    bar_lightUp (COL, rows); // initialise the bar

    while(1)
    {
        pacer_wait ();

        ball_tick++;
        if (ball_tick >= 120) // ball moves at 4 Hz (every 120 loops)
        {
            display_pixel_set (col, row, 1); // initialise ball position
            ball_task ();
        }

        bar_task (rows);

        display_update ();
        navswitch_update ();
    }
    return 0;
}
