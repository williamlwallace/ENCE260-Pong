/** @file   aesthetics.c
    @authors Alina Phang, William Wallace
    @date   16 October 2018
    @brief  Module containing the various user screens as well as their supplementary functions
*/

#include "aesthetics.h"


/** Starting screen: Push to be the player with the ball first, NAVSWITCH_EAST otherwise */
void startScreen (int* playing)
{
    tinygl_setUp ();
    tinygl_text ("PONG:PUSH FOR BALL\0");

    uint8_t state = 0;
    uint8_t led_tick = 0;

    while (1) {
        pacer_wait ();

        led_tick++;
        if (led_tick >= FLASHING_SPEED) { // flash LED at 12 Hz

            led_tick = 0;
            state = led_task (state);
        }

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) { // player that pushes down gets the ball first

            *playing = 1; // variable indicating that this player is now active
            break;

        } else if (navswitch_push_event_p (NAVSWITCH_EAST)) { // other player

            *playing = 0;
            break;
        }

        tinygl_update ();
        navswitch_update ();
    }

    led_set  (LED1, 0); // turn LED off
}


/** Screen shown if you lose */
void loserScreen (int* playing, int* rows)
{
    tinygl_setUp ();
    tinygl_text ("GAME OVER\0");

    while (1) {
        pacer_wait ();

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {

            *playing = 1;
            restartGame (playing, rows);
            break;
        }

        if (navswitch_push_event_p (NAVSWITCH_EAST)) {

            *playing = 0;
            restartGame (playing, rows);
            break;
        }

        navswitch_update ();
        tinygl_update ();
    }
}


/** Screen shown if you win */
void winnerScreen (int* playing, int* rows)
{
    tinygl_setUp ();
    tinygl_text ("YOU WIN!!!\0");

    uint8_t state = 0;
    uint8_t led_tick = 0;

    while (1) {
        pacer_wait ();

        led_tick++;
        if (led_tick >= FLASHING_SPEED) { // flash LED at 12 Hz

            led_tick = 0;
            state = led_task (state);
        }

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {

            *playing = 1;
            restartGame (playing, rows);
            break;
        }

        if (navswitch_push_event_p (NAVSWITCH_EAST)) {

            *playing = 0;
            restartGame (playing, rows);
            break;
        }

        navswitch_update ();
        tinygl_update ();
    }
}


/** Toggles LED */
uint8_t led_task (uint8_t state)
{
    led_set  (LED1, state);
    state = !state;

    return state;
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

    while (count_tick < (HALF_SECOND * 3)) { // after '1' has shown for a half second
        pacer_wait();

        count_tick++;

        display_character ('3');

        if (count_tick >= HALF_SECOND) { // after half a second

            display_character ('2');
        }

        if (count_tick >= (HALF_SECOND) * 2) { // after another half second

            display_character ('1');
        }

        tinygl_update ();
    }
}
