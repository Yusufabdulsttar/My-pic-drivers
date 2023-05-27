/* 
 * File:   mcal_timer2.c
 * Author: yusuf
 *
 * Created on May 4, 2023, 5:57 PM
 */
#include "mcal_timer2.h"

#ifdef TIMER2_INTERRUPT 
static void(* TIMER2_INTHANDLER)(void) = NULL;
#endif
static uint8 preload2_value = 0;

Std_ReturnType timer2_init(const timer2_t *timer2){
    Std_ReturnType ret = E_OK;
    if (NULL == timer2){
        ret = E_NOT_OK;
    }
    else {
        /*disable timer0*/
        TIMER2_DISABLE();
        /*prescaler value*/
        T2CONbits.T2CKPS = timer2->prescale_value;
        /*postscaler value*/
        T2CONbits.TOUTPS = timer2->post_scale_value;
        /*preload value */
        TMR2 = timer2->preload_value;
        preload2_value = timer2->preload_value;
        /*interrupt configuration*/
#ifdef TIMER2_INTERRUPT 
        TIMER2_INTERRUPT_FlagClear();
        TIMER2_INTERRUPT_ENABLE();
        TIMER2_INTHANDLER = timer2->EXT_INTERRUPT;
#ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE 
        INTERRUPT_PriorityLevelEnable();
        if (PRIORITY_HIGH == timer2->priority){
            INTERRUPT_GlobalHighEnable();
            TIMER2_INTERRUPT_PriortyHigh();
        }
        else if(PRIORITY_LOW == timer2->priority){
            INTERRUPT_GlobalLowEnable();
            TIMER2_INTERRUPT_PriortyLow();
        } 
#else
        INTERRUPT_GlobalEnabl();
        INTERRUPT_PeripheralEnable();
#endif
#endif
        /*enable timer0*/
        TIMER2_ENABLE();
    }
    return ret;   
}

Std_ReturnType timer2_Deinit(const timer2_t *timer2){
    Std_ReturnType ret = E_OK;
    if (NULL == timer2){
        ret = E_NOT_OK;
    }
    else {
        /*disable timer0*/
        TIMER2_DISABLE();
        /*disable interrupt future*/
#ifdef TIMER2_INTERRUPT
        TIMER2_INTERRUPT_FlagClear();
        TIMER2_INTERRUPT_DISABLE();
#endif
    }
    return ret;  
}

Std_ReturnType timer2_Read (const timer2_t *timer2, uint8 *value){
    Std_ReturnType ret = E_OK;
    if (NULL == timer2){
        ret = E_NOT_OK;
    }
    else {
         *value = TMR2;
    }
    return ret; 
}

Std_ReturnType timer2_write (const timer2_t *timer2, uint8 value){
    Std_ReturnType ret = E_OK;
    if (NULL == timer2){
        ret = E_NOT_OK;
    }
    else {
         TMR2 = value;
    }
    return ret; 
}

void TIMER2_ISR (void ){
    #ifdef TIMER2_INTERRUPT
    TIMER2_INTERRUPT_FlagClear();
   /*code*/
    TMR2 = preload2_value;

    if(TIMER2_INTHANDLER) {
        TIMER2_INTHANDLER();
    }
    #endif
}
