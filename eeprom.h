/*
 * eeprom.h
 *
 *  Created on: Oct 31, 2020
 *      Author: Sonikku
 */

#ifndef EEPROM_H_
#define EEPROM_H_


// Definitions
//--------------------
void I2C_Write_EEProm(unsigned char slave_address, unsigned int register_address, char * data, unsigned char DataLength );
void I2C_Read_EEProm(unsigned char slave_address, unsigned int memory_address, char * data, unsigned char DataLength );
void WriteTimer(void);
void ResetI2C(void);



// Function Prototypes
//--------------------

#endif /* EEPROM_H_ */
