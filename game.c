#include "system.h"
#include "pacer.h"
#include "led.h"
#include "navswitch.h"
#include "display.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"

#define PACER_RATE 500
#define MESSAGE_RATE 10

void lightUp (int col, int rows[])
{
    display_pixel_set (col, rows[0], 1);
    display_pixel_set (col, rows[1], 1);
    display_pixel_set (col, rows[2], 1);
}

void flashLed (void)
{
    uint8_t state = 0;
    led_init ();

    while (1)
    {
        pacer_wait ();
        led_set (LED1, state);
        state = !state;
    }
}


void moveLeft (int col, int rows[])
{
    int i = 0;
    display_pixel_set (col, rows[0], 0);
    if (rows[2] == 6) {
        for (; i < 3; i++) {
            display_pixel_set (col, rows[i], 0);
            rows[i] -= 4;
        }
    } else {
        for (; i < 3; i++) {
            rows[i]++;
        }
    }
}


void moveRight (int col, int rows[])
{
    int i = 0;
    display_pixel_set (col, rows[2], 0);
    if (rows[0] == 0) {
        for (; i < 3; i++) {
            display_pixel_set (col, rows[i], 0);
            rows[i] += 4;
        }
    } else {
        for (; i < 3; i++) {
            rows[i]--;
        }
    }
}


void tinygl_setUp (void)
{
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
}


int main (void)
{
    system_init ();
    display_init ();
    navswitch_init ();
    pacer_init (PACER_RATE);
    tinygl_init (PACER_RATE);
    tinygl_setUp ();
    tinygl_text ("Pong\0");

    int col = 4;
    int rows[3] = {2, 3, 4};

    lightUp (col, rows);

    while(1)
    {
        pacer_wait ();
        tinygl_update ();
        display_update ();
        navswitch_update ();

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            tinygl_clear ();
        }

        if (navswitch_push_event_p (NAVSWITCH_NORTH))
            moveRight (col, rows);
            lightUp (col, rows);


        if (navswitch_push_event_p (NAVSWITCH_SOUTH))
            moveLeft (col, rows);
            lightUp (col, rows);

    }
    return 0;
}
