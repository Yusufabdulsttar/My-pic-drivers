/* 
 * File:   mcal_ccp1.c
 * Author: yusuf
 *
 * Created on May 7, 2023, 8:20 PM
 */

#include "mcal_ccp1.h"

#ifdef CCP1_INTERRUPT 
static void(* CCP1_INTHANDLER)(void) = NULL;
static void(* CCP2_INTHANDLER)(void) = NULL;
#endif
static Std_ReturnType cfg_capture(const ccp_t *ccp);
static Std_ReturnType cfg_compare(const ccp_t *ccp);
static Std_ReturnType cfg_interrupt(const ccp_t *ccp);
static Std_ReturnType timer_cfg (const ccp_t *ccp);

Std_ReturnType ccp_int (const ccp_t *ccp){
    Std_ReturnType ret = E_OK;
    if (NULL == ccp){
        ret = E_NOT_OK;
    }
    else {
    /*cpp disable*/
    if(CCP1_SELECT == ccp->ccp_select){    
    CCP1_SET_MODE(CCP_DISABLE);
    }
    else if(CCP2_SELECT == ccp->ccp_select){
    CCP2_SET_MODE(CCP_DISABLE);
    }
    /*ccp mode select*/
    if(CCP_CAPTURE_MODE_SELECTED == ccp->ccp_mode){
        ret = cfg_capture(ccp);
    }    
    else if(CCP_COMPARE_MODE_SELECTED == ccp->ccp_mode){
        ret = cfg_compare(ccp);
    }
    else if(CCP_PWM_MODE_SELECTED == ccp->ccp_mode){
        /*set the pwm period by init PR2*/ 
        PR2 = (uint8)((_XTAL_FREQ / (ccp->pmw_freq * 4.0 * ccp->timer2_postscaler_value * ccp->timer2_prescaler_value))-1);
    }
    /*ccp interrupt*/
    ret = cfg_interrupt(ccp);
    /*timer init*/
    timer_cfg (ccp);
    /*pin init*/
    gpio_pin_config_intialize(&(ccp->pin));
    }
    return ret; 
}


Std_ReturnType ccp_Deint (const ccp_t *ccp){
    Std_ReturnType ret = E_OK;
    if (NULL == ccp){
        ret = E_NOT_OK;
    }
    else {
    /*cpp disable*/
        if(CCP1_SELECT == ccp->ccp_select){
            CCP1_SET_MODE(CCP_DISABLE);
#ifdef CCP1_INTERRUPT 
            CCP1_INTERRUPT_DISABLE();
#endif  
        }
        else if(CCP2_SELECT == ccp->ccp_select){
            CCP2_SET_MODE(CCP_DISABLE);
#ifdef CCP2_INTERRUPT 
            CCP2_INTERRUPT_DISABLE();
#endif    
        }
    }
    return ret; 
}

Std_ReturnType ccp_capture_ready (uint8 *capture_state,const ccp_t *ccp){
    Std_ReturnType ret = E_OK;
    if (NULL == capture_state){
        ret = E_NOT_OK;
    }
    else {
        /*capture mode for ccp1*/
        if(CCP1_SELECT == ccp->ccp_select){
        if(CCP_CAPTURE_READY == PIR1bits.CCP1IF){
            *capture_state = CCP_CAPTURE_READY;
        }
        else if(CCP_CAPTURE_NOT_READY == PIR1bits.CCP1IF){
            *capture_state = CCP_CAPTURE_NOT_READY;
        }
        }
        /*capture mode for ccp2*/
        if(CCP2_SELECT == ccp->ccp_select){
        if(CCP_CAPTURE_READY == PIR2bits.CCP2IF){
            *capture_state = CCP_CAPTURE_READY;
        }
        else if(CCP_CAPTURE_NOT_READY == PIR2bits.CCP2IF){
            *capture_state = CCP_CAPTURE_NOT_READY;
        }
        }
    }
    return ret; 
}
Std_ReturnType ccp_capture_read (uint16 *value,const ccp_t *ccp){
    Std_ReturnType ret = E_OK;
    if (NULL == ccp){
        ret = E_NOT_OK;
    }
    else {
        if(CCP1_SELECT == ccp->ccp_select){
        CCP_REG_T temp = {.ccpr_high =0,.ccpr_high = 0};
        temp.ccpr_low = CCPR1L;
        temp.ccpr_high = CCPR1H;
        *value = temp.ccpr_16Bit;
        }
        else if(CCP2_SELECT == ccp->ccp_select){
        CCP_REG_T temp = {.ccpr_high =0,.ccpr_high = 0};
        temp.ccpr_low = CCPR2L;
        temp.ccpr_high = CCPR2H;
        *value = temp.ccpr_16Bit;
        }
        }
    return ret; 
}

