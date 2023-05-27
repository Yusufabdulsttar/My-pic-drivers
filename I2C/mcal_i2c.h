/* 
 * File:   mcal_i2c.h
 * Author: yusuf
 *
 * Created on May 16, 2023, 1:18 PM
 */

#ifndef MCAL_I2C_H
#define	MCAL_I2C_H


/* section : Includes   */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"
/* section : Micro Declaration  */

/* Acknowledge Status bit (Master Transmit mode only) */
#define I2C_ACKN_RECEIVED_FROM_SLAVE       0
#define I2C_ACKN_NOT_RECEIVED_FROM_SLAVE   1

/* mode configure */
#define I2C_MASTER_MODE   1
#define I2C_SLAVE_MODE    0
/* Acknowledge Data bit (Master Receive mode only) */
#define I2C_MASTER_SEND_ACK  0
#define I2C_MASTER_SEND_NACK 1
/* Slow rate Control */
#define I2C_STANDARD_SPEED_MODE100   1
#define I2C_HIGH_SPEED_MODE400       0

/* Master Synchronous Serial Port Mode Select bits */
#define I2C_Slave_mode_7bit                      0x06U 
#define I2C_Slave_mode_10bit                     0x07U
#define I2C_Slave_mode_7bit_with_interrupt       0x0EU
#define I2C_Slave_mode_10bit_with_interrupt      0x0FU
#define I2C_Master_mode_Firmware_Controlled      0x0BU
#define I2C_Master_mode_Defined_Clock            0x08U


/* section : Micro Function Declaration  */

#define MSSP_ENABLE()    (SSPCON1bits.SSPEN = 1)
#define MSSP_DISABLE()   (SSPCON1bits.SSPEN = 0)


/* section : Data Type Declaration  */
 
typedef struct {
    uint32 clock;	       /* Master Clock Frequency */
    Interrupt_priority_t i2c_priority;
    Interrupt_priority_t i2c_BUS_COL_priority;
    uint8 i2c_slave_address; /* I2C Slave Address (7bit address and first bit for R/W)*/
    uint8 i2c_mode :1;     /* mode configure */
    uint8 slew_rate :1;    /* Slow rate Control */
    uint8 mode_cfg :4;    /* Master Synchronous Serial Port Mode Select bits */
#ifdef SPI_INTERRUPT 
    void (* I2C_Report_Write_Collision)(void);
    void (* I2C_DefaultInterruptHandler)(void);
    void (* I2C_Report_Receive_Overflow)(void);
#endif
}i2c_t;

/* section : Function Declaration  */

Std_ReturnType i2c_init(const i2c_t *i2c);
Std_ReturnType i2c_Deinit(void);
Std_ReturnType i2c_send_start(void);
Std_ReturnType i2c_send_Repeated_start(void);
Std_ReturnType i2c_send_stop(void);
Std_ReturnType i2c_master_write(uint8 data,uint8 *ack);
Std_ReturnType i2c_master_read(uint8 ack, uint8 *data);

#endif	/* MCAL_I2C_H */

