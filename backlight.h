/*
 * backlight.h
 *
 *  Created on: Nov 1, 2020
 *      Author: Sonikku
 */

#ifndef BACKLIGHT_H_
#define BACKLIGHT_H_

// Defs
//-------
#define LCD_BACKLIGHT BIT0          // [P1.0]

// Function Prototypes
//---------------------
void BLTimerISR(void);
void BacklightON(void);
void InitBacklight(unsigned char timeout);


#endif /* BACKLIGHT_H_ */