Std_ReturnType ccp_compare_ready (uint8 *compare_state,const ccp_t *ccp){
    Std_ReturnType ret = E_OK;
    if (NULL == ccp){
        ret = E_NOT_OK;
    }
    else {
        if(CCP1_SELECT == ccp->ccp_select){
        if(CCP_COMPARE_READY == PIR1bits.CCP1IF){
            *compare_state = CCP_COMPARE_READY;
        }
        else if(CCP_COMPARE_NOT_READY == PIR1bits.CCP1IF){
            *compare_state = CCP_COMPARE_NOT_READY;
        }
        }
        else if(CCP2_SELECT == ccp->ccp_select){
        if(CCP_COMPARE_READY == PIR2bits.CCP2IF){
            *compare_state = CCP_COMPARE_READY;
        }
        else if(CCP_COMPARE_NOT_READY == PIR2bits.CCP2IF){
            *compare_state = CCP_COMPARE_NOT_READY;
        }
        }
    }
    return ret; 
}
Std_ReturnType ccp_compare_write_value (uint16 value,const ccp_t *ccp){
    Std_ReturnType ret = E_OK;
    if (NULL == ccp){
        ret = E_NOT_OK;
    }
    else{
    if(CCP1_SELECT == ccp->ccp_select){
    CCP_REG_T temp = {.ccpr_high =0,.ccpr_high = 0};
    temp.ccpr_16Bit = value;
    CCPR1L = temp.ccpr_low;
    CCPR1H = temp.ccpr_high;
    }
    else if(CCP2_SELECT == ccp->ccp_select){
    CCP_REG_T temp = {.ccpr_high =0,.ccpr_high = 0};
    temp.ccpr_16Bit = value;
    CCPR2L = temp.ccpr_low;
    CCPR2H = temp.ccpr_high;
    }
    }
    return ret; 
}

Std_ReturnType ccp_pwm_duty_cycle (const uint8 duty,const ccp_t *ccp){
    Std_ReturnType ret = E_OK;
    if (NULL == ccp){
        ret = E_NOT_OK;
    }
    else {
    uint16 reg_temp = 0;
    reg_temp =(uint16)( 4 * (PR2 +1) * (duty/100.0));
    if(CCP1_SELECT == ccp->ccp_select){
    CCPR1L = (uint8)(reg_temp >> 2);
    CCP1CONbits.DC1B = (uint8) (reg_temp & 0x03);
    }
    else if(CCP2_SELECT == ccp->ccp_select){
    CCPR2L = (uint8)(reg_temp >> 2);
    CCP2CONbits.DC2B = (uint8) (reg_temp & 0x03);  
    }
    }
    return ret; 
}


Std_ReturnType ccp_start_pwm (const ccp_t *ccp){
    Std_ReturnType ret = E_OK;
    if (NULL == ccp){
        ret = E_NOT_OK;
    }
    else {
    Std_ReturnType ret = E_OK;
    if(CCP1_SELECT == ccp->ccp_select){
        CCP1_SET_MODE(CCP_PMW_MODE);
    }
    else if(CCP2_SELECT == ccp->ccp_select){
        CCP2_SET_MODE(CCP_PMW_MODE);
    }
    }
    return ret;
}

