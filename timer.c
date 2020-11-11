/*
 * timer.c - general purpose delay timer function derived from a timer interrupt
 *
 *  Created on: 09 Aug 2020
 *      Author: jasonmitchell
 */
#include <msp430.h>
#include "timer.h"



unsigned int TIMERCOUNT;




// Name: Delay
// Function: Generate specified delay
// Arguments: Delay in milli-seconds between 0 and 65535
// Returns: void
//------------------------------------------------------------------------
void Delay(unsigned int delay){

    unsigned int user_delay;


    user_delay = delay * 2;
    TIMERCOUNT = 0;

    for(;;){
        if (TIMERCOUNT >= user_delay){
            return;
        }
    }

}


// Name: InitTimerSystem
// Function: Initialise the timer system e.g. clear variables
// Arguments: void
// Returns: void
//------------------------------------------------------------------------
void InitTimerSystem(void){
    TIMERCOUNT = 0;
}

// Name: CallInISR
// Function: Call this in the timer ISR, so that it drives our timers here
// Arguments: void
// Returns: void
//------------------------------------------------------------------------
void CallInISR(void){

    TIMERCOUNT++;
    TIME_MEASURE++;


}





