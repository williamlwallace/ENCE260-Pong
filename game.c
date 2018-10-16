/** ENCE260 Assignment 2018
 * Game: Pong
 * Group number: 426
 * Student              ID
 * Alina Phang          35207642
 * William Wallace      11389447*/

#include "stdio.h"
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
#define BALL 'B'

int playing = 0; // global playing variable, 0 if player not active
bool not_break = true; // makes sure the game does not break please


/** Initialises all of the drivers we will use */
void initialiseAll (void)
{
    system_init ();
    display_init ();
    navswitch_init ();
    ir_uart_init ();
    pacer_init (PACER_RATE);
    tinygl_init (PACER_RATE);
}


/** Initialises the font, speed and mode of text */
void tinygl_setUp (void)
{
    tinygl_font_set (&font5x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
}


/** Toggles LED */
uint8_t led_task (uint8_t state)
{
    led_set  (LED1, state);
    state = !state;
    return state;
}


/** Starting screen: Push to be the player with the ball first, NAVSWITCH_EAST otherwise */
void startScreen (void)
{
    tinygl_setUp ();
    tinygl_text ("PONG:PUSH FOR BALL\0");
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

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) { // player that pushes down gets the ball first
            playing = 1; // variable indicating that this player is now active
            break;
        } else if (navswitch_push_event_p (NAVSWITCH_EAST)) { // other player
            break;
        }

        tinygl_update ();
        navswitch_update ();
    }

    led_set  (LED1, 0); // turn LED off
}


/** Screen shown if you lose */
void loserScreen (void)
{
    tinygl_setUp ();
    tinygl_text ("GAME OVER\0");

    while (1) {
        pacer_wait ();

        tinygl_update ();
    }
}


/** Screen shown if you win */
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


/** Function for displaying a character, taken from lab3-ex3.c */
void display_character (char character)
{
    char buffer[2];
    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}


/** Counts down from 3, half a second at a time */
void countdown (void)
{
    tinygl_font_set (&font5x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
    int count_tick = 0;

    while (count_tick < 750) // after '1' has shown for a half second
    {
        pacer_wait();

        count_tick++;

        display_character ('3');

        if (count_tick >= 250) { // after half a second
            display_character ('2');
        }

        if (count_tick >= 500) { // after another half second
            display_character ('1');
        }

        tinygl_update ();
    }
}


/** Tries to receive a message from the other player */
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


/** Global ball variables: global so that they can be modified between functions */
int row = 0;
int col = 0;

int rowinc = 0;
int colinc = 0; // increments decide which direction ball moves in

char sendRow = 0; // row converted to a char so it can be transmitted to the other player
char inverseRow[7] = {6, 5, 4, 3, 2, 1, 0}; // where the row is on the other side

uint8_t ball_tick = 0;


/** Moves ball in a certain direction, one LED at a time. Reference: ball movement code from Michael Hayes' bounce2.c */
void ball_task (void)
{
    ball_tick = 0; // reset ticks
    display_pixel_set (col, row, 0); // erase previous position

    col += colinc;
    row += rowinc; // always moves diagonally

    if (row > 6 || row < 0) // if ball hits wall, bounce
    {
        row -= rowinc * 2;
        rowinc = -rowinc; // reverses direction for next time
    }

    if (col == 4) // if at bottom of screen
    {
        if (display_pixel_get (col, row)) { // if bar in the right place, bounce
            col -= colinc * 2;
            colinc = -colinc; // reverses direction for next time

        } else { // you missed
            sendMessage (LOST); // send message to other player that they've won
            display_clear ();
            loserScreen ();
        }
    }

    if (col < 0) // if past top of screen
    {
        playing = 0;
        sendRow = inverseRow[row]; // the row as a char, according to where it should be received at on the other side
        sendMessage (BALL); // let the other player know it has to receive a ball
        sendMessage (sendRow);
    }

    display_pixel_set (col, row, 1); // draw new position
}


int main (void)
{
    initialiseAll ();

    int rows[3] = {3, 4, 5}; // initial LED positions of bar
    char message = 0;

    row = 3;
    col = 0;
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

        if (playing == 1) { // at first this is only the player that pushed down
            ball_tick++;

            if (ball_tick >= 100) { // ball moves at 5 Hz (every 100 loops)
                display_pixel_set (col, row, 1); // initialise ball position
                ball_task ();
            }
        }

        bar_task (rows);

        display_update ();
        navswitch_update ();


        if (playing == 0) {
            message = getMessage ();

            if (message == BALL) {
                playing = 1; // now other player joins
                row = getMessage ();
                col = 0;
                rowinc = -rowinc;
                //colinc = -colinc;
            }

            if (message == LOST) { // the other player lost
                display_clear ();
                winnerScreen ();
            }
        }
    }
}
