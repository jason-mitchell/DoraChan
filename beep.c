//-----------------------------------------------------------------------------------------------------
//                  Name: Beep.c
//                  Function: Perform beeper using DS1307 SQW output
//                  Author: Sonikku2k
//
//          Usage scenario: Cannot use while I2C comm to EEPROM is in progress. To prevent issues, a semaphore is added
//
//
//-----------------------------------------------------------------------------------------------------

#include "rtc.h"
#include "beep.h"

unsigned char beeper_semaphore;
unsigned int beep_time = 0;
unsigned int BEEPTIMER = 0;

// Beep control function - arms the beeper
//-----------------------------------------
void Beep(unsigned int duration){
    BEEPTIMER = 0;
    beep_time = duration * 2;
    WriteRTC(0x07, 0x11);           // Write RTC register to output 4.096kHz on its SQW pin

}


// Semaphore control from other functions that share access to the I2C
//---------------------------------------------------------------------
void SetSemaphore(unsigned char value){
    beeper_semaphore = value;
}

// Timing function called in the systick
//----------------------------------------
void BeepTick(void){

   if (beeper_semaphore == 1){
       return;
   }

   if (beep_time > 0){
       BEEPTIMER++;
       if (BEEPTIMER >= beep_time){
           WriteRTC(0x07, 0x00);           // Write RTC register to disable SQW pin
           beep_time = 0;
       }
   }




}
