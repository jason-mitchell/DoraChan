/*
 * wiegand.h
 *
 *  Created on: Nov 1, 2020
 *      Author: Sonikku
 */

#ifndef WIEGAND_H_
#define WIEGAND_H_

// Defs
//---------------------

#define WIEGAND_26BIT 0
#define WIEGAND_34BIT 1

// Function prototypes
void Wiegand_A(void);
void Wiegand_B(void);
void InitWiegand(unsigned char mode);
void ResetWiegand(void);
unsigned char CheckWiegandData(void);
void GetWiegandData(unsigned char *data);




#endif /* WIEGAND_H_ */
