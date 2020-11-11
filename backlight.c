//----------------------------------------------------------------------------------------
//      Name: backlight.c
//      Simple library to handle the LCD backlight
//
//
//----------------------------------------------------------------------------------------

#include <msp430.h>
#include "backlight.h"


unsigned char backlight_timeout;
unsigned char timeout_counter;
unsigned int timing_value;


//------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------

// Function called in the timer ISR, providing timing
//------------------------------------------------------
void BLTimerISR(void){

    // This is calibrated here to execute every second
    timing_value--;
    if (timing_value == 0){
        timing_value = 1000 * 2;
        if(timeout_counter > 0){
            timeout_counter--;
            if (timeout_counter == 0){
                P1OUT &= ~LCD_BACKLIGHT;
            }
        }
    }



}
// Function to turn the backlight on and arm the pre-set timeout
//----------------------------------------------------------------
void BacklightON(void){

    P1OUT |= LCD_BACKLIGHT;                     // Backlight ON
    timeout_counter = backlight_timeout;        // Load time value


}

//----------------------------------------------------------------------------
// Initialisation function - called to turn the light on and set the timeout
//----------------------------------------------------------------------------
void InitBacklight(unsigned char timeout){
    P1OUT |= LCD_BACKLIGHT;                     // Backlight ON
    backlight_timeout = timeout;                // set the timeout value
    timeout_counter = timeout;
    timing_value = 1000 * 2;


}
