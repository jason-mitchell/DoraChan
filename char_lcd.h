/*
 * char_lcd.h
 *
 *  Created on: Oct 31, 2020
 *      Author: Sonikku
 */

#ifndef CHAR_LCD_H_
#define CHAR_LCD_H_

// Definitions (where the hardware is mapped to)
//-----------------------------------------------
#define control_line_port P1OUT
#define data_bus_port P2OUT

#define LCD_RS BIT4             // [P1.4]
#define LCD_ENA BIT5            // [P1.5]

// Function Prototypes
//--------------------
void InitLCD(void);
void WriteLCDInst(unsigned char cmd);
void WriteLCDData(unsigned char data);
void putChar(unsigned char b);
void putString( char *string);
void ClearLCD(void);
void LCDTimerA(void);
void LCDTimerB(void);
void NextLine(void);
void HomeCursor(void);




#endif /* CHAR_LCD_H_ */
