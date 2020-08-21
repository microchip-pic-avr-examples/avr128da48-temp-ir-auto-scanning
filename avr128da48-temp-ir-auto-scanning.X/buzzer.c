/*
 * File:   buzzer.c
 * Author: M16128
 *
 * Created on June 29, 2020, 2:07 PM
 */

#include "mcc_generated_files/mcc.h"
#include <util/delay.h>
#include "buzzer.h"

#define TCD_PRESCALLER                  32
#define BUZZER_PER_COMPUTE(FREQ)        (uint16_t)(((F_CPU/TCD_PRESCALLER)/(FREQ))/2-1)

void Buzzer_Sound(sound_t type)
{
    switch (type)
    {
        case BEEP_ERROR:
            Buzzer_Beep(BUZZER_PER_COMPUTE(400),300);
            Buzzer_Beep(BUZZER_PER_COMPUTE(600),300);
            Buzzer_Beep(BUZZER_PER_COMPUTE(400),300);
            Buzzer_Beep(BUZZER_PER_COMPUTE(600),300);
            Buzzer_Beep(BUZZER_PER_COMPUTE(400),300);
            break;
        case BEEP_DENIED:
            Buzzer_Beep(BUZZER_PER_COMPUTE(300),300);
            Buzzer_Beep(BUZZER_PER_COMPUTE(400),300);
            Buzzer_Beep(BUZZER_PER_COMPUTE(300),300);
            Buzzer_Beep(BUZZER_PER_COMPUTE(400),300);
            Buzzer_Beep(BUZZER_PER_COMPUTE(300),300);
            break;
        case BEEP_ALLOWED:
            Buzzer_Beep(BUZZER_PER_COMPUTE(1000),100);
            Buzzer_Beep(BUZZER_PER_COMPUTE(2000),200);
            Buzzer_Beep(BUZZER_PER_COMPUTE(1000),100);
            Buzzer_Beep(BUZZER_PER_COMPUTE(2000),200);
            Buzzer_Beep(BUZZER_PER_COMPUTE(1000),100);
            Buzzer_Beep(BUZZER_PER_COMPUTE(2000),100);
            break;
        case BEEP_DETECTED:
            Buzzer_Beep(BUZZER_PER_COMPUTE(3000),100);
            Buzzer_Beep(BUZZER_PER_COMPUTE(2000),100);
            Buzzer_Beep(BUZZER_PER_COMPUTE(3000),100);
            break;
        default:
            break;    
    }       
}

// Per = period computed using BUZZER_PER_COMPUTE macro , duration in milli-seconds
void Buzzer_Beep(uint16_t per, uint16_t duration)
{
    TCD0.CTRLA = 0x11;
    TCD0.CMPASET = per>>1;
    TCD0.CMPBCLR = per;
    TCD0.CMPBSET = per>>1;
    while(duration--)
        _delay_ms(1);
    TCD0.CTRLA = 0x10;
}