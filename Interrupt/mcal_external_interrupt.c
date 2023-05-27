/* 
 * File:   mcal_external_interrupt.c
 * Author: yusuf
 *
 * Created on April 27, 2023, 5:37 PM
 */

#include "mcal_external_interrupt.h"


static void (* INT0_INTHANDLER) (void) = NULL;
static void (* INT1_INTHANDLER) (void) = NULL;
static void (* INT2_INTHANDLER) (void) = NULL;

static void (* RB4_INTHANDLER_HIGH) (void) = NULL;
static void (* RB4_INTHANDLER_LOW) (void) = NULL;
static void (* RB5_INTHANDLER_HIGH) (void) = NULL;
static void (* RB5_INTHANDLER_LOW) (void) = NULL;
static void (* RB6_INTHANDLER_HIGH) (void) = NULL;
static void (* RB6_INTHANDLER_LOW) (void) = NULL;
static void (* RB7_INTHANDLER_HIGH) (void) = NULL;
static void (* RB7_INTHANDLER_LOW) (void) = NULL;

static Std_ReturnType Interrupt_INT_Ena (const Interrupt_INT_t *obj);
static Std_ReturnType Interrupt_INT_disa (const Interrupt_INT_t *obj);
static Std_ReturnType Interrupt_INT_edge (const Interrupt_INT_t *obj);
static Std_ReturnType Interrupt_INT_periority (const Interrupt_INT_t *obj);
static Std_ReturnType Interrupt_INT_pin_init (const Interrupt_INT_t *obj);
static Std_ReturnType Interrupt_INT_Clearflag (const Interrupt_INT_t *obj);

Std_ReturnType Interrupt_PORTB_Init (const Interrupt_PORTB_t *obj);
Std_ReturnType Interrupt_PORTB_DeInit (const Interrupt_PORTB_t *obj);

static Std_ReturnType INT0_SetHandler (void (* INTHANDLER) (void));
static Std_ReturnType INT1_SetHandler (void (* INTHANDLER) (void));
static Std_ReturnType INT2_SetHandler (void (* INTHANDLER) (void));
static Std_ReturnType Inturrupt_INTx_SetHandler (const Interrupt_INT_t *obj);




Std_ReturnType Interrupt_INT_Init (const Interrupt_INT_t *obj){
    Std_ReturnType ret = E_OK;
    if (NULL == obj){
        ret = E_NOT_OK;
    }
    else {
        /*Disable The External Interrupt*/
        ret = Interrupt_INT_disa (obj);
        /*Clear Interrupt Flag*/
        ret = Interrupt_INT_Clearflag (obj);
        /*configure The External edge*/
        ret = Interrupt_INT_edge (obj);
        /*configure The External priority*/
        #ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE
        ret = Interrupt_INT_periority (obj);
        #endif
        /*configure The External I/O pin*/
        ret = Interrupt_INT_pin_init (obj);
        /*Call Back Function*/
        ret = Inturrupt_INTx_SetHandler (obj);
        /*Enable The External Interrupt*/
        ret = Interrupt_INT_Ena (obj);
        }
    return ret; 
}

void INT0_ISR (void){
    /*clear the flag*/
    EXT_INT0_INTERRUPT_FlagClear();
    /*code*/
    
    /*call back function*/
    if (INT0_INTHANDLER){INT0_INTHANDLER();}
}

void INT1_ISR (void){
    /*clear the flag*/
    EXT_INT1_INTERRUPT_FlagClear();
    /*code*/
    
    /*call back function*/
    if (INT1_INTHANDLER){INT1_INTHANDLER();}
}

void INT2_ISR (void){
    /*clear the flag*/
    EXT_INT2_INTERRUPT_FlagClear();
    /*code*/
    
    /*call back function*/
    if (INT2_INTHANDLER){INT2_INTHANDLER();}
}

