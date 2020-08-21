/*
 * File:   motion.c
 * Author: M16128
 *
 * Created on June 30, 2020, 5:27 PM
 */


#include <avr/io.h>
#include <stdbool.h>
#include "adc.h"

void Motion_Init(void)
{
    VREF0_init();
    ADC0_init();
    ADC0_start();
}

bool Motion_Check(void)
{
    if (ADC0.INTFLAGS & ADC_WCMP_bm)
        return true;
    else
        return false;
}

void Motion_Detect_ON(void)
{
    ADC0.INTFLAGS = ADC_WCMP_bm;      // clear interrupt flag
    ADC0.INTCTRL |=  ADC_WCMP_bm;     // enable future interrupts 
}

void Motion_Detect_OFF(void)
{
    ADC0.INTCTRL &= ~ADC_WCMP_bm;     // disable future interrupts 
}