/* 
 * File:   mcal_i2c.c
 * Author: yusuf
 *
 * Created on May 16, 2023, 1:18 PM
 */
#include "mcal_i2c.h"

#ifdef SPI_INTERRUPT 
static void (*I2C_Report_Write_Collision_InterruptHandler)(void);
static void (*I2C_DefaultInterruptHandle)(void);
static void (*I2C_Report_Receive_Overflow_InterruptHandle)(void);
#endif


Std_ReturnType i2c_init(const i2c_t *i2c){
    Std_ReturnType ret = E_OK;
    if (NULL == i2c){
        ret = E_NOT_OK;
    }
    else {
        /* Disable mssp module*/
        MSSP_DISABLE();
        /* Master Synchronous Serial Port Mode Select bits */
        SSPCON1bits.SSPM = i2c->mode_cfg;
        /*configure master or slave mode initialize */
        if(I2C_MASTER_MODE == i2c->i2c_mode){
                /*  I2C Master mode, clock = FOSC/(4 * (SSPADD + 1)) */
                SSPADD = (uint8)(((_XTAL_FREQ / 4.0) / i2c->clock) - 1);
        }
        else if(I2C_SLAVE_MODE == i2c->i2c_mode){
            /* Clear the Write Collision Detect */
            SSPCON1bits.WCOL = 0;
            /* Clear the Receive Overflow Indicator */
            SSPCON1bits.SSPOV = 0; 
            /* Release the clock */
            SSPCON1bits.CKP = 1;
            /* Assign the I2C Slave Address */
            SSPADD = i2c->i2c_slave_address;
        }
        /* Serial clock (SCL) is Input */
        TRISCbits.TRISC3 = 1; 
        /* Serial data (SDA) is Input */
        TRISCbits.TRISC4 = 1; 
        /* Slew rate control */
        SSPSTATbits.SMP = i2c->slew_rate;
        /* interrupt */
#ifdef I2C_INTERRUPT
        I2C_INTERRUPT_ENABLE();
        I2C_BUS_COL_INTERRUPT_ENABLE();
        I2C_INTERRUPT_FlagClear();
        I2C_BUS_COL_INTERRUPT_FlagClear();
        I2C_Report_Write_Collision_InterruptHandler = i2c->I2C_Report_Write_Collision;
        I2C_DefaultInterruptHandle = i2c->I2C_DefaultInterruptHandler;
        I2C_Report_Receive_Overflow_InterruptHandle = i2c->I2C_Report_Receive_Overflow;
/* Interrupt Priority Configurations */
#ifdef INTERRUPT_PRIORITY_LEVEL_ENABLE 
        INTERRUPT_PriorityLevelEnable();
        if(PRIORITY_HIGH == i2c->i2c_priority){
            /* Enables all high-priority interrupts */
            INTERRUPT_GlobalHighEnable();
            I2C_INTERRUPT_PriortyHigh();
        }
        else if(PRIORITY_LOW == i2c->i2c_priority){
            /* Enables all unmasked peripheral interrupts */
            INTERRUPT_GlobalLowEnable();
            I2C_INTERRUPT_PriortyLow();
        }
        if(PRIORITY_HIGH == i2c->i2c_BUS_COL_priority){
            /* Enables all high-priority interrupts */
            INTERRUPT_GlobalHighEnable();
            I2C_BUS_COL_INTERRUPT_PriortyHigh();
        }
        else if(PRIORITY_LOW == i2c->i2c_BUS_COL_priority){
            /* Enables all unmasked peripheral interrupts */
            INTERRUPT_GlobalHighEnable();
            I2C_BUS_COL_INTERRUPT_PriortyLow();
        }
#else
        INTERRUPT_GlobalEnabl();
        INTERRUPT_PeripheralEnable();
#endif    
#endif           
        /* Enable mssp module*/
        MSSP_ENABLE();
    }
    return ret;
}

Std_ReturnType i2c_Deinit(void){
    Std_ReturnType ret = E_OK;
        /* Disable mssp module*/
        MSSP_DISABLE();
#ifdef I2C_INTERRUPT
        I2C_INTERRUPT_DISABLE();
        I2C_BUS_COL_INTERRUPT_DISABLE();
#endif        
    return ret;
}