void RB4_ISR (uint8 logic){
    /*clear the flag*/
    EXT_PORTB_INTERRUPT_FlagClear();
    /*code*/
    
    /*call back function*/
    if (logic == 1){
        if (RB4_INTHANDLER_HIGH) {RB4_INTHANDLER_HIGH();}
    }
    else if (logic == 0){
        if (RB4_INTHANDLER_LOW) {RB4_INTHANDLER_LOW();}
    }
}
void RB5_ISR (uint8 logic){
    /*clear the flag*/
    EXT_PORTB_INTERRUPT_FlagClear();
    /*code*/
    
    /*call back function*/
    if (logic == 1){
        if (RB5_INTHANDLER_HIGH) {RB5_INTHANDLER_HIGH();}
    }
    else if (logic == 0){
        if (RB5_INTHANDLER_LOW) {RB5_INTHANDLER_LOW();}
    }
}
void RB6_ISR (uint8 logic){
    /*clear the flag*/
    EXT_PORTB_INTERRUPT_FlagClear();
    /*code*/
    
    /*call back function*/
    if (logic == 1){
        if (RB6_INTHANDLER_HIGH) {RB6_INTHANDLER_HIGH();}
    }
    else if (logic == 0){
        if (RB6_INTHANDLER_LOW) {RB6_INTHANDLER_LOW();}
    }
}
void RB7_ISR (uint8 logic){
    /*clear the flag*/
    EXT_PORTB_INTERRUPT_FlagClear();
    /*code*/
    
    /*call back function*/
    if (logic == 1){
        if (RB7_INTHANDLER_HIGH) {RB7_INTHANDLER_HIGH();}
    }
    else if (logic == 0){
        if (RB7_INTHANDLER_LOW) {RB7_INTHANDLER_LOW();}
    }
}

Std_ReturnType Interrupt_INT_DeInit (const Interrupt_INT_t *obj){
    Std_ReturnType ret = E_OK;
    if (NULL == obj){
        ret = E_NOT_OK;
    }
    else {
        ret = Interrupt_INT_disa (obj);
    }
    return ret; 
}    


Std_ReturnType Interrupt_PORTB_Init (const Interrupt_PORTB_t *obj){
    Std_ReturnType ret = E_OK;
    if (NULL == obj){
        ret = E_NOT_OK;
    }
    else {
        /*Disable The External Interrupt*/
        EXT_PORTB_INTERRUPT_DISABLE();
        /*Clear Interrupt Flag*/
        EXT_PORTB_INTERRUPT_FlagClear();
        /*configure The External priority*/
        #ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE
        INTERRUPT_PriorityLevelEnable();
        if(PRIORITY_HIGH == obj->priority){ 
            INTERRUPT_GlobalHighEnable();
            EXT_PORTB_INTERRUPT_PriortyHigh();
        }
        else if(PRIORITY_LOW == obj->priority){ 
            INTERRUPT_GlobalLowEnable();
            EXT_PORTB_INTERRUPT_PriortyLow();
        }
        #else
            INTERRUPT_GlobalEnabl();
            INTERRUPT_PeripheralEnable();
        #endif
        /*configure The External I/O pin*/
        ret = gpio_pin_direction_intialize(&(obj->pin_int));
        /*Call Back Function*/
        switch (obj->pin_int.pin) {
            case PIN4 : 
                RB4_INTHANDLER_HIGH = obj->EXT_INTERRUPT_HIGH;
                RB4_INTHANDLER_LOW  = obj->EXT_INTERRUPT_LOW; 
                break;
            case PIN5 : 
                RB5_INTHANDLER_HIGH = obj->EXT_INTERRUPT_HIGH;
                RB5_INTHANDLER_LOW  = obj->EXT_INTERRUPT_LOW;   
                break;
            case PIN6 : 
                RB6_INTHANDLER_HIGH = obj->EXT_INTERRUPT_HIGH;
                RB6_INTHANDLER_LOW  = obj->EXT_INTERRUPT_LOW;   
                break;    
            case PIN7 : 
                RB7_INTHANDLER_HIGH = obj->EXT_INTERRUPT_HIGH;
                RB7_INTHANDLER_LOW  = obj->EXT_INTERRUPT_LOW;  
                break;        
            default : ret = E_NOT_OK;           
        }
        /*Enable The External Interrupt*/
        EXT_PORTB_INTERRUPT_ENABLE();
    }
    return ret; 
}


