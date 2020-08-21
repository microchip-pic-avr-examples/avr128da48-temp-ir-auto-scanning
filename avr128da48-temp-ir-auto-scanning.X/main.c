/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "adc.h"
#include "buzzer.h"
#include "config.h"
#include "i2c.h"
#include "spi_basic.h"
#include "oledC_shapes.h"
#include "oledC_click.h"
#include "oledC_colors.h"
#include "motion.h"
#include "temperature.h"


/* If valid_temp is true, OLED will display in GREEN, RED otherwise*/
void Main_Display (const char *string1, const char *string2, double temperature, bool valid_temp)
{
    char temp_string[16];
    oledC_clearScreen();
    printf("\n\r %s ",string1);
    printf("%s",string2);
    strcpy(temp_string, string1);
    oledC_printString(temp_string,OLEDC_COLOR_WHITE,0);
    strcpy(temp_string, string2);
    oledC_printString(temp_string,OLEDC_COLOR_WHITE,1);
    if(temperature > TEMP_ABSOLUTE_ZERO)
    {
        _delay_ms(ONE_SECOND);
        sprintf(temp_string,"%.1f C",temperature);
        printf("\n\r %s ",temp_string);
        if (valid_temp)
            oledC_printString(temp_string,OLEDC_COLOR_GREEN,2);
        else
            oledC_printString(temp_string,OLEDC_COLOR_RED,2);
    }
}

int main(void)
{
    double max_temp,t_amb,t_obj;
    uint16_t n=0;
    
    /* Initializes MCU, drivers and middle-ware */
    SYSTEM_Initialize();
    I2C_0_Init();
    SPI_init();
    Motion_Init();
    oledC_setup();
    oledC_clearScreen();

    ENABLE_INTERRUPTS();
    
    while (1)
    {
        if(Motion_Check() == true)
        {
            Motion_Detect_OFF();
            Buzzer_Sound(BEEP_DETECTED);
            Main_Display("Motion","detected",TEMP_HIDDEN,0);
            _delay_ms(ONE_SECOND);
            Main_Display("Step in","front of",TEMP_HIDDEN,0);
            _delay_ms(ONE_SECOND);
            Main_Display("the round","sensor",TEMP_HIDDEN,0);
            _delay_ms(ONE_SECOND);
            n = TIMEOUT_MEAS;
            do
            {
                t_amb = Temperature_ReadAmbient();
                t_obj = Temperature_ReadObject();
                if ( (t_obj-t_amb) > TEMP_DIFF)
                    break;
                _delay_ms(MEASURE_INTERVAL);
            }
            while(--n);      //decrement REPEATED_MEAS until = 0
            if   (n==0)   //timeout after REPEATED_MEAS*MEASURE_INTERVAL waiting
            {
                Main_Display("No person","detected",TEMP_HIDDEN,0);
                _delay_ms(THRE_SECONDS);
            }
            else
            {
                Main_Display("Measuring","stay still",TEMP_HIDDEN,0);
                _delay_ms(ONE_SECOND);
                max_temp = Temperature_MaxObj(REPEATED_MEAS);
                if ((max_temp > TEMP_HIGH) || (max_temp < TEMP_LOW))
                {
                    Main_Display("Temp out","of range:",max_temp,0);
                    _delay_ms(ONE_SECOND);
                    Main_Display("Access","denied",max_temp,0);
                    Buzzer_Sound(BEEP_ERROR);
                    _delay_ms(THRE_SECONDS);
                }
                else if (max_temp > TEMP_GOOD)
                {
                    Main_Display("Measured","Temp>37:",max_temp,0);
                    _delay_ms(ONE_SECOND);
                    Main_Display("Access","denied",max_temp,0);
                    Buzzer_Sound(BEEP_DENIED);
                    _delay_ms(THRE_SECONDS); 
                }
                else
                {
                    Main_Display("Temp is","normal:",max_temp,1);
                    _delay_ms(ONE_SECOND);
                    Main_Display("Door","opened",TEMP_HIDDEN,0);
                    Buzzer_Sound(BEEP_ALLOWED);
                    DOOR_SetLow();              //LED_ON  -> Door Open
                    _delay_ms(THRE_SECONDS);
                    DOOR_SetHigh();             //LED_OFF -> Door Closed
                    Main_Display("Door","closed",TEMP_HIDDEN,0);
                    _delay_ms(THRE_SECONDS);
                }
            }
            oledC_clearScreen();
            Motion_Detect_ON();
        }
        sleep_mode();
    }
}

/**
    End of File
*/