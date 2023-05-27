/* 
 * File:   mcal_adc.?
 * Author: yusuf
 *
 * Created on May 1, 2023, 4:48 PM
 */
#include "mcal_adc.h"
#ifdef ADC_INTERRUPT
static void (* ADC_INTHANDLER) (void) = NULL;
#endif
Std_ReturnType adc_init (const adc_cfg_t*adc){
    Std_ReturnType ret = E_OK;
    if (NULL == adc){
        ret = E_NOT_OK;
    }
    else {
        /*disable ADC */
        ADC_DISABLE();
        /*conf analog pins*/
        adc_conf_analog_pins(adc->set_pins_analog);
        /*conf volt ref*/
        if(ADC_VOLT_REF == (adc->conf_volt_ref)){
           ENABLE_VOLT_REF();
        }
        else {
           DISABLE_VOLT_REF(); 
        }
        /*configure interrupt future */
#ifdef ADC_INTERRUPT
        ADC_INTERRUPT_FlagClear();
        ADC_INTERRUPT_ENABLE();
        ADC_INTHANDLER = adc->EXT_INTERRUPT;
#ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE 
        INTERRUPT_PriorityLevelEnable();
        if (PRIORITY_HIGH == adc->priority){
            INTERRUPT_GlobalHighEnable();
            ADC_INTERRUPT_PriortyHigh();
        }
        else if(PRIORITY_LOW == adc->priority){
            INTERRUPT_GlobalLowEnable();
            ADC_INTERRUPT_PriortyLow();
        } 
#else
        INTERRUPT_GlobalEnabl();
        INTERRUPT_PeripheralEnable();
#endif
#endif
        /*result format*/
        RESULT_RIGHT_FORMAT();
        /*select channel*/
        adc_select_channel (adc->channel);
        /*select acquisition time*/
        ADCON2bits.ACQT = adc->acq_time;
        /*select conversion clock*/
        ADCON2bits.ADCS = adc->conv_clock;
        /*Turn on ADC*/
        ADC_ENABLE();
    }
    return ret; 
}


Std_ReturnType adc_select_channel (const adc_select_channel_t channel){
    Std_ReturnType ret = E_OK;
    ADCON0bits.CHS = channel;        /*select channel*/
    switch(channel){                 /*conf the pins to be input*/
    case ADC_CHANNEL_AN0  : SET_BIT(TRISA, PIN0); break;
    case ADC_CHANNEL_AN1  : SET_BIT(TRISA, PIN1); break;     
    case ADC_CHANNEL_AN2  : SET_BIT(TRISA, PIN2); break;
    case ADC_CHANNEL_AN3  : SET_BIT(TRISA, PIN3); break;
    case ADC_CHANNEL_AN4  : SET_BIT(TRISA, PIN5); break;
    case ADC_CHANNEL_AN5  : SET_BIT(TRISE, PIN0); break;
    case ADC_CHANNEL_AN6  : SET_BIT(TRISE, PIN1); break;
    case ADC_CHANNEL_AN7  : SET_BIT(TRISE, PIN2); break;
    case ADC_CHANNEL_AN8  : SET_BIT(TRISB, PIN2); break;
    case ADC_CHANNEL_AN9  : SET_BIT(TRISB, PIN3); break;
    case ADC_CHANNEL_AN10 : SET_BIT(TRISB, PIN1); break;
    case ADC_CHANNEL_AN11 : SET_BIT(TRISB, PIN4); break;
    case ADC_CHANNEL_AN12 : SET_BIT(TRISB, PIN0); break;
    }
    return ret; 
}


Std_ReturnType adc_deinit (const adc_cfg_t*adc){
    Std_ReturnType ret = E_OK;
    if (NULL == adc){
        ret = E_NOT_OK;
    }
    else {
        ADC_DISABLE();
#ifdef ADC_INTERRUPT
        ADC_INTERRUPT_FlagClear();
        ADC_INTERRUPT_DISABLE();
#ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE 
        INTERRUPT_PriorityLevelDisable();
#endif
#endif
    }
    return ret;  
}


Std_ReturnType adc_conv_value (const adc_cfg_t*adc,const adc_select_channel_t channel,uint16 *value){
    Std_ReturnType ret = E_OK;
    if (NULL == adc){
        ret = E_NOT_OK;
    }
    else {
        /*select channel*/
        adc_select_channel(channel);
        /*Start conversion ( interrupt flag is set if adc interrupt is enable )*/
        ADC_START();         
        /*Wait until conversion is done*/
        while(ADCON0bits.GODONE);
        /*result*/
        *value = (uint16)((ADRESH * 256) + ADRESL);
    }
    return ret; 
}

void ADC_ISR (void ){
    #ifdef ADC_INTERRUPT
    ADC_INTERRUPT_FlagClear();
   /*code*/
    if(ADC_INTHANDLER) {
        ADC_INTHANDLER();
    }
    #endif
    
}
