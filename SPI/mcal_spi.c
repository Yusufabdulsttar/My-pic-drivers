/* 
 * File:   mcal_spi.c
 * Author: yusuf
 *
 * Created on May 13, 2023, 2:28 PM
 */

#include "mcal_spi.h"


#ifdef SPI_INTERRUPT 
static void (* SPI_InterruptHandler)(void);
#endif


Std_ReturnType spi_init(const spi_t *spi){
    Std_ReturnType ret = E_OK;
    if (NULL == spi){
        ret = E_NOT_OK;
    }
    else {
        /* Disable SPI */
        SPI_Disable();
        /*select master or slave*/
        if (SPI_MASTER_MODE == spi->spi_select_mode){
            /* sck is configure to be o/p*/
            TRISCbits.TRISC3 = 0; 
            /* configure sample bit (read the data at middle of data o/p time or at end */
            SSPSTATbits.SMP = spi->spi_clock_sample;
            /* pin choose to control SS pin*/
            gpio_pin_config_intialize(&(spi->ss_pin));
        }
        else if (SPI_SLAVE_MODE == spi->spi_select_mode){
            /* sck is configure to be i/p*/
            TRISCbits.TRISC3 = 1; 
            /* clear sample bit (read the data at middle of data o/p time */
            SSPSTATbits.SMP = SPI_READ_AT_MIDDLE;
            /* SS pin is set in slave mode*/
            TRISAbits.TRISA5 = 1;
        }
        /* configure clock rate */
        SSPCON1bits.SSPM = spi->clock_rate;
        /* select clock priority */
        SSPCON1bits.CKP = spi->spi_clock_priority;
        /* select clock phase */
        SSPSTATbits.CKE = spi->spi_clock_phase;
        /* Clear SDO pin*/
        TRISCbits.TRISC5 = 0;
        /* configure interrupt*/
#ifdef SPI_INTERRUPT 
        SPI_INTERRUPT_FlagClear();
        SPI_INTERRUPT_ENABLE();
        SPI_InterruptHandler = spi->SPI_InterruptHandler;
#ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE 
        INTERRUPT_PriorityLevelEnable();
        if (PRIORITY_HIGH == spi->priority){
            INTERRUPT_GlobalHighEnable();
            SPI_INTERRUPT_PriortyHigh();
        }
        else if(PRIORITY_LOW == spi->priority){
            INTERRUPT_GlobalLowEnable();
            SPI_INTERRUPT_PriortyLow();
        } 
#else
        INTERRUPT_GlobalEnabl();
        INTERRUPT_PeripheralEnable();
#endif
#endif
        /* Enable SPI */
        SPI_ENABLE();
    }
    return ret; 
    
}


Std_ReturnType spi_deinit(void){
    Std_ReturnType ret = E_OK;
    
#ifdef SPI_INTERRUPT
        SPI_INTERRUPT_FlagClear();
        SPI_INTERRUPT_DISABLE();
#endif
        SPI_Disable();
        
    return ret; 
}

Std_ReturnType spi_read_non_bloking(uint8 *data){
    Std_ReturnType ret = E_OK;
    if (SPI_RECEIVE_COMPLETE == SSPSTATbits.BF){
    *data = SSPBUF; 
    }
    else if(SPI_RECEIVE_NOT_COMPLETE == SSPSTATbits.BF){
        ret = E_NOT_OK;
    }
    return ret; 
}

Std_ReturnType spi_read_bloking(uint8 *data){
    Std_ReturnType ret = E_OK;
    while (!SSPSTATbits.BF);
    *data = SSPBUF; 
    return ret; 
}

Std_ReturnType spi_write(uint8 data){
    Std_ReturnType ret = E_OK;
    uint8 dummydatat;
    dummydatat = SSPBUF;
    SSPBUF = data;
    if (SPI_OVERFLOW_OCCURE == SSPCON1bits.SSPOV){
        dummydatat = SSPBUF;
        SSPCON1bits.SSPOV = SPI_NO_OVERFLOW;
    }
    /* collision detect */
    if (SPI_COLLISION_DETECTED == SSPCON1bits.WCOL){
        ret = E_NOT_OK;
        /* clear the collision detect bit (must clear in the software) */
        SSPCON1bits.WCOL = SPI_NO_CLOOISION;
    }
    else if(SPI_NO_CLOOISION == SSPCON1bits.WCOL){
        ret = E_OK;
    }
    return ret; 
}

Std_ReturnType spi_write_string(uint8 *data,uint16 str_len,uint8 *data_read){
    Std_ReturnType ret = E_OK;
    uint8 counter ,datatemp=0;
    for (counter = 0 ; counter < str_len ; counter++){
        ret = spi_write(data[counter]);
        ret = spi_read_non_bloking(&datatemp);
        *data_read = datatemp;
    }
    return ret; 
}


void SPI_ISR(void){
    #ifdef SPI_INTERRUPT
    SPI_INTERRUPT_FlagClear();
   /*code*/

    if(SPI_InterruptHandler) {
        SPI_InterruptHandler();
    }
    #endif
}