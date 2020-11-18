//---------------------------------------------------------------------------------------------------
//       ドラちゃん
//       [DORA-CHAN]
//       ACCESS CONTROL FOR CATS [猫のアクセス制御]
//       https://github.com/jason-mitchell/DoraChan
//       Version 1.0
//       31 Oct '20
//
//----------------------------------------------------------------------------------------------------
#pragma diag_suppress 169       // This compiler.... tsk tsk...

#include <stdio.h>
#include <stdint.h>
#include <msp430.h> 
#include "timer.h"
#include "char_lcd.h"
#include "eeprom.h"
#include "wiegand.h"
#include "backlight.h"
#include "rtc.h"
#include "beep.h"
#include "servo.h"


unsigned char var;
char str[32];
unsigned char tag[4];

signed char encoder = 0;

unsigned int pos;
unsigned int pwm_value;

unsigned char frx_event;
unsigned char button_event;

DateTime datetime;

// Hardware defs
//--------------------

#define SDA_PIN     BIT7            // Bit 7 USCI Port 1(SDA)
#define SCL_PIN     BIT6            // Bit 6 USCI Port 1(SCL)
#define BUTTON BIT3                 // [P1.3]
#define PHASE_A BIT4                // [P2.4]
#define PHASE_B BIT5                // [P2.5]
#define FRX BIT1                    // [P1.1]






//-----------------------------------------------------------------------------------------------------
//                           Main function
//------------------------------------------------------------------------------------------------------
int main(void){
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
    // Configure and calibrate system clock
    //--------------------------------------
    DCOCTL = 0;                                  // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                       // Set range
    DCOCTL = CALDCO_1MHZ;                        // Set DCO step + modulation */
    BCSCTL2 = 0x00;



    // Configure ports
    //------------------
    P1DIR = 0x35;                               // P1.0 thru P1.5 OUTPUTS excepting P1.1 which is an input
    P1REN = 0x0A;                               // Enable pull-up/pull down on P1.3 & P1.1
    P1OUT = 0x0A;                               // Bit 3 & Bit 1 = 1 i.e. pull up

    P1OUT |= LCD_ENA;                           // LCD latch pin high

    P2DIR = 0x0F;                               // Lower 4 bits are outputs
    P2REN = 0xF0;
    P2OUT = 0xF0;                               // Enable pull on P2.4, P2.5, P2.6 and P2.7

    // Configure WDT as systick timer
    //--------------------------------

    WDTCTL = WDT_MDLY_0_5 ;                     // Enable WDT with period of 500uS
    IE1 |= WDTIE;                               // Enable WDT interrupt

    InitServo();

    // Configure USCI for I2C operation
    //----------------------------------------------
    P1SEL |= SDA_PIN + SCL_PIN;                     // Assign I2C pins to USCI_B0
    P1SEL2|= SDA_PIN + SCL_PIN;
    UCB0CTL1 = UCSWRST;                             // Enable SW reset, HOLD USCB in a reset state
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;           // I2C Master, MODE 3 = I2C, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;                  // Use SMCLK, keep SW reset
    UCB0BR0 = 72;                                   // Set I2C master speed  72 gives approx 200Khz clock at 16Mhz
    UCB0BR1 = 0;                                    // Set I2C master speed
    UCB0CTL1 &= ~UCSWRST;                           // Clear SW reset, resume operation


    // Configure other interrupts
    //---------------------------------

    P1IES = 0x0A;                               // High-to-low transition on P1.4 & P1.1
    P1IE = 0x0A;   // 0x0A
    P1IFG = 0x0;                                // Clear interrupts

    P2SEL2 = P2SEL2 & 0x3F;
    P2SEL = P2SEL & 0x3F;                       // P2.7 and P2.6 made GPIO
    P2IES = P2IN & (PHASE_A | PHASE_B);         // P2.4 & P2.5 are interrupts
    P2IES |= 0x40;
    P2IES |= 0x80;                              // Set bits for P2.6 and P2.7 to be high-low interrupt edge
    P2IFG = 0;
    //P2IE = PHASE_A | PHASE_B;
    P2IE = 0xF0;                                // Enable interrupts on all MSB pins
    pos = 0;

    __enable_interrupt();                       // Enable System interrupts

    // Initialise modules
    //---------------------
    InitTimerSystem();
    InitWiegand(WIEGAND_34BIT);                 // Initialise Wiegand for 34 bits
    InitBacklight(15);                          // Initialise backlight to be on for default 15 seconds...


    // Init LCD in 4 bit mode...
    InitLCD();
    ClearLCD();
    putString("DORACHAN");
    NextLine();
    putString("  V0.1  ");
    CommandServo(100, 850);
    Delay(600);
    ClearLCD();
    ResetWiegand();             // Reader transmits some stuff, so clear it before we actually use it...
    // Perform test of EEPROM here...
    char buf[16] = {0};                  //i2c received data buffer
    //I2C_Write_EEProm(0x50, 0x00,"ZZZZ",4);
    //Delay(4);
    I2C_Read_EEProm(0x50, 0x00, buf, 4);
    sprintf(str, "%02X%02X%02X%02X", buf[0], buf[1], buf[2], buf[3]);
    putString(str);
    NextLine();
    buf[0] = 0;
    I2C_Read_EEProm(0x50, 0x02, buf, 4);
    sprintf(str, "%02X%02X%02X%02X", buf[0], buf[1], buf[2], buf[3]);
    putString(str);
    Delay(3000);
    ClearLCD();

    // Check the real-time clock
   RTCCheck();
    //Delay(1000);
   // WriteRTC(0x00, 0x80);           // Write control reg
    //WriteRTC(0x00, 0x01);
   I2C_Read_EEProm(0x50, 0x00, buf, 2); // Dummy read because I2C is obtuse
   Delay(10);
   I2C_Read_EEProm(0x50, 0x00, buf, 4);
   sprintf(str, "%02X%02X%02X%02X", buf[0], buf[1], buf[2], buf[3]);
   NextLine();
   putString(str);
   Delay(100);
   ClearLCD();
   Beep(100);
   Delay(350);
   Beep(100);
   Delay(200);
    for(;;){
        HomeCursor();
        GetDate(&datetime);
        sprintf(str, "%02X:%02X:%02X", datetime.hours, datetime.minutes, datetime.seconds);
        putString(str);
        NextLine();
        if (frx_event == 1){
            putString("FRX");
            frx_event = 0;
        }
        if (button_event == 1){
            putString("BUTTON");
            button_event = 0;
        }
        if (CheckWiegandData() == 1){
            // We have wiegand packet...
            tag[0] = 0;
            tag[1] = 0;
            tag[2] = 0;
            tag[3] = 0;
            GetWiegandData(&tag);
            ResetWiegand();
            BacklightON();
            Beep(100);
            sprintf(str, "%02X%02X%02X%02X", tag[0], tag[1], tag[2], tag[3]);
            putString(str);
        }
        Delay(100);
    }


	for(;;){
	    HomeCursor();
	    pwm_value = 50 + pos / 4;
	    if (pwm_value < 380){
	        CCR1 = pwm_value;
	    }
	    sprintf(str, "Val=%d       ", pos / 4);
	    putString(str);

	    if (CheckWiegandData() == 1){
	        // We have wiegand packet...
	        NextLine();
	        tag[0] = 0;
	        tag[1] = 0;
	        tag[2] = 0;
	        tag[3] = 0;
	        GetWiegandData(&tag);
	        ResetWiegand();
	        BacklightON();
	        sprintf(str, "%02X%02X%02X%02X", tag[0], tag[1], tag[2], tag[3]);
            putString(str);
	    }


	}
}


