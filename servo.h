/*
 * servo.h
 *
 *  Created on: Nov 5, 2020
 *      Author: Sonikku
 */

#ifndef SERVO_H_
#define SERVO_H_

// Definitions
//------------------

// Function Prototypes
//---------------------
void InitServo(void);
void CommandServo(unsigned int position, unsigned int change_rate);



#endif /* SERVO_H_ */
