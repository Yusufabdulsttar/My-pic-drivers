/* 
 * File:   mcal_adc.h
 * Author: yusuf
 *
 * Created on May 1, 2023, 4:48 PM
 */

#ifndef MCAL_ADC_H
#define	MCAL_ADC_H

/* section : Includes   */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "mcal_adc_cfg.h"
#include "../Interrupt/mcal_internal_interrupt.h"
/* section : Micro Declaration  */

#define ADC_VOLT_REF  1
#define ADC_VOLT_MC   0

/* section : Micro Function Declaration  */

#define ADC_ENABLE()   (ADCON0bits.ADON = 1)
#define ADC_DISABLE()  (ADCON0bits.ADON = 0)

#define ADC_START()     (ADCON0bits.GODONE = 1)

#define ENABLE_VOLT_REF()      do{(ADCON1bits.VCFG1 = 1);(ADCON1bits.VCFG0 = 1);}while(0)
#define DISABLE_VOLT_REF()     do{(ADCON1bits.VCFG1 = 0);(ADCON1bits.VCFG0 = 0);}while(0)

#define adc_conf_analog_pins(value)    (ADCON1bits.PCFG = (value))

#define RESULT_RIGHT_FORMAT()   (ADCON2bits.ADFM = 1)
#define RESULT_LEFT_FORMAT()    (ADCON2bits.ADFM = 0)


/* section : Data Type Declaration  */

typedef enum {
/*an0-11 this mean (from an0 to an11) is analog pin*/
    ADC_ALL_ANALOG_FUNCTION = 0,
    ADC_AN0_11_ANALOG_FUNCTION = 3,
    ADC_AN0_10_ANALOG_FUNCTION,
    ADC_AN0_9_ANALOG_FUNCTION, 
    ADC_AN0_8_ANALOG_FUNCTION,
    ADC_AN0_7_ANALOG_FUNCTION,
    ADC_AN0_6_ANALOG_FUNCTION,
    ADC_AN0_5_ANALOG_FUNCTION,             
    ADC_AN0_4_ANALOG_FUNCTION,
    ADC_AN0_3_ANALOG_FUNCTION,
    ADC_AN0_2_ANALOG_FUNCTION,
    ADC_AN0_1_ANALOG_FUNCTION,
    ADC_AN0_0_ANALOG_FUNCTION,
    ADC_ALL_DIGITAL_FUNCTION
}adc_set_pins_analog_t; 

typedef enum {
    ADC_CHANNEL_AN0,
    ADC_CHANNEL_AN1,        
    ADC_CHANNEL_AN2,
    ADC_CHANNEL_AN3,
    ADC_CHANNEL_AN4, 
    ADC_CHANNEL_AN5,
    ADC_CHANNEL_AN6,
    ADC_CHANNEL_AN7,
    ADC_CHANNEL_AN8,             
    ADC_CHANNEL_AN9,
    ADC_CHANNEL_AN10,
    ADC_CHANNEL_AN11,
    ADC_CHANNEL_AN12             
}adc_select_channel_t; 

         
typedef enum {
    ADC_0_TAD,
    ADC_2_TAD,
    ADC_4_TAD,
    ADC_6_TAD,
    ADC_8_TAD,
    ADC_12_TAD,
    ADC_16_TAD,
    ADC_20_TAD,            
}adc_acquisition_time_t; 
         
typedef enum {
    ADC_div_FOSC_2,
    ADC_div_FOSC_8,
    ADC_div_FOSC_32,
    ADC_MC_OSC_FRC,            
    ADC_div_FOSC_4,
    ADC_div_FOSC_16,            
    ADC_div_FOSC_64                 
}adc_conv_clock_t; 

typedef struct {
    adc_select_channel_t channel;
#ifdef ADC_INTERRUPT    
    void (* EXT_INTERRUPT)(void);
#ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE 
    Interrupt_priority_t priority;
#endif
#endif
    adc_acquisition_time_t acq_time;
    adc_conv_clock_t conv_clock;
    adc_set_pins_analog_t set_pins_analog;
    uint8 conf_volt_ref : 1;
}adc_cfg_t;


/* section : Function Declaration  */

Std_ReturnType adc_init (const adc_cfg_t*adc);
Std_ReturnType adc_deinit (const adc_cfg_t*adc);
Std_ReturnType adc_conv_value (const adc_cfg_t*adc,const adc_select_channel_t channel,uint16 *value);
Std_ReturnType adc_select_channel (const adc_select_channel_t channel);
#endif	/* MCAL_ADC_H */

