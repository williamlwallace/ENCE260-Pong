#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "tinygl.h"

typedef struct ball_s {
    int ballx;
    int bally;
    int ballx_dir;
    int bally_dir;
} ball


/** Checks if the ball collides with the bar
 * @param pongBall is the game ball
 * @param barPos is the position of the centre pixel of the bar
 * @return pongBall with new direction and movement
 **/
ball barCollision(ball pongBall, int barPos) {

    // Ball is moving towards right wall
    if (pongBall.ballx_dir == 1) {
        if (pongBall.ballx == 6) { // Bounce of right wall
                pongBall.ballx_dir = -1;
        }

    // Ball is moving towards left wall
    } else if (pongBall.ballx_dir == -1) {
        if (pongBall.ballx == 0) { // Bounce off left wall
                pongBall.ballx_dir = 1;
        }
    }

    return pongBall


/** Calculates the balls movement
 * @param pongBall is the current ball location
 * @returns pongBall with the new direction and movement
 **/
ball ballMovement(ball pongBall) {

    // Ball is moving up towards other player
    if (pongBall.bally_dir == 1) {
        if (pongBall.bally < 4) { // If ball position is less than end of screen
            pongBall.bally += 1; // keep moving up
        } else {
            pongBall.bally_dir = 1; // Else ball moving down towards player
            pongBall.bally -= 1;
        }

    // Ball is moving down towards player
    } else if (pongBall.bally_dir == -1) {
        if (pongBall.bally < 1) { // If ball reaches bar
            pongBall.bally_dir = 1; // then direction changes and ball moves upwards
        }
        pongBall.bally += pongBall.bally_dir;
    }
    return pongBall
}


/**
 * Called in game.c, updates the game ball
 * @param pongBall is a struct for the ball used in the game
 * @param barPos is the current position of the bar
 * @return returns the ball struct with updated position
 **/
ball ballUpdate(ball pongBall,int barPos){

    ball pongBall = barCollision(pongBall, barPos);
    ball pongBall = ballMovement(pongBall);

    return pongBall;
}