Std_ReturnType ccp_stop_pwm (const ccp_t *ccp){
    Std_ReturnType ret = E_OK;
    if (NULL == ccp){
        ret = E_NOT_OK;
    }
    else {
    Std_ReturnType ret = E_OK;
    if(CCP1_SELECT == ccp->ccp_select){
        CCP1_SET_MODE(CCP_DISABLE);
    }
    else if(CCP2_SELECT == ccp->ccp_select){
        CCP2_SET_MODE(CCP_DISABLE);
    }
    }
    return ret;
}

static Std_ReturnType cfg_capture(const ccp_t *ccp){
    Std_ReturnType ret = E_OK;
    if (NULL == ccp){
        ret = E_NOT_OK;
    }
    else {
    if(CCP1_SELECT == ccp->ccp_select){
        switch(ccp->ccp_mode_variant){
            case CCP_CAPTURE_MODE_1ST_FALLING_EDGE : {
                CCP1_SET_MODE(CCP_CAPTURE_MODE_1ST_FALLING_EDGE);
                break;
            }
            case CCP_CAPTURE_MODE_1ST_RISING_EDGE : {
                CCP1_SET_MODE(CCP_CAPTURE_MODE_1ST_RISING_EDGE);
                break;
            }
            case CCP_CAPTURE_MODE_4TH_RISING_EDGE : {
                CCP1_SET_MODE(CCP_CAPTURE_MODE_4TH_RISING_EDGE);
                break;
            }
            case CCP_CAPTURE_MODE_16TH_RISING_EDGE : {
                CCP1_SET_MODE(CCP_CAPTURE_MODE_16TH_RISING_EDGE);
                break;
            }
          }
        }
    else if(CCP2_SELECT == ccp->ccp_select){
        switch(ccp->ccp_mode_variant){
            case CCP_CAPTURE_MODE_1ST_FALLING_EDGE : {
                CCP2_SET_MODE(CCP_CAPTURE_MODE_1ST_FALLING_EDGE);
                break;
            }
            case CCP_CAPTURE_MODE_1ST_RISING_EDGE : {
                CCP2_SET_MODE(CCP_CAPTURE_MODE_1ST_RISING_EDGE);
                break;
            }
            case CCP_CAPTURE_MODE_4TH_RISING_EDGE : {
                CCP2_SET_MODE(CCP_CAPTURE_MODE_4TH_RISING_EDGE);
                break;
            }
            case CCP_CAPTURE_MODE_16TH_RISING_EDGE : {
                CCP2_SET_MODE(CCP_CAPTURE_MODE_16TH_RISING_EDGE);
                break;
            }
          }
        }
    }   
    return ret;
}

static Std_ReturnType timer_cfg (const ccp_t *ccp){
    Std_ReturnType ret = E_OK;
    if (NULL == ccp){
        ret = E_NOT_OK;
    }
    else {
    if(CCP1_CCP2_TIMER1 == ccp->ccp_timer_capture){
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 0;
    }
    else if(CCP1_TIMER1_CCP2_TIMER3 == ccp->ccp_timer_capture){
        T3CONbits.T3CCP1 = 1;
        T3CONbits.T3CCP2 = 0; 
    }
    else if(CCP1_CCP2_TIMER3 == ccp->ccp_timer_capture){
        T3CONbits.T3CCP1 = 1;
        T3CONbits.T3CCP2 = 1; 
    }
    }
    return ret;
}

