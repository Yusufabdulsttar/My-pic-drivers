/* 
 * File:   ecu_Relay.h
 * Author: yusuf
 *
 * Created on April 19, 2023, 10:54 PM
 */

#ifndef ECU_RELAY_H
#define	ECU_RELAY_H


/* section : Includes   */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecu_Relay_cfg.h"
/* section : Micro Declaration  */

/* section : Micro Function Declaration  */

/* section : Data Type Declaration  */
typedef enum {
    RELAY_OFF,
    RELAY_ON  
}relay_status_t;

typedef struct {
    uint8 port : 4 ;
    uint8 pin  : 3 ;
    uint8 relay_state : 1 ;
}relay_t;
/* section : Function Declaration  */

Std_ReturnType relayPinitialize (const relay_t *relay);
Std_ReturnType relay_on (const relay_t *relay);
Std_ReturnType relay_off (const relay_t *relay);


#endif	/* ECU_RELAY_H */

