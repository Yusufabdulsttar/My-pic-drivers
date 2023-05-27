/* 
 * File:   mcal_timer3.c
 * Author: yusuf
 *
 * Created on May 4, 2023, 7:26 PM
 */
#include "mcal_timer3.h"

#ifdef TIMER3_INTERRUPT 
static void(* TIMER3_INTHANDLER)(void) = NULL;
#endif
static uint16 preload3_value = 0;

Std_ReturnType timer3_init(const timer3_t *timer3){
    Std_ReturnType ret = E_OK;
    if (NULL == timer3){
        ret = E_NOT_OK;
    }
    else {
        /*disable timer0*/
        TIMER3_DISABLE();
        /*16bit mode*/
        T3CONbits.RD16 = 1;
        /*timer mode or counter mode*/
        if(TIMER3_COUNTER == timer3->timer3_mode){
            TIMER3_COUNTER_MODE();
            if(TIMER3_NOT_SYNC == timer3->timer3_Counter_Sync){
                TIMER3_NOT_SYNC_COUNTER();
            }
            else if(TIMER3_SYNC == timer3->timer3_Counter_Sync){
                TIMER3_SYNC_COUNTER();
            }
        }
        else if(TIMER3_TIMER == timer3->timer3_mode){
            TIMER3_TIMER_MODE();
        }
        /*prescaler value*/
        T3CONbits.T3CKPS = timer3->prescale_value;
        /*preload value */
        TMR3H = (timer3->preload_value >> 8); 
        TMR3L = (uint8) timer3->preload_value;
        preload3_value = timer3->preload_value;
        /*interrupt configuration*/
#ifdef TIMER3_INTERRUPT 
        TIMER3_INTERRUPT_FlagClear();
        TIMER3_INTERRUPT_ENABLE();
        TIMER3_INTHANDLER = timer3->EXT_INTERRUPT;
#ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE 
        INTERRUPT_PriorityLevelEnable();
        if (PRIORITY_HIGH == timer3->priority){
            INTERRUPT_GlobalHighEnable();
            TIMER3_INTERRUPT_PriortyHigh();
        }
        else if(PRIORITY_LOW == timer3->priority){
            INTERRUPT_GlobalLowEnable();
            TIMER3_INTERRUPT_PriortyLow();
        } 
#else
        INTERRUPT_GlobalEnabl();
        INTERRUPT_PeripheralEnable();
#endif
#endif
        /*enable timer0*/
        TIMER3_ENABLE();
    }
    return ret;   
}

Std_ReturnType timer3_Deinit(const timer3_t *timer3){
    Std_ReturnType ret = E_OK;
    if (NULL == timer3){
        ret = E_NOT_OK;
    }
    else {
        /*disable timer0*/
        TIMER3_DISABLE();
        /*disable interrupt future*/
#ifdef TIMER3_INTERRUPT
        TIMER3_INTERRUPT_FlagClear();
        TIMER3_INTERRUPT_DISABLE();
#endif
    }
    return ret;  
}

Std_ReturnType timer3_Read (const timer3_t *timer3, uint16 *value){
    Std_ReturnType ret = E_OK;
    if (NULL == timer3){
        ret = E_NOT_OK;
    }
    else {
         *value =(uint16)(TMR3L + (TMR3H << 8));
    }
    return ret; 
}

Std_ReturnType timer3_write (const timer3_t *timer3, uint16 value){
    Std_ReturnType ret = E_OK;
    if (NULL == timer3){
        ret = E_NOT_OK;
    }
    else {
         TMR3H = (value >> 8); 
         TMR3L = (uint8)value;
    }
    return ret; 
}

void TIMER3_ISR (void ){
    #ifdef TIMER3_INTERRUPT
    TIMER3_INTERRUPT_FlagClear();
   /*code*/
    TMR3H = (preload3_value >> 8); 
    TMR3L = (uint8)preload3_value;
    if(TIMER3_INTHANDLER) {
        TIMER3_INTHANDLER();
    }
    #endif
}
