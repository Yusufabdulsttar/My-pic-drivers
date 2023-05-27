/* 
 * File:   mcal_usart.h
 * Author: yusuf
 *
 * Created on May 10, 2023, 5:33 PM
 */

#ifndef MCAL_USART_H
#define	MCAL_USART_H

/* section : Includes   */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"
/* section : Micro Declaration  */
#define USART_ENABLE   1
#define USART_DISABLE  0

#define USART_ASYNCHRONOUS_TX_ENABLE    1 
#define USART_ASYNCHRONOUS_TX_Disable   0 

#define USART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE   1
#define USART_ASYNCHRONOUS_INTERRUPT_TX_DISABLE  0

#define USART_ASYNCHRONOUS_RX_ENABLE    1 
#define USART_ASYNCHRONOUS_RX_Disable   0 

#define USART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE   1
#define USART_ASYNCHRONOUS_INTERRUPT_RX_DISABLE  0
/* section : Micro Function Declaration  */

/* section : Data Type Declaration  */
typedef enum{
    BAUDRATE_ASYN_8BIT_lOW_SPEED,
    BAUDRATE_ASYN_8BIT_HIGH_SPEED,
    BAUDRATE_ASYN_16BIT_lOW_SPEED,   
    BAUDRATE_ASYN_16BIT_HIGH_SPEED,
}baudrate_gen_t;

typedef struct{
    uint32 baud_rate;
    Interrupt_priority_t priority_tx;
    Interrupt_priority_t priority_rx;
    baudrate_gen_t baudrate_gen;
    uint8 usart_tx_interrupt;
    uint8 usart_rx_interrupt;
#ifdef USART_TX_INTERRUPT 
    void (* USART_TxInterruptHandler)(void);
#endif
#ifdef USART_RX_INTERRUPT 
    void (* USART_RxInterruptHandler)(void);
#endif
}usart_t;
/* section : Function Declaration  */
Std_ReturnType usart_asy_init(const usart_t *usart);
Std_ReturnType usart_asy_deinit(const usart_t *usart);
Std_ReturnType usart_asy_read(uint16 *data);
Std_ReturnType usart_asy_write(uint8 data);
Std_ReturnType usart_asy_write_string(uint8 *data,uint16 str_len);


#endif	/* MCAL_USART_H */

