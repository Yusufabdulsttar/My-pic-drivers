/* 
 * File:   mcal_timer1.h
 * Author: yusuf
 *
 * Created on May 4, 2023, 3:16 PM
 */

#ifndef MCAL_TIMER1_H
#define	MCAL_TIMER1_H

/* section : Includes   */
#include "../../GPIO/hal_gpio.h"
#include "../../Interrupt/mcal_internal_interrupt.h"

/* section : Micro Declaration  */

#define TIMER1_COUNTER  1
#define TIMER1_TIMER    0

#define TIMER1_OSC_ENABLE     1
#define TIMER1_OSC_DISABLE    0

#define TIMER1_NOT_SYNC     1
#define TIMER1_SYNC         0
/* section : Micro Function Declaration  */
#define TIMER1_ENABLE()               (T1CONbits.TMR1ON = 1)
#define TIMER1_DISABLE()              (T1CONbits.TMR1ON = 0)

#define TIMER1_SYSTEM_CLOCK_STATUS()  (T1CONbits.T1RUN)

#define TIMER1_SOC__ENABLE()          (T1CONbits.T1OSCEN = 1)
#define TIMER1_SOC__DISABLE()         (T1CONbits.T1OSCEN = 0)

#define TIMER1_NOT_SYNC_COUNTER()     (T1CONbits.T1SYNC = 1)
#define TIMER1_SYNC_COUNTER()         (T1CONbits.T1SYNC = 0)

#define TIMER1_TIMER_MODE()           (T1CONbits.TMR1CS = 0)
#define TIMER1_COUNTER_MODE()         (T1CONbits.TMR1CS = 1)



/* section : Data Type Declaration  */
 
typedef enum {
    PRESCALER_DISABLE_1,
    PRESCALER1_2,
    PRESCALER1_4,
    PRESCALER1_8,                 
}timer1_prescaler_select_t;

typedef struct{
#ifdef TIMER1_INTERRUPT
    void (* EXT_INTERRUPT)(void);
#endif    
    timer1_prescaler_select_t prescale_value;
    uint16 preload_value;
    Interrupt_priority_t priority;
    uint8 timer1_mode : 1;
    uint8 timer1_osc : 1;
    uint8 timer1_Counter_Sync : 1;   
}timer1_t;

/* section : Function Declaration  */

Std_ReturnType timer1_init(const timer1_t *timer1);
Std_ReturnType timer1_Deinit(const timer1_t *timer1);
Std_ReturnType timer1_Read (const timer1_t *timer1, uint16 *value);
Std_ReturnType timer1_write (const timer1_t *timer1, uint16 value);


#endif	/* MCAL_TIMER1_H */