Std_ReturnType Interrupt_PORTB_DeInit (const Interrupt_PORTB_t *obj){
    Std_ReturnType ret = E_OK;
    if (NULL == obj){
        ret = E_NOT_OK;
    }
    else {
        EXT_PORTB_INTERRUPT_DISABLE();
        ret = E_OK;
    }
    return ret; 
}

static Std_ReturnType Interrupt_INT_Ena (const Interrupt_INT_t *obj){
    Std_ReturnType ret = E_OK;
    if (NULL == obj){
        ret = E_NOT_OK;
    }
    else {
        switch (obj->source) {
            case Interrupt_INT0 :
                #ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE
                INTERRUPT_PriorityLevelEnable();
                INTERRUPT_GlobalHighEnable();
                #else
                INTERRUPT_GlobalEnabl();
                INTERRUPT_PeripheralEnable ();
                #endif
                EXT_INT0_INTERRUPT_ENABLE();
                ret = E_OK;
                break;
            case Interrupt_INT1 : 
                #ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE
                INTERRUPT_PriorityLevelEnable();    
                if(PRIORITY_HIGH == obj->priority){ INTERRUPT_GlobalHighEnable(); }
                else if(PRIORITY_LOW == obj->priority){ INTERRUPT_GlobalLowEnable(); }
                #else
                INTERRUPT_GlobalEnabl();
                INTERRUPT_PeripheralEnable ();     
                #endif
                EXT_INT1_INTERRUPT_ENABLE();
                ret = E_OK;
                break;
            case Interrupt_INT2 :
                #ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE
                INTERRUPT_PriorityLevelEnable();  
                if(PRIORITY_HIGH == obj->priority){ INTERRUPT_GlobalHighEnable(); }
                else if(PRIORITY_LOW == obj->priority){ INTERRUPT_GlobalLowEnable (); }
                #else
                INTERRUPT_GlobalEnabl();
                INTERRUPT_PeripheralEnable (); 
                #endif
                EXT_INT2_INTERRUPT_ENABLE();
                ret = E_OK;
                break;    
            default : ret = E_NOT_OK;    
        }
    }
    return ret;     
}


static Std_ReturnType Interrupt_INT_disa (const Interrupt_INT_t *obj){
    Std_ReturnType ret = E_OK;
    if (NULL == obj){
        ret = E_NOT_OK;
    }
    else {
        switch (obj->source) {
            case Interrupt_INT0 : 
                EXT_INT0_INTERRUPT_DISABLE();
                ret = E_OK;
                break;
            case Interrupt_INT1 : 
                EXT_INT1_INTERRUPT_DISABLE();
                ret = E_OK;
                break;
            case Interrupt_INT2 : 
                EXT_INT2_INTERRUPT_DISABLE();
                ret = E_OK;
                break;    
            default : ret = E_NOT_OK;           
        }
    }
    return ret;    
}

