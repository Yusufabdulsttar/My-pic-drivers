/* 
 * File:   mcal_timer1.c
 * Author: yusuf
 *
 * Created on May 4, 2023, 3:16 PM
 */
#include "mcal_timer1.h"

#ifdef TIMER1_INTERRUPT 
static void(* TIMER1_INTHANDLER)(void) = NULL;
#endif
static uint16 preload1_value = 0;

Std_ReturnType timer1_init(const timer1_t *timer1){
    Std_ReturnType ret = E_OK;
    if (NULL == timer1){
        ret = E_NOT_OK;
    }
    else {
        /*disable timer0*/
        TIMER1_DISABLE();
        /*16bit mode*/
        T1CONbits.RD16 = 1;
        /*timer mode or counter mode*/
        if(TIMER1_COUNTER == timer1->timer1_mode){
            TIMER1_COUNTER_MODE();
            if(TIMER1_NOT_SYNC == timer1->timer1_Counter_Sync){
                TIMER1_NOT_SYNC_COUNTER();
            }
            else if(TIMER1_SYNC == timer1->timer1_Counter_Sync){
                TIMER1_SYNC_COUNTER();
            }
        }
        else if(TIMER1_TIMER == timer1->timer1_mode){
            TIMER1_TIMER_MODE();
        }
        /*Timer oscillator cfg*/
        if(TIMER1_OSC_ENABLE == timer1->timer1_osc){
            TIMER1_SOC__ENABLE();
        }
        else if(TIMER1_OSC_DISABLE == timer1->timer1_osc){
            TIMER1_SOC__DISABLE();
        }
        /*prescaler value*/
        T1CONbits.T1CKPS = timer1->prescale_value;
        /*preload value */
        TMR1H = (uint8)(timer1->preload_value >> 8); 
        TMR1L = (uint8) timer1->preload_value;
        preload1_value = timer1->preload_value;
        /*interrupt configuration*/
#ifdef TIMER1_INTERRUPT 
        TIMER1_INTERRUPT_FlagClear();
        TIMER1_INTERRUPT_ENABLE();
        TIMER1_INTHANDLER = timer1->EXT_INTERRUPT;
#ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE 
        INTERRUPT_PriorityLevelEnable();
        if (PRIORITY_HIGH == timer1->priority){
            INTERRUPT_GlobalHighEnable();
            TIMER1_INTERRUPT_PriortyHigh();
        }
        else if(PRIORITY_LOW == timer1->priority){
            INTERRUPT_GlobalLowEnable();
            TIMER1_INTERRUPT_PriortyLow();
        } 
#else
        INTERRUPT_GlobalEnabl();
        INTERRUPT_PeripheralEnable();
#endif
#endif
        /*enable timer0*/
        TIMER1_ENABLE();
    }
    return ret;   
}


Std_ReturnType timer1_Deinit(const timer1_t *timer1){
    Std_ReturnType ret = E_OK;
    if (NULL == timer1){
        ret = E_NOT_OK;
    }
    else {
        /*disable timer0*/
        TIMER1_DISABLE();
        /*disable interrupt future*/
#ifdef TIMER1_INTERRUPT
        TIMER1_INTERRUPT_FlagClear();
        TIMER1_INTERRUPT_DISABLE();
#endif
    }
    return ret;  
}

Std_ReturnType timer1_Read (const timer1_t *timer1, uint16 *value){
    Std_ReturnType ret = E_OK;
    if (NULL == timer1){
        ret = E_NOT_OK;
    }
    else {
         *value =(uint16)(TMR1L + (TMR1H << 8));
    }
    return ret; 
}

Std_ReturnType timer1_write (const timer1_t *timer1, uint16 value){
    Std_ReturnType ret = E_OK;
    if (NULL == timer1){
        ret = E_NOT_OK;
    }
    else {
         TMR1H = (value >> 8); 
         TMR1L = (uint8)value;
    }
    return ret; 
}

void TIMER1_ISR (void ){
    #ifdef TIMER1_INTERRUPT
    TIMER1_INTERRUPT_FlagClear();
   /*code*/
    TMR1H = (preload1_value >> 8); 
    TMR1L = (uint8)preload1_value;
    
    if(TIMER1_INTHANDLER) {
        TIMER1_INTHANDLER();
    }
    #endif
}