Std_ReturnType i2c_send_start(void){
    Std_ReturnType ret = E_OK;
        /* Generates a Start condition */
        SSPCON2bits.SEN = 1;
        /* Indicates that a Start bit has been transmitted */
        while(SSPCON2bits.SEN); 
        /* Clear the flag*/
        PIR1bits.SSPIF = 0;
    return ret;
}


Std_ReturnType i2c_send_Repeated_start(void){
    Std_ReturnType ret = E_OK;
        /* Generates a Repeated Start condition */
        SSPCON2bits.RSEN = 1;
        /* Indicates that a Repeated Start bit has been transmitted */
        while(SSPCON2bits.RSEN); 
        /* Clear the flag*/
        PIR1bits.SSPIF = 0;
    return ret;
}


Std_ReturnType i2c_send_stop(void){
    Std_ReturnType ret = E_OK;
        /* Generates a stop condition */
        SSPCON2bits.PEN = 1;
        /* Indicates that a stop bit has been transmitted */
        while(SSPCON2bits.PEN); 
        /* Clear the flag*/
        PIR1bits.SSPIF = 0;
    return ret;  
}


Std_ReturnType i2c_master_write(uint8 data,uint8 *ack){
    Std_ReturnType ret = E_OK;
    if (NULL == ack){
        ret = E_NOT_OK;
    }
    else {
        /* write data to the data register */
        SSPBUF = data;
        /* wait until transmit is compleat*/
        while(SSPSTATbits.BF);
        /* clear the flag */
        PIR1bits.SSPIF = 0;
        /* report acknowledge from slave */
        if (I2C_ACKN_RECEIVED_FROM_SLAVE == SSPCON2bits.ACKSTAT){
            *ack = I2C_ACKN_RECEIVED_FROM_SLAVE;
        } 
        else if (I2C_ACKN_NOT_RECEIVED_FROM_SLAVE == SSPCON2bits.ACKSTAT){
            *ack = I2C_ACKN_NOT_RECEIVED_FROM_SLAVE;
        }
    }
    return ret;
}


Std_ReturnType i2c_master_read(uint8 ack, uint8 *data){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == data)){
        ret = E_NOT_OK;
    }
    else{
        /* Master Mode Receive Enable */
        SSPCON2bits.RCEN = 1;
        /* Wait for buffer full flag : A complete byte received */
        while(!SSPSTATbits.BF);
        /* Copy The data registers to buffer variable */
        *data = SSPBUF;
        /* Send ACK or NACK after read */
        if(I2C_MASTER_SEND_ACK == ack){
            SSPCON2bits.ACKDT = 0; /* Acknowledge */
            /* Initiates Acknowledge sequence on SDA and SCL pins and transmit ACKDT data bit. */
            SSPCON2bits.ACKEN = 1; 
            while(SSPCON2bits.ACKEN); /* Automatically cleared by hardware */
        }
        else if(I2C_MASTER_SEND_NACK == ack){
            SSPCON2bits.ACKDT = 1; /* Not Acknowledge */
            /* Initiates Acknowledge sequence on SDA and SCL pins and transmit ACKDT data bit. */
            SSPCON2bits.ACKEN = 1;
            while(SSPCON2bits.ACKEN); /* Automatically cleared by hardware */
        }
        else { /* Nothing */ }
        ret = E_OK;
    }
    return ret;
}


void I2C_ISR (void){
#ifdef I2C_INTERRUPT
    I2C_INTERRUPT_FlagClear();
   /*code*/   
    if(I2C_DefaultInterruptHandle){
        I2C_DefaultInterruptHandle();
    }
#endif   
}

void I2C_BUS_COL_ISR (void){
#ifdef I2C_INTERRUPT
    I2C_BUS_COL_INTERRUPT_FlagClear();
   /*code*/   
    if(I2C_Report_Write_Collision_InterruptHandler){
        I2C_Report_Write_Collision_InterruptHandler();
    }
#endif   
}

