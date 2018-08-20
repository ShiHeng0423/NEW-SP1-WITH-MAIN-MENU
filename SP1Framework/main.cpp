// This is the main file to hold everthing together

#include "Framework\timer.h"
#include "game.h"
#include "mALL.h"

CStopWatch timer;                            // Timer function to keep track of time and the frame rate
bool escgame = false;                    // Set to true if you want to quit the game
const unsigned char fps = 100;                // FPS of this game
const unsigned int frametime = 1000 / fps;    // time for each frame

//main loop declaration
void mainLoop( void );

// TODO:
// Bug in waitUntil. it waits for the time from getElapsedTime to waitUntil, but should be insignificant.

// main function - starting function
// You should not be modifying this unless you know what you are doing
int main( void )
{
    init();      // initialize your variables
    mainLoop();  // main loop
    shutdown();  // do clean up, if any. free memory  
    return 0;
}

//--------------------------------------------------------------
// Purpose  : This main loop calls functions to get input, 
//            update and render the game at a specific frame rate
//            You should not be modifying this unless you know what you are doing.
// Input    : void
// Output   : void
//--------------------------------------------------------------
void mainLoop( void )
{
    timer.startTimer();    // Start timer to calculate how long it takes to render this frame
    while (!escgame)      // run this loop until user wants to quit 
    {        
        keyboard();                         // get keyboard input
        update(timer.getElapsedTime());   // update the game
        loadscreen();                           // render the graphics output to screen
        timer.framelimit(frametime);   // Frame rate limiter. Limits each frame to a specified time in ms.      
    }    
}

