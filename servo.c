//---------------------------------------------------------------------------------------------------------------------------------
//                      Name: servo.c
//                  Control library for DS3225-270
//               https://www.robotics.org.za/DS3225-270
//
//
//
//---------------------------------------------------------------------------------------------------------------------------------

#include <msp430.h>
#include "servo.h"
#include "timer.h"

#define min_duty_cycle 50
#define max_duty_cycle 380

unsigned int servo_position = min_duty_cycle;


//-----------------------------------------------------------------------------------------------------------------------
// Name: InitServo
// Function: Set up the PWM for the servo. The DS3225-270 has a usable PWM range from around 20% duty cycle to around 70%
// Parameters: nil
// Returns: void
//------------------------------------------------------------------------------------------------------------------------
void InitServo(void){

    // Configure Timer as PWM for servo
    // Target period is 20mS (50Hz) Futaba standard
    //-----------------------------------------------
    P1SEL |= BIT2;                                      // P1.2 to TA0.1
    TACTL = TACLR;                                      // Clear Timer
    CCR0 = 2500;                                        // PWM Period
    CCTL1 = OUTMOD_7;                                   // CCR1 reset/set
    CCR1 = 0;                                           // CCR1 PWM duty cycle = 0
    TACTL = TASSEL_2 + MC_1 + ID_3;                     // SMCLK, up mode, timer running, divide input clock by 8
    CCR1 = servo_position;                              // PWM pulse width set


}


void CommandServo(unsigned int position, unsigned int change_rate){

    unsigned char direction = 0;

    if (position >= max_duty_cycle){
        return;
    }

    // Check difference to determine which direction to ramp, if a change rate is set
    if (position > servo_position){
        direction = 0;              // ramp PWM up at given rate
    }

    if (position < servo_position){
        direction = 1;              // ramp PWM down at given rate
    }

    if(change_rate == 0){
        // Rate is specified as zero, set servo to position immediately
        servo_position = position;
        CCR1 = servo_position;
    } else {
        // A rate has been set, move servo at given rate
        for(;;){
            if(servo_position == position){
                return;
            }

            if (direction == 0){
                servo_position++;
            } else {
                servo_position--;
            }
            CCR1 = servo_position;
            Delay(1002 - change_rate);
        }
    }

}