//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                  INTERRUPT HANDLERS
//---------------------------------------------------------------------------------------------------------------------------------------------------------------

// Port 1 interrupt service routine
// Used for button on P1.3 and FRX input on P1.1
//---------------------------------------------------
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){

    unsigned char iflags = P1IFG;

    if (iflags & FRX){
        frx_event = 1;
    }
    if (iflags & BUTTON){
        BacklightON();
        button_event = 1;
    }


    P1IFG = 0;
}

// Port 2 interrupt service routine
// Used for rotary encoder
//----------------------------------------------------
#pragma vector=PORT2_VECTOR
__interrupt void port2_ISR(void){

    unsigned char state = P2IN & (PHASE_A | PHASE_B);
    unsigned char iflags = P2IFG;

    switch(iflags & (PHASE_A | PHASE_B)){

    case (PHASE_B):
        if (P2IES & PHASE_B) {       // if falling edge on Line B
            if (state & PHASE_A) {   // Transitioned from State 3 to State 2
                pos += 1;
            } else {
                pos -= 1;           // Transitioned from State 1 to State 0
            }
        } else {                      // else rising edge on Line B
            if (state & PHASE_A) {   // Transitioned from State 2 to State 3
                pos -= 1;
            } else {                  // Transitioned from State 0 to State 1
               pos += 1;
            }
        }

        P2IES ^= PHASE_B;
        BacklightON();
        break;

    case (PHASE_A):
        if (P2IES & PHASE_A) {       // if falling edge on Line A
            if (state & PHASE_B) {   // Transitioned from State 3 to State 1
                pos -= 1;
            } else {
                pos += 1;           // Transitioned from State 2 to State 0
            }
         } else {                      // else rising edge on Line A
             if (state & PHASE_B) {   // Transitioned from State 1 to State 3
                 pos += 1;
             } else {                  // Transitioned from State 0 to State 2
                 pos -= 1;
             }

         }

         P2IES ^= PHASE_A;
         BacklightON();
        break;


    default:
        break;
    }

    // Check other interrupt sources i.e. P2.6 and P2.7 which form the WIEGAND receiver
    switch(iflags & 0xC0){

    case 0x80:
        Wiegand_B();

        break;


    case 0x40:
        Wiegand_A();

        break;


    }


   P2IFG = 0;


}


// Watchdog Timer interrupt service routine
// Used here as a system timer
// Period is 65.6 uS
//---------------------------------------------------
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void){

    CallInISR();
    BLTimerISR();
    BeepTick();


}
