/* 
 * File:   mcal_spi.h
 * Author: yusuf
 *
 * Created on May 13, 2023, 2:28 PM
 */

#ifndef MCAL_SPI_H
#define	MCAL_SPI_H

/* section : Includes   */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"
/* section : Micro Declaration  */

/* select mode */
#define SPI_MASTER_MODE           1
#define SPI_SLAVE_MODE            0

/* clock priority */
#define SPI_CLOCK_IDLE_HIGH       1
#define SPI_CLOCK_IDLE_LOW        0

/* clock phase */
#define SPI_FALLING_EDGE          1
#define SPI_RAISING_EDGE          0

/* for sample clock*/
#define SPI_READ_AT_END           1
#define SPI_READ_AT_MIDDLE        0

#define SPI_RECEIVE_COMPLETE      1
#define SPI_RECEIVE_NOT_COMPLETE  0

#define SPI_OVERFLOW_OCCURE       1
#define SPI_NO_OVERFLOW           0

#define SPI_COLLISION_DETECTED    1
#define SPI_NO_CLOOISION          0
/* section : Micro Function Declaration  */

#define SPI_ENABLE()    (SSPCON1bits.SSPEN = 1)
#define SPI_Disable()   (SSPCON1bits.SSPEN = 0)

/* section : Data Type Declaration  */

typedef enum {
    SPI_MASTER_FDIV_4,
    SPI_MASTER_FDIV_16,
    SPI_MASTER_FDIV_64,   
    SPI_MASTER_TMR2,
    SPI_SLAVE_SS_ENABLE,        
    SPI_SLAVE_SS_DISABLE        
}spi_clock_rate;

typedef struct {
#ifdef SPI_INTERRUPT 
    void (* SPI_InterruptHandler)(void);
#endif
    spi_clock_rate clock_rate;
    pin_config_t ss_pin;
    Interrupt_priority_t priority;
    uint8 spi_select_mode    :1;
    uint8 spi_clock_priority :1;
    uint8 spi_clock_phase    :1;
    uint8 spi_clock_sample   :1;
}spi_t;


/* section : Function Declaration  */
Std_ReturnType spi_init(const spi_t *spi);
Std_ReturnType spi_deinit(void);
Std_ReturnType spi_read_non_bloking(uint8 *data);
Std_ReturnType spi_read_bloking(uint8 *data);
Std_ReturnType spi_write(uint8 data);
Std_ReturnType spi_write_string(uint8 *data,uint16 str_len,uint8 *data_read);


#endif	/* MCAL_SPI_H */