static Std_ReturnType Interrupt_INT_edge (const Interrupt_INT_t *obj){
    Std_ReturnType ret = E_OK;
    if (NULL == obj){
        ret = E_NOT_OK;
    }
    else {
        switch (obj->source) {
            case Interrupt_INT0 : 
                if(INTERRUPT_RiseEdge == obj->edge){ EXT_INT0_INTERRUPT_RiseEdge(); }
                else if(INTERRUPT_fallEdge == obj->edge){ EXT_INT0_INTERRUPT_FallEdge(); }
                ret = E_OK;
                break;
            case Interrupt_INT1 : 
                if(INTERRUPT_RiseEdge == obj->edge){ EXT_INT1_INTERRUPT_RiseEdge(); }
                else if(INTERRUPT_fallEdge == obj->edge){ EXT_INT1_INTERRUPT_FallEdge(); }
                ret = E_OK;
                break;
            case Interrupt_INT2 : 
                if(INTERRUPT_RiseEdge == obj->edge){ EXT_INT2_INTERRUPT_RiseEdge(); }
                else if(INTERRUPT_fallEdge == obj->edge){ EXT_INT2_INTERRUPT_FallEdge(); }
                ret = E_OK;
                break;    
            default : ret = E_NOT_OK;           
        }
    }
    return ret;      
}
#ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE
static Std_ReturnType Interrupt_INT_periority (const Interrupt_INT_t *obj){
    Std_ReturnType ret = E_OK;
    if (NULL == obj){
        ret = E_NOT_OK;
    }
    else {
        switch (obj->source) {
            case Interrupt_INT1 : 
                if(PRIORITY_HIGH == obj->priority){ EXT_INT1_INTERRUPT_PriortyHigh(); }
                else if(PRIORITY_LOW == obj->priority){ EXT_INT1_INTERRUPT_PriortyLow(); }
                ret = E_OK;
                break;
            case Interrupt_INT2 : 
                if(PRIORITY_HIGH == obj->priority){ EXT_INT2_INTERRUPT_PriortyHigh(); }
                else if(PRIORITY_LOW == obj->priority){ EXT_INT2_INTERRUPT_PriortyLow(); }
                ret = E_OK;
                break;    
            default : ret = E_NOT_OK;           
        }
    }
    return ret;       
}
#endif
static Std_ReturnType Interrupt_INT_pin_init (const Interrupt_INT_t *obj){
    Std_ReturnType ret = E_OK;
    if (NULL == obj){
        ret = E_NOT_OK;
    }
    else {
        ret = gpio_pin_direction_intialize (&(obj->pin_int));
    }
    return ret;         
}

static Std_ReturnType Interrupt_INT_Clearflag (const Interrupt_INT_t *obj){
    Std_ReturnType ret = E_OK;
    if (NULL == obj){
        ret = E_NOT_OK;
    }
    else {
        switch (obj->source) {
            case Interrupt_INT0 : 
                EXT_INT0_INTERRUPT_FlagClear();
                ret = E_OK;
                break;
            case Interrupt_INT1 : 
                EXT_INT1_INTERRUPT_FlagClear();
                ret = E_OK;
                break;
            case Interrupt_INT2 : 
                EXT_INT2_INTERRUPT_FlagClear();
                ret = E_OK;
                break;    
            default : ret = E_NOT_OK;           
        }
    }
    return ret;    
}

static Std_ReturnType INT0_SetHandler (void (* INTHANDLER) (void)){
    Std_ReturnType ret = E_OK;
    if (NULL == INTHANDLER){
        ret = E_NOT_OK;
    }
    else {
          INT0_INTHANDLER = INTHANDLER;
          ret = E_OK;
        }
    return ret;        
}
static Std_ReturnType INT1_SetHandler (void (* INTHANDLER) (void)){
    Std_ReturnType ret = E_OK;
    if (NULL == INTHANDLER){
        ret = E_NOT_OK;
    }
    else {
          INT1_INTHANDLER = INTHANDLER;
          ret = E_OK;
        }
    return ret;   
}
static Std_ReturnType INT2_SetHandler (void (* INTHANDLER) (void)){
    Std_ReturnType ret = E_OK;
    if (NULL == INTHANDLER){
        ret = E_NOT_OK;
    }
    else {
          INT2_INTHANDLER = INTHANDLER;
          ret = E_OK;
        }
    return ret;  
}
static Std_ReturnType Inturrupt_INTx_SetHandler (const Interrupt_INT_t *obj){
    Std_ReturnType ret = E_OK;
    if (NULL == obj){
        ret = E_NOT_OK;
    }
    else {
        switch (obj->source) {
            case Interrupt_INT0 : 
                ret = INT0_SetHandler(obj->EXT_INTERRUPT);
                break;
            case Interrupt_INT1 : 
                ret = INT1_SetHandler(obj->EXT_INTERRUPT);
                break;
            case Interrupt_INT2 : 
                ret = INT2_SetHandler(obj->EXT_INTERRUPT);
                break;    
            default : ret = E_NOT_OK;           
        }
    }
    return ret;    
}