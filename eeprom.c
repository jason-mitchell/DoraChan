//-------------------------------------------------------------------------------------------------------------
//                  Filename: eeprom.c
//                 Use MSP430 I2C module to read/write 24LCxx series EEPROM
//                 Author: Sonikku2k
//
//--------------------------------------------------------------------------------------------------------------

#include <msp430.h>
#include "eeprom.h"
#include "beep.h"


// Notes:
// Original code was intended to access large EEPROM where the address is 16 bits.
// Also the original author tried to do page writes, unsuccessfully (doesn't understand how serial EEPROM work)
// It is modified for the 24LC02 and other small EEPROM
//-------------------------------------------------------------------------------------------------------------



//--------------------------------------------------
// EEPROM read function
// Address is unsigned int (16 bits) but only lower 8 bits (1 byte) is sent
//--------------------------------------------------
void I2C_Read_EEProm(unsigned char slave_address, unsigned int memory_address, char * data, unsigned char DataLength ){
    SetSemaphore(1);
    int rLoop = 0;  //loop counter
    while (UCB0STAT & UCBBUSY);                     // wait for USCI B0 bus to be inactive
    UCB0I2CSA = slave_address;                      // set SLAVE address

    // Address byte
    UCB0CTL1 |= UCTR + UCTXSTT;                     // set USCI to be I2C TX,  send start condition
 //   UCB0TXBUF = (memory_address & 0x7F00) >> 8;     // transfer memory_address MSB
    UCB0TXBUF = (memory_address & 0x00FF);

    while (UCB0CTL1 & UCTXSTT);                     // waiting for slave address to transfer
    while ((IFG2 & UCB0TXIFG) != UCB0TXIFG);        // wait for TX IFG to clear

 //   UCB0TXBUF = (memory_address & 0x00FF);          // transfer memory_address LSB
 //   while ((IFG2 & UCB0TXIFG) != UCB0TXIFG);        // wait for TX IFG to clear

    UCB0CTL1 &= ~UCTR;                              // set USCI to be RECEIVER
    UCB0CTL1 |= UCTXSTT;                            // send restart
    while (UCB0CTL1 & UCTXSTT);                     // wait until I2C STT is sent

    // Data read loop
    for (rLoop = 0; rLoop < DataLength; rLoop++){
         while ((IFG2 & UCB0RXIFG) != UCB0RXIFG);   // wait for RX buffer to have data
            data[rLoop] = UCB0RXBUF;                // Move rvcd data of or USCI buffer. This also clears the UCB0RXIFG flag

            if (rLoop == DataLength - 2){            // NACK and STOP must be send before the last byte is read from the buffer.
                                                     // if not the CPU will read an extra byte.
                UCB0CTL1 |= UCTXNACK;                // generate a NACK
                UCB0CTL1 |= UCTXSTP;                 // generate a stop condition
            }
    }

    SetSemaphore(0);
}

//---------------------------------------------------------------------------------------
// EEPROM Write function
// Changed to write single bytes only but will write the buffer passed as argument
// This makes it slower but guarantees data integrity
//---------------------------------------------------------------------------------------
void I2C_Write_EEProm(unsigned char slave_address, unsigned int memory_address, char * data, unsigned char DataLength ){

    unsigned int address = memory_address;
    unsigned int selector = 0;
    SetSemaphore(1);
    for(selector = 0; selector < DataLength; selector++){
        __disable_interrupt(); //prevent interrupts from messing with the I2C functions

        while (UCB0STAT & UCBBUSY);                             // wait for USCI B0 bus to be inactive
        UCB0I2CSA = slave_address;                              // set SLAVE address
        UCB0CTL1 |= UCTR + UCTXSTT;                             // Set USCI to be I2C TX,  send start condition
        UCB0TXBUF = (address & 0x00FF);                         // Set memory address

        while (UCB0CTL1 & UCTXSTT);                             // waiting for slave address to be transferred
        while ((IFG2 & UCB0TXIFG) != UCB0TXIFG);                // wait for TX IFG to clear

    //    UCB0TXBUF = (address & 0x00FF);     //transferring memory_address LSB
    //    while ((IFG2 & UCB0TXIFG) != UCB0TXIFG);        //wait for TX IFG to clear


        UCB0TXBUF = data[selector];                            // send data byte to be written
        while ((IFG2 & UCB0TXIFG) != UCB0TXIFG);               // wait for TX IFG to clear
        UCB0CTL1 |= UCTXSTP;                                   // set I2C stop condition
        while ((UCB0STAT & UCSTPIFG) == UCSTPIFG);             // wait for Stop condition to be set

        WriteTimer();                                          // Wait write time

        __enable_interrupt();
        address++;
    }
    SetSemaphore(0);
}


//----------------------------------------------------------------------
// Name: WriteTimer
// Function: Software timer for timing the E2PROM WRITE CYCLE
// Parameters: none
// Returns: nil
//---------------------------------------------------------------------
void WriteTimer(void){
    unsigned int r = 0;
    for (r = 0; r < 1500; r++){
        __delay_cycles(10);
    }
}