static Std_ReturnType cfg_compare(const ccp_t *ccp){
    Std_ReturnType ret = E_OK;
    if (NULL == ccp){
        ret = E_NOT_OK;
    }
    else {
        if(CCP1_SELECT == ccp->ccp_select){
        switch(ccp->ccp_mode_variant){
            case CCP_COMPARE_MODE_TOGGLE_ON_MATCH : {
                CCP1_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH);
                break;
            }
            case CCP_COMPARE_MODE_SET_LOW_ON_MATCH : {
                CCP1_SET_MODE(CCP_COMPARE_MODE_SET_LOW_ON_MATCH);
                break;
            }
            case CCP_COMPARE_MODE_SET_HIGH_ON_MATCH : {
                CCP1_SET_MODE(CCP_COMPARE_MODE_SET_HIGH_ON_MATCH);
                break;
            }
            case CCP_COMPARE_MODE_GEN_SOFW_INTERRUPT : {
                CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_SOFW_INTERRUPT);
                break;
            }
            case CCP_COMPARE_MODE_GEN_EVENT : {
                CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_EVENT);
                break;
            }
        }
    }
    else if(CCP2_SELECT == ccp->ccp_select){
        switch(ccp->ccp_mode_variant){
            case CCP_COMPARE_MODE_TOGGLE_ON_MATCH : {
                CCP2_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH);
                break;
            }
            case CCP_COMPARE_MODE_SET_LOW_ON_MATCH : {
                CCP2_SET_MODE(CCP_COMPARE_MODE_SET_LOW_ON_MATCH);
                break;
            }
            case CCP_COMPARE_MODE_SET_HIGH_ON_MATCH : {
                CCP2_SET_MODE(CCP_COMPARE_MODE_SET_HIGH_ON_MATCH);
                break;
            }
            case CCP_COMPARE_MODE_GEN_SOFW_INTERRUPT : {
                CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_SOFW_INTERRUPT);
                break;
            }
            case CCP_COMPARE_MODE_GEN_EVENT : {
                CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_EVENT);
                break;
            }
        }
    }
    }
    return ret;
}


static Std_ReturnType cfg_interrupt(const ccp_t *ccp){
    Std_ReturnType ret = E_OK;
    if (NULL == ccp){
        ret = E_NOT_OK;
    }
    else {
        if(CCP1_SELECT == ccp->ccp_select){
#ifdef CCP1_INTERRUPT 
        CCP1_INTERRUPT_FlagClear();
        CCP1_INTERRUPT_ENABLE();
        CCP1_INTHANDLER = ccp->CCP1_INTERRUPT_HANDLER;
#ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE 
        INTERRUPT_PriorityLevelEnable();
        if (PRIORITY_HIGH == ccp->priority{
            INTERRUPT_GlobalHighEnable();
            CCP1_INTERRUPT_PriortyHigh();
        }
        else if(PRIORITY_LOW == ccp->priority){
            INTERRUPT_GlobalLowEnable();
            CCP1_INTERRUPT_PriortyLow();
        } 
#else
        INTERRUPT_GlobalEnabl();
        INTERRUPT_PeripheralEnable();
#endif
#endif
    }
    else if(CCP2_SELECT == ccp->ccp_select){
    /*ccp2 interrupt*/
#ifdef CCP2_INTERRUPT 
        CCP2_INTERRUPT_FlagClear();
        CCP2_INTERRUPT_ENABLE();
        CCP2_INTHANDLER = ccp->CCP2_INTERRUPT_HANDLER;
#ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE 
        INTERRUPT_PriorityLevelEnable();
        if (PRIORITY_HIGH == ccp->priority){
            INTERRUPT_GlobalHighEnable();
            CCP2_INTERRUPT_PriortyHigh();
        }
        else if(PRIORITY_LOW == ccp->priority){
            INTERRUPT_GlobalLowEnable();
            CCP2_INTERRUPT_PriortyLow();
        } 
#else
        INTERRUPT_GlobalEnabl();
        INTERRUPT_PeripheralEnable();
#endif
#endif
    }
    }
    return ret;
}

void CCP1_ISR (void){
    #ifdef CCP1_INTERRUPT
    CCP1_INTERRUPT_FlagClear();
   /*code*/
    
    if(CCP1_INTHANDLER) {
        CCP1_INTHANDLER();
    }
    #endif
}

void CCP2_ISR (void){
    #ifdef CCP2_INTERRUPT
    CCP2_INTERRUPT_FlagClear();
   /*code*/
    
    if(CCP2_INTHANDLER) {
        CCP2_INTHANDLER();
    }
    #endif
}