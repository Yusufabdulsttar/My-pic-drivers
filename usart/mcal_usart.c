/* 
 * File:   mcal_usart.c
 * Author: yusuf
 *
 * Created on May 10, 2023, 5:33 PM
 */
#include "mcal_usart.h"

static Std_ReturnType baud_rate_gen(const usart_t *usart);
static Std_ReturnType usart_async_TX_init(const usart_t *usart);
static Std_ReturnType usart_async_RX_init(const usart_t *usart);

#ifdef USART_TX_INTERRUPT 
static void (* USART_TxInterruptHandler)(void);
#endif

#ifdef USART_RX_INTERRUPT 
static void (* USART_RxInterruptHandler)(void);
#endif

Std_ReturnType usart_asy_init(const usart_t *usart){
    Std_ReturnType ret = E_OK;
    if (NULL == usart){
        ret = E_NOT_OK;
    }
    else {
        /* Disable usart*/
        RCSTAbits.SPEN = USART_DISABLE;
        /*cfg tx and rx pin to be input*/
        TRISCbits.RC7 = 1;
        TRISCbits.RC6 = 1;
        /*baud rate init*/
        baud_rate_gen(usart);
        /* tx cfg*/
        usart_async_TX_init(usart);
        /*rx cfg*/
        usart_async_RX_init(usart);
        /*Enable usart*/
        RCSTAbits.SPEN = USART_ENABLE;
        ret = E_OK;
    }
    return ret; 
    
}


Std_ReturnType usart_asy_deinit(const usart_t *usart){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == usart){
        ret = E_NOT_OK;
    }
    else{
        RCSTAbits.SPEN = USART_DISABLE;
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType usart_asy_read(uint16 *data){
    Std_ReturnType ret = E_NOT_OK;
    if(1 == PIR1bits.RCIF){
        *data = RCREG;
        ret = E_OK;
    }
    else{
        ret = E_NOT_OK;
    }
    return ret;
}

Std_ReturnType usart_asy_write(uint8 data){
    Std_ReturnType ret = E_OK;
    uint16 d = 0;
    while(!TXSTAbits.TRMT){d++;};
#ifdef USART_TX_INTERRUPT
     USART_TX_INTERRUPT_ENABLE();
#endif
    TXREG = data;
    return ret;
}

Std_ReturnType usart_asy_write_string(uint8 *data,uint16 str_len){
    Std_ReturnType ret = E_OK;
    uint16 char_counter = 0;
    for(char_counter=0; char_counter < str_len; char_counter++){
        ret = usart_asy_write(data[char_counter]);
    }
    return ret;
}

static Std_ReturnType usart_async_TX_init(const usart_t *usart){
    Std_ReturnType ret = E_OK;
    if (NULL == usart){
        ret = E_NOT_OK;
    }
    else {
        TXSTAbits.TXEN = USART_ASYNCHRONOUS_TX_ENABLE;
        USART_TxInterruptHandler = usart->USART_TxInterruptHandler;
        /* USART Transmit Interrupt Configuration */
            if(USART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE == usart->usart_tx_interrupt){
            /* Interrupt Configurations */ 
#ifdef USART_TX_INTERRUPT 
            USART_TX_INTERRUPT_ENABLE();        
            /* Interrupt Priority Configurations */
#ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE 
        INTERRUPT_PriorityLevelEnable();
        if (PRIORITY_HIGH == usart->priority_tx){
            INTERRUPT_GlobalHighEnable();
            USART_TX_INTERRUPT_PriortyHigh();
        }
        else if(PRIORITY_LOW == usart->priority_tx){
            INTERRUPT_GlobalLowEnable();
            USART_TX_INTERRUPT_PriortyLow();
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

static Std_ReturnType usart_async_RX_init(const usart_t *usart){
    Std_ReturnType ret = E_OK;
    if (NULL == usart){
        ret = E_NOT_OK;
    }
    else {
        RCSTAbits.CREN = USART_ASYNCHRONOUS_RX_ENABLE;
        USART_RxInterruptHandler = usart->USART_RxInterruptHandler;
        /* USART Receiver Interrupt Configuration */
            if(USART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE == usart->usart_rx_interrupt){
            /* Interrupt Configurations */ 
#ifdef USART_RX_INTERRUPT 
            USART_RX_INTERRUPT_ENABLE();        
            /* Interrupt Priority Configurations */
#ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE 
        INTERRUPT_PriorityLevelEnable();
        if (PRIORITY_HIGH == usart->priority_rx){
            INTERRUPT_GlobalHighEnable();
            USART_RX_INTERRUPT_PriortyHigh();
        }
        else if(PRIORITY_LOW == usart->priority_rx){
            INTERRUPT_GlobalLowEnable();
            USART_RX_INTERRUPT_PriortyLow();
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

static Std_ReturnType baud_rate_gen(const usart_t *usart){
    Std_ReturnType ret = E_OK;
    if (NULL == usart){
        ret = E_NOT_OK;
    }
    else {
        float tempreg = 0;
        switch(usart->baudrate_gen){
            case BAUDRATE_ASYN_8BIT_lOW_SPEED :
                TXSTAbits.SYNC = 0;
                TXSTAbits.BRGH =  0;
                BAUDCONbits.BRG16 = 0;       
                tempreg = ((_XTAL_FREQ /(float)usart->baud_rate)/64)- 1;
                break;
            case BAUDRATE_ASYN_8BIT_HIGH_SPEED :
                TXSTAbits.SYNC = 0;
                TXSTAbits.BRGH =  1;
                BAUDCONbits.BRG16 = 0;  
                tempreg = ((_XTAL_FREQ /(float)usart->baud_rate)/16)- 1;
                break;
            case BAUDRATE_ASYN_16BIT_lOW_SPEED :
                TXSTAbits.SYNC = 0;
                TXSTAbits.BRGH =  0;
                BAUDCONbits.BRG16 = 1;   
                tempreg = ((_XTAL_FREQ /(float)usart->baud_rate)/16)- 1;
                break;
            case BAUDRATE_ASYN_16BIT_HIGH_SPEED :
                TXSTAbits.SYNC = 0;
                TXSTAbits.BRGH =  1;
                BAUDCONbits.BRG16 = 1; 
                tempreg = ((_XTAL_FREQ /(float)usart->baud_rate)/4)- 1;
                break;
        }
        SPBRG = (uint8)((uint32)tempreg);
        SPBRGH = (uint8)(((uint32)tempreg) >> 8);
    }
    return ret; 
}

void USART_TX_ISR(void){
    USART_TX_INTERRUPT_DISABLE();
    if(USART_TxInterruptHandler){
        USART_TxInterruptHandler();
    }else { /* Nothing */}
}

void USART_RX_ISR(void){
    if(USART_RxInterruptHandler){
        USART_RxInterruptHandler();
    }else { /* Nothing */}
}
