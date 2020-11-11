//------------------------------------------------------------------------------------
//                      rtc.c
//              RTC library for DS1307 on I2C bus shared with EEPROM
//                  November 2020
//
// 3 November 2020 - Routines work well.. if they get stuck the RTC has bad I2C contact
//--------------------------------------------------------------------------------------

#include <msp430.h>
#include "rtc.h"
#include "beep.h"

#define DS1307_SLAVE_ADDR 0x68



unsigned char tmp;




//-------------------------------------------------------------------
// Name: GetDate
// Function: Retrieve time and data
// Parameters: Pointer to struct of type DateTime
// Returns: void
//--------------------------------------------------------------------
void GetDate(DateTime *dt){

    dt->seconds = ReadRTC(RTC_SECONDS);
    dt->minutes = ReadRTC(RTC_MINUTES);
    dt->hours = ReadRTC(RTC_HOURS);
    dt->day = ReadRTC(RTC_DAY);
    dt->date = ReadRTC(RTC_DATE);
    dt->month = ReadRTC(RTC_MONTH);
    dt->year = ReadRTC(RTC_YEAR);



}

//-------------------------------------------------------------------
// Name: ReadRTC
// Function: Read specified address in DS1307
// Parameters: Register to be read
// Returns: Value read from the register
//--------------------------------------------------------------------
unsigned char ReadRTC(unsigned char register_address){

    unsigned char read_data;

    SetSemaphore(1);
    while (UCB0STAT & UCBBUSY);                     // wait for USCI B0 bus to be inactive
    UCB0I2CSA = DS1307_SLAVE_ADDR;                  // set SLAVE address

    // Address byte
    UCB0CTL1 |= UCTR + UCTXSTT;                     // set USCI to be I2C TX,  send start condition
    UCB0TXBUF = register_address;                   // transmit register address

    //while (UCB0CTL1 & UCTXSTT);                     // waiting for slave address to transfer and ACK
    while ((IFG2 & UCB0TXIFG) != UCB0TXIFG);        // wait for TX IFG to become '1' which means the transfer is complete


    UCB0I2CSA = DS1307_SLAVE_ADDR;                  // set SLAVE address

    UCB0CTL1 &= ~UCTR;                              // set USCI to be RECEIVER
    UCB0CTL1 |= UCTXSTT;                            // send restart
    while (UCB0CTL1 & UCTXSTT);                     // wait until I2C STT is sent

    // The read is started, now reading can proceed..
    while ((IFG2 & UCB0RXIFG) != UCB0RXIFG);        // wait for RX buffer to have data
    tmp = UCB0RXBUF;                                // 1st read is garbage usually..

    while ((IFG2 & UCB0RXIFG) != UCB0RXIFG);        // wait for RX buffer to have data
    read_data = UCB0RXBUF;                          // Get the data



    UCB0CTL1 |= UCTXNACK;                           // generate a NACK
    UCB0CTL1 |= UCTXSTP;                            // generate a stop condition
    SetSemaphore(0);
    return read_data;

}

//-------------------------------------------------------------------
// Name: WriteRTC
// Function: Write specified address in DS1307
// Parameters: Register to be read, value to be written
// Returns: void
//--------------------------------------------------------------------
void WriteRTC(unsigned char address, unsigned char value){

    SetSemaphore(1);
    while (UCB0STAT & UCBBUSY);                             // wait for USCI B0 bus to be inactive
    UCB0I2CSA = DS1307_SLAVE_ADDR;;                         // set SLAVE address
    UCB0CTL1 |= UCTR + UCTXSTT;                             // Set USCI to be I2C TX,  send start condition
    UCB0TXBUF = address;                                    // Set register address

    while (UCB0CTL1 & UCTXSTT);                             // waiting for slave address to be transferred
    while ((IFG2 & UCB0TXIFG) != UCB0TXIFG);                // wait for TX IFG to clear


    UCB0TXBUF = value;                                      // send data byte to be written
    while ((IFG2 & UCB0TXIFG) != UCB0TXIFG);               // wait for TX IFG to clear
    UCB0CTL1 |= UCTXSTP;                                   // set I2C stop condition
    while ((UCB0STAT & UCSTPIFG) == UCSTPIFG);             // wait for Stop condition to be set

    SetSemaphore(0);

}

//----------------------------------------------------------------------------------------------
// Name: RTCCheck
// Function: Check the RTC for valid time, if invalid the MSB of seconds register is set
//           and the clock will not run
// Parameters: void
// Returns: void
//-----------------------------------------------------------------------------------------------
void RTCCheck(void){

    unsigned char reg;
    SetSemaphore(1);
    reg = ReadRTC(RTC_SECONDS);
    if (reg == 0x80){
        // CH bit is set, we have a reset clock
        // Write the seconds register to let the clock start
        WriteRTC(RTC_SECONDS, 0x01);
        WriteRTC(0x07, 0x00);           // SQW pin disabled
    }
    SetSemaphore(0);
}



