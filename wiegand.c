//-------------------------------------------------------------------------------------------------------------
//                  Filename: Wiegand.c
//                 Wiegand protocol receiver
//                 Author: Sonikku2k
//
//--------------------------------------------------------------------------------------------------------------
#include <msp430.h>
#include "wiegand.h"


unsigned char w_data [4];           // Wiegand data storage
unsigned char bitcount = 0;         // Bit counter
unsigned char bytecount = 0;
unsigned char w_mode = 0;           // Wiegand mode {26 or 34}
unsigned char rxreg = 0;
unsigned char parity = 0;           // Check for parity
unsigned char dataflag = 0;        // Flag to indicate complete Wiegand packet received...


// Pulse reception routines called by interrupt handlers
//---------------------------------------------------------
// A-phase i.e. '1' bits
void Wiegand_A(void){
    if (dataflag == 0){
        if (parity == 0){
            parity = 1;
            return;
        }
        // We're past the leading parity bit, begin shift
        rxreg = rxreg <<  1;
        rxreg |= 0x01;                  // Set LSB
        bitcount++;
        if (bitcount == 8){
            bitcount = 0;
            w_data[bytecount] = rxreg;
            bytecount++;
        }
        if (bytecount == 4){
            dataflag = 1;
        }
    }

}

// B-phase i.e. '0' bits
void Wiegand_B(void){
    if (dataflag == 0){
        if (parity == 0){
            parity = 1;
            return;
        }
        // We're past the leading parity bit, begin shift
        rxreg = rxreg << 1;
        rxreg &= ~0x01;                  // Clear LSB
        bitcount++;
        if (bitcount == 8){
            bitcount = 0;
            w_data[bytecount] = rxreg;
            bytecount++;
        }
        if (bytecount == 4){
            dataflag = 1;
        }
    }

}

//-------------------------------------------------------------
// Name: InitWiegand
// Setup Wiegand reception and format
//-------------------------------------------------------------
void InitWiegand(unsigned char mode){
    w_mode = mode;
    w_data[0] = 0;
    w_data[1] = 0;
    w_data[2] = 0;
    w_data[3] = 0;
    bitcount = 0;
    bytecount = 0;
    rxreg = 0;
    parity = 0;
    dataflag = 0;

}

//---------------------------------------------------------------
// Name: ResetWiegand
// Function: Reset receiver to be ready for a new packet
//---------------------------------------------------------------
void ResetWiegand(void){

    w_data[0] = 0;
    w_data[1] = 0;
    w_data[2] = 0;
    w_data[3] = 0;
    bitcount = 0;
    bytecount = 0;
    rxreg = 0;
    parity = 0;
    dataflag = 0;
}

//---------------------------------------------------------------
// Name: CheckWiegandData
// Function: Check to see if a full Wiegand packet has been received
//---------------------------------------------------------------
unsigned char CheckWiegandData(void){
    return dataflag;
}

//---------------------------------------------------------------
// Name: GetWiegandData
// Function: Retrieve data from buffer
//---------------------------------------------------------------
void GetWiegandData(unsigned char *data){
    data[3] = w_data[0];
    data[2] = w_data[1];
    data[1] = w_data[2];
    data[0] = w_data[3];

}
