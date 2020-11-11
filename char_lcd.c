//------------------------------------------------------------------------------------------------
//                  Name: char_lcd.c
//
//              Created on: Oct 31, 2020
//                  Author: Sonikku
//  Driver for character based LCD modules in 4 bit mode
//-------------------------------------------------------------------------------------------------

#include <msp430.h>
//#include "timer.h"
#include "char_lcd.h"

//-------------------------------------------------------
// Name: InitLCD
// Function: Initialise the LCD in 4 bit mode
//-------------------------------------------------------
void InitLCD(){

    //Delay(100);             // Power-on delay
    LCDTimerB();
    LCDTimerB();
    LCDTimerB();
    P2OUT = 0x03 | 0x30;           // instruction
    P1OUT &= ~LCD_ENA;      // Enable goes low
    P1OUT |= LCD_ENA;       // Enable goes high

    //Delay(10);
    LCDTimerB();
    P2OUT = 0x03 | 0x30;           // instruction
    P1OUT &= ~LCD_ENA;      // Enable goes low
    P1OUT |= LCD_ENA;       // Enable goes high

    //Delay(10);
    LCDTimerB();
    P2OUT = 0x03 | 0x30;           // instruction
    P1OUT &= ~LCD_ENA;      // Enable goes low
    P1OUT |= LCD_ENA;       // Enable goes high

    //Delay(10);
    LCDTimerB();
    P2OUT = 0x02 | 0x30;           // instruction
    P1OUT &= ~LCD_ENA;      // Enable goes low
    P1OUT |= LCD_ENA;       // Enable goes high

    // LCD should now be in 4-bit mode, so we can use the function
    WriteLCDInst(0x28);     // Function Set (00011000) Duty cycle, bus width, font size
    WriteLCDInst(0x0C);     // Set up the mode of the cursor, turn LCD on, etc...


}

//----------------------------------------------------------------------
// Name: LCDTimerA
// Function: Software timer for timing the LCD cycle when writing regular data/commands
// Parameters: none
// Returns: nil
//---------------------------------------------------------------------
void LCDTimerA(void){
    unsigned int r = 0;
    for (r = 0; r < 135; r++){
        __delay_cycles(10);
    }
}

//----------------------------------------------------------------------
// Name: LCDTimerB
// Function: Software timer for timing the LCD cycle when waiting for LCD to perform commands
// Parameters: none
// Returns: nil
//---------------------------------------------------------------------
void LCDTimerB(void){
    unsigned int r = 0;
    for (r = 0; r < 1500; r++){
        __delay_cycles(10);
    }
}
//----------------------------------------------------------------------
// Name: putString
// Function: Function to write a string to the display
// Parameters: string in the form of a byte array
// Returns: void
//----------------------------------------------------------------------
void putString( char *string){
    unsigned int selector = 0;

    while(string[selector] != 0){
        putChar(string[selector]);
        selector++;
    }
}

//----------------------------------------------------------------------
// Name: putChar
// Function: Standard I/O function to write a character to the display
// Parameters: Byte comprising the character
// Returns: void
//----------------------------------------------------------------------
void putChar(unsigned char b){
    WriteLCDData(b);
}

//----------------------------------------------------------------------
// Name: HomeCursor
// Function: Home cursor without affecting display content
//----------------------------------------------------------------------
void HomeCursor(void){
    WriteLCDInst(0x02);
    LCDTimerB();

}
//----------------------------------------------------------------------
// Name: NextLine
// Function: Go to next line on LCD by cursor manipulation
//----------------------------------------------------------------------
void NextLine(void){
    WriteLCDInst(0xC0);
    LCDTimerB();


}
//----------------------------------------------------------------------
// Name: ClearLCD
// Function: Send instruction to LCD to clear display and home cursor
// Parameters: void
//-----------------------------------------------------------------------
void ClearLCD(void){
    WriteLCDInst(0x01);
    LCDTimerB();
}

//-------------------------------------------------------
// Name: WriteLCDInst
// Function: Transfer 8-bit instruction to LCD on 4 bit bus
// Parameters: Instruction code
// Returns: void
//-------------------------------------------------------
void WriteLCDInst(unsigned char cmd){

    unsigned char cmd_out;
    P1OUT &= ~LCD_RS;
    cmd_out = cmd;
    cmd_out = cmd_out >> 4;
    cmd_out = cmd_out & 0x0F;

    P2OUT = cmd_out | 0x30;         // instruction hi nibble + preserve P2.4 and P2.5
    P1OUT &= ~LCD_ENA;              // Enable goes low
    P1OUT |= LCD_ENA;               // Enable goes high
    //LCDTimerA();
    P2OUT = (cmd & 0x0F) | 0x30;    // instruction lo nibble + preserve P2.4 and P2.5
    P1OUT &= ~LCD_ENA;              // Enable goes low
    P1OUT |= LCD_ENA;               // Enable goes high
    LCDTimerA();


}

//-------------------------------------------------------
// Name: WriteLCDData
// Function: Transfer 8-bit DATA to LCD on 4 bit bus
// Parameters: Instruction code
// Returns: void
//-------------------------------------------------------
void WriteLCDData(unsigned char data){

    unsigned char data_out;

    P1OUT |= LCD_RS;
    data_out = data;
    data_out = data_out >> 4;
    data_out = data_out & 0x0F;

    P2OUT = data_out | 0x30;            // instruction hi nibble
    P1OUT &= ~LCD_ENA;                  // Enable goes low
    P1OUT |= LCD_ENA;                   // Enable goes high
    //LCDTimerA();
    P2OUT = (data & 0x0F) | 0x30;         // instruction lo nibble
    P1OUT &= ~LCD_ENA;                  // Enable goes low
    P1OUT |= LCD_ENA;                   // Enable goes high
    LCDTimerA();


}

