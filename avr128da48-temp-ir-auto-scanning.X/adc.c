/*
 * File:   adc.c
 * Author: M16128
 *
 * Created on June 30, 2020, 5:37 PM
 */

#include "mcc_generated_files/mcc.h"
#include <avr/io.h>
#include <stdbool.h>
#include "adc.h"
#include "config.h"

void VREF0_init(void)
{
    VREF.ADC0REF = VREF_REFSEL_2V048_gc     /* Select 2.048V Reference for ADC */
                 | VREF_ALWAYSON_bm;        /* Select the Always On mode */
}

void ADC0_init(void)
{
    ADC0.CTRLC = ADC_PRESC_DIV4_gc;        /* CLK_PER divided by 4 */
    ADC0.CTRLA = ADC_ENABLE_bm             /* ADC Enable: enabled */
               | ADC_RESSEL_12BIT_gc       /* 12-bit mode */
               | ADC_RUNSTBY_bm            /* Run in Stand-bye mode */
               | ADC_FREERUN_bm;           /* Enable Free-Run mode */
    ADC0.MUXPOS = ADC_MUXPOS_AIN4_gc;      /* Select ADC channel AIN4 <-> PD4 */
    ADC0.WINLT = ADC_LOW_THRESHOLD;     /* Set conversion window comparator low threshold */
    ADC0.WINHT = ADC_HIGH_THRESHOLD;    /* Set conversion window comparator high threshold */
    ADC0.CTRLE = ADC_WINCM_OUTSIDE_gc;     /* Set conversion window mode, outside window */
    
    ADC0.INTCTRL |=  ADC_WCMP_bm;
    ADC0.INTFLAGS |=  ADC_WCMP_bm;
}

void ADC0_start(void)
{
    /* Start ADC conversion */
    ADC0.COMMAND = ADC_STCONV_bm;
}

ISR(ADC0_WCMP_vect)
{
    // empty ISR, do not remove
}