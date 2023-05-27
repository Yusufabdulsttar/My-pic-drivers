/* 
 * File:   mcal_external_interrupt.h
 * Author: yusuf
 *
 * Created on April 27, 2023, 5:37 PM
 */

#ifndef MCAL_EXTERNAL_INTERRUPT_H
#define	MCAL_EXTERNAL_INTERRUPT_H
/* section : Includes   */
#include "mcal_interrupt_cfg.h"
/* section : Micro Declaration  */
/* section : Micro Function Declaration  */
#ifdef EXTERNAL_INTERRUPT_INTX_ENABLE 
#define EXT_INT0_INTERRUPT_ENABLE()         (INTCONbits.INT0E = 1)
#define EXT_INT0_INTERRUPT_DISABLE()        (INTCONbits.INT0E = 0)
#define EXT_INT0_INTERRUPT_FlagClear()      (INTCONbits.INT0IF = 0)
#define EXT_INT0_INTERRUPT_RiseEdge()       (INTCON2bits.INTEDG0 = 1)
#define EXT_INT0_INTERRUPT_FallEdge()       (INTCON2bits.INTEDG0 = 0)

#define EXT_INT1_INTERRUPT_ENABLE()         (INTCON3bits.INT1IE = 1)
#define EXT_INT1_INTERRUPT_DISABLE()        (INTCON3bits.INT1IE = 0)
#define EXT_INT1_INTERRUPT_FlagClear()      (INTCON3bits.INT1IF = 0)
#define EXT_INT1_INTERRUPT_RiseEdge()       (INTCON2bits.INTEDG1 = 1)
#define EXT_INT1_INTERRUPT_FallEdge()       (INTCON2bits.INTEDG1 = 0)

#define EXT_INT2_INTERRUPT_ENABLE()         (INTCON3bits.INT2IE = 1)
#define EXT_INT2_INTERRUPT_DISABLE()        (INTCON3bits.INT2IE = 0)
#define EXT_INT2_INTERRUPT_FlagClear()      (INTCON3bits.INT2IF = 0)
#define EXT_INT2_INTERRUPT_RiseEdge()       (INTCON2bits.INTEDG2 = 1)
#define EXT_INT2_INTERRUPT_FallEdge()       (INTCON2bits.INTEDG2 = 0)


#ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE 
#define EXT_INT1_INTERRUPT_PriortyHigh()    (INTCON3bits.INT1IP = 1)
#define EXT_INT1_INTERRUPT_PriortyLow()     (INTCON3bits.INT1IP = 0)

#define EXT_INT2_INTERRUPT_PriortyHigh()    (INTCON3bits.INT2IP = 1)
#define EXT_INT2_INTERRUPT_PriortyLow()     (INTCON3bits.INT2IP = 0)
#endif
#endif


#ifdef EXTERNAL_INTERRUPT_PORTB_ENABLE 

#define EXT_PORTB_INTERRUPT_ENABLE()         (INTCONbits.RBIE = 1)
#define EXT_PORTB_INTERRUPT_DISABLE()        (INTCONbits.RBIE = 0)
#define EXT_PORTB_INTERRUPT_FlagClear()      (INTCONbits.RBIF = 0)

#ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE 
#define EXT_PORTB_INTERRUPT_PriortyHigh()    (INTCON2bits.RBIP = 1)
#define EXT_PORTB_INTERRUPT_PriortyLow()     (INTCON2bits.RBIP = 0)

#endif
#endif
/* section : Data Type Declaration  */
typedef enum {
    INTERRUPT_RiseEdge = 0,
    INTERRUPT_fallEdge        
}Interrupt_INT_edge_t;

typedef enum {
    Interrupt_INT0 = 0,
    Interrupt_INT1,
    Interrupt_INT2        
}Interrupt_INTx_t;

typedef struct {
    void (* EXT_INTERRUPT)(void);
    Interrupt_INT_edge_t edge;
    Interrupt_INTx_t source;
    Interrupt_priority_t priority;
    pin_config_t pin_int;
}Interrupt_INT_t;

typedef struct {
    void (* EXT_INTERRUPT_HIGH)(void);
    void (* EXT_INTERRUPT_LOW)(void);
    pin_config_t pin_int;
    Interrupt_priority_t priority;
}Interrupt_PORTB_t;
/* section : Function Declaration  */

Std_ReturnType Interrupt_INT_Init (const Interrupt_INT_t *obj);
Std_ReturnType Interrupt_INT_DeInit (const Interrupt_INT_t *obj);
Std_ReturnType Interrupt_PORTB_Init (const Interrupt_PORTB_t *obj);
Std_ReturnType Interrupt_PORTB_DeInit (const Interrupt_PORTB_t *obj);

#endif	/* MCAL_EXTERNAL_INTERRUPT_H */

