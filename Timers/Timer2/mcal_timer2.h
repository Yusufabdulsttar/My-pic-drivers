/* 
 * File:   mcal_timer2.h
 * Author: yusuf
 *
 * Created on May 4, 2023, 5:57 PM
 */

#ifndef MCAL_TIMER2_H
#define	MCAL_TIMER2_H

/* section : Includes   */
#include "../../GPIO/hal_gpio.h"
#include "../../Interrupt/mcal_internal_interrupt.h"

/* section : Micro Declaration  */

/* section : Micro Function Declaration  */
#define TIMER2_ENABLE()               (T2CONbits.TMR2ON = 1)
#define TIMER2_DISABLE()              (T2CONbits.TMR2ON = 0)

/* section : Data Type Declaration  */

typedef enum {
    PRESCALER_TIMER2_DISABLE_1,
    PRESCALER_TIMER2_4,
    PRESCALER_TIMER2_16,                 
}timer2_prescaler_select_t;

typedef enum {
    POSTSCALER_TIMER2_DISABLE_1,
    POSTSCALER_TIMER2_2,
    POSTSCALER_TIMER2_3,
    POSTSCALER_TIMER2_4, 
    POSTSCALER_TIMER2_5,  
    POSTSCALER_TIMER2_6,
    POSTSCALER_TIMER2_7,
    POSTSCALER_TIMER2_8,  
    POSTSCALER_TIMER2_9,
    POSTSCALER_TIMER2_10,
    POSTSCALER_TIMER2_11,  
    POSTSCALER_TIMER2_12,  
    POSTSCALER_TIMER2_13,
    POSTSCALER_TIMER2_14,
    POSTSCALER_TIMER2_15,
    POSTSCALER_TIMER2_16,      
}timer2_postscaler_select_t;

typedef struct{
#ifdef TIMER2_INTERRUPT
    void (* EXT_INTERRUPT)(void);
#endif    
    timer2_prescaler_select_t prescale_value;
    timer2_postscaler_select_t post_scale_value;
    uint8 preload_value;
    Interrupt_priority_t priority;
}timer2_t;

/* section : Function Declaration  */
Std_ReturnType timer2_init(const timer2_t *timer2);
Std_ReturnType timer2_Deinit(const timer2_t *timer2);
Std_ReturnType timer2_Read (const timer2_t *timer2, uint8 *value);
Std_ReturnType timer2_write (const timer2_t *timer2, uint8 value);

#endif	/* MCAL_TIMER2_H */

