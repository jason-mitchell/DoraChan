/*
 * rtc.h
 *
 *  Created on: Nov 3, 2020
 *      Author: Sonikku
 */

#ifndef RTC_H_
#define RTC_H_

// Definitions
//-------------
#define RTC_SECONDS 0
#define RTC_MINUTES 1
#define RTC_HOURS 2
#define RTC_DAY 3
#define RTC_DATE 4
#define RTC_MONTH 5
#define RTC_YEAR 6
#define RTC_CONTROL 7
#define RTC_RAM_START 8
#define RTC_RAM_END 0x3F

#define Sunday 1
#define Monday 2
#define Tuesday 3
#define Wednesday 4
#define Thursday 5
#define Friday 6
#define Saturday 7

typedef struct{
    unsigned char seconds;
    unsigned char minutes;
    unsigned char hours;
    unsigned char day;
    unsigned char date;
    unsigned char month;
    unsigned char year;

}DateTime;

// Function Prototypes
//---------------------

unsigned char ReadRTC(unsigned char address);
void WriteRTC(unsigned char address, unsigned char value);
void RTCCheck(void);
void GetDate(DateTime *dt);

#endif /* RTC_H_ */
