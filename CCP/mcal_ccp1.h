/* 
 * File:   mcal_ccp1.h
 * Author: yusuf
 *
 * Created on May 7, 2023, 8:20 PM
 */

#ifndef MCAL_CCP1_H
#define	MCAL_CCP1_H
/* section : Includes   */
#include "ccp_cfg.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"

/* section : Micro Declaration  */

#define CCP_DISABLE  0
#define CCP_CAPTURE_MODE_1ST_FALLING_EDGE  4
#define CCP_CAPTURE_MODE_1ST_RISING_EDGE   5
#define CCP_CAPTURE_MODE_4TH_RISING_EDGE   6
#define CCP_CAPTURE_MODE_16TH_RISING_EDGE  7
#define CCP_COMPARE_MODE_TOGGLE_ON_MATCH   2
#define CCP_COMPARE_MODE_SET_HIGH_ON_MATCH  8
#define CCP_COMPARE_MODE_SET_LOW_ON_MATCH   9
#define CCP_COMPARE_MODE_GEN_SOFW_INTERRUPT  10
#define CCP_COMPARE_MODE_GEN_EVENT         11
#define CCP_PMW_MODE                       12

#define CCP_CAPTURE_READY      1
#define CCP_CAPTURE_NOT_READY  0

#define CCP_COMPARE_READY      1
#define CCP_COMPARE_NOT_READY  0

/* section : Micro Function Declaration  */

#define CCP1_SET_MODE(X)   (CCP1CONbits.CCP1M = (X));
#define CCP2_SET_MODE(X)   (CCP2CONbits.CCP2M = (X));
/* section : Data Type Declaration  */
typedef enum{
    CCP_CAPTURE_MODE_SELECTED = 0, /* Select CCP1 Capture Mode */
    CCP_COMPARE_MODE_SELECTED,     /* Select CCP1 Compare Mode */
    CCP_PWM_MODE_SELECTED          /* Select CCP1 PWM Mode */
}ccp1_mode_t;

typedef enum{
    CCP1_SELECT = 0, /* Select CCP1 */
    CCP2_SELECT,     /* Select CCP2 */
}ccp_select_t;

typedef enum{
    CCP1_CCP2_TIMER1 = 0,      /* Timer1 is the capture/compare for CCP*/
    CCP1_TIMER1_CCP2_TIMER3,   /* Timer3 is the capture/compare for CCP2 nd Timer1 for CCP1 */
    CCP1_CCP2_TIMER3,          /* Timer3 is the capture/compare FOR CCP */
}ccp_timer_capture_t;

typedef union{
    struct{
        uint8 ccpr_low;  /* CCPR1 low register */
        uint8 ccpr_high; /* CCPR1 high register */
    };
    struct{
        uint16 ccpr_16Bit; /* Read CCPR1 as a 16 Bit value */
    };
}CCP_REG_T;

typedef struct {
    uint32 pmw_freq;
    ccp_select_t ccp_select;
    ccp_timer_capture_t ccp_timer_capture;
    ccp1_mode_t ccp_mode;
    uint8 ccp_mode_variant;
    pin_config_t pin;
    Interrupt_priority_t priority;
    uint8 timer2_postscaler_value : 4;
    uint8 timer2_prescaler_value : 2;
#ifdef CCP1_INTERRUPT
    void (* CCP1_INTERRUPT_HANDLER)(void);
#endif    
#ifdef CCP2_INTERRUPT
    void (* CCP2_INTERRUPT_HANDLER)(void);
#endif    
}ccp_t;

/* section : Function Declaration  */
Std_ReturnType ccp_int (const ccp_t *ccp);
Std_ReturnType ccp_Deint (const ccp_t *ccp);

Std_ReturnType ccp_capture_ready (uint8 *capture_state,const ccp_t *ccp);
Std_ReturnType ccp_capture_read (uint16 *value,const ccp_t *ccp);

Std_ReturnType ccp_compare_ready (uint8 *compare_state,const ccp_t *ccp);
Std_ReturnType ccp_compare_write_value (uint16 value,const ccp_t *ccp);

Std_ReturnType ccp_pwm_duty_cycle (const uint8 duty,const ccp_t *ccp);
Std_ReturnType ccp_start_pwm (const ccp_t *ccp);
Std_ReturnType ccp_stop_pwm (const ccp_t *ccp);
#endif	/* MCAL_CCP1_H */

