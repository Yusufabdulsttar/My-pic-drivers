/* 
 * File:   mcal_timer3.h
 * Author: yusuf
 *
 * Created on May 4, 2023, 7:26 PM
 */

#ifndef MCAL_TIMER1_H
#define	MCAL_TIMER1_H

/* section : Includes   */
#include "../../GPIO/hal_gpio.h"
#include "../../Interrupt/mcal_internal_interrupt.h"

/* section : Micro Declaration  */

#define TIMER3_COUNTER  1
#define TIMER3_TIMER    0

#define TIMER3_NOT_SYNC     1
#define TIMER3_SYNC         0

/* section : Micro Function Declaration  */
#define TIMER3_ENABLE()               (T3CONbits.TMR3ON = 1)
#define TIMER3_DISABLE()              (T3CONbits.TMR3ON = 0)

#define TIMER3_NOT_SYNC_COUNTER()     (T3CONbits.T3SYNC = 1)
#define TIMER3_SYNC_COUNTER()         (T3CONbits.T3SYNC = 0)

#define TIMER3_TIMER_MODE()           (T3CONbits.TMR3CS = 0)
#define TIMER3_COUNTER_MODE()         (T3CONbits.TMR3CS = 1)

/* section : Data Type Declaration  */
 
typedef enum {
    PRESCALER_TIMER3_DISABLE_1,
    PRESCALER_TIMER3_2,
    PRESCALER_TIMER3_4,
    PRESCALER_TIMER3_8,                 
}timer3_prescaler_select_t;

typedef struct{
#ifdef TIMER3_INTERRUPT
    void (* EXT_INTERRUPT)(void);
#endif    
    timer3_prescaler_select_t prescale_value;
    uint16 preload_value;
    Interrupt_priority_t priority;
    uint8 timer3_mode : 1;
    uint8 timer3_Counter_Sync : 1;   
}timer3_t;

/* section : Function Declaration  */

Std_ReturnType timer3_init(const timer3_t *timer3);
Std_ReturnType timer3_Deinit(const timer3_t *timer3);
Std_ReturnType timer3_Read (const timer3_t *timer3, uint16 *value);
Std_ReturnType timer3_write (const timer3_t *timer3, uint16 value);


#endif	/* MCAL_TIMER1_H */

