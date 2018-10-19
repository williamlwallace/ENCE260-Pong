/** ENCE260 Assignment 2018
 * Game: Pong
 * Group number: 426
 * Student              Usercode
 * Alina Phang          aph78
 * William Wallace      wwa52 */

#include "initialisers.h"
#include "pong.h"
#include "bar.h"
#include "ball.h"
#include "communication.h"
#include "aesthetics.h"


int main (void)
{
    initialiseAll ();

    int rows[3] = {CENTRE_LED - 1, CENTRE_LED, CENTRE_LED + 1}; // initial LED positions of bar

    int playing = 0; // initial state of players: 0 = not your turn, 1 = your turn

    playGame (&playing, rows);

    return 0;
}
