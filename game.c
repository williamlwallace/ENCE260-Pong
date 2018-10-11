#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "display.h"
#include "tinygl.h"
#include "../fonts/font5x5_1.h"
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


void startScreen (void)
{
    tinygl_setUp ();
    tinygl_text ("PONG\0");

    while (navswitch_up_p (NAVSWITCH_PUSH)) {
        pacer_wait ();

        tinygl_update ();
        navswitch_update ();
    }
}


int main (void)
{
    initialiseAll ();
    startScreen ();

    int rows[3] = {2, 3, 4}; // initial position of bar

    while(1)
    {
        pacer_wait ();
        tinygl_clear ();

        bar_lightUp (COL, rows);

        navswitch_update ();
        display_update ();

        if (navswitch_push_event_p (NAVSWITCH_NORTH))
            bar_moveRight (COL, rows);
            bar_lightUp (COL, rows);

        if (navswitch_push_event_p (NAVSWITCH_SOUTH))
            bar_moveLeft (COL, rows);
            bar_lightUp (COL, rows);

    }
    return 0;
}
