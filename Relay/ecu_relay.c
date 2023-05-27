/* 
 * File:   ecu_Relay.c
 * Author: yusuf
 *
 * Created on April 19, 2023, 10:54 PM
 */

#include "ecu_Relay.h"

/**
 * #brief initialize the assigned pin to be output
 * @param relay pointer to the relay configuration
 * @return Std_ReturnType :
 *            E_ok     : the function done successfully
 *            E_NOT_OK : the function has issue           
 */
Std_ReturnType relayPinitialize (const relay_t *relay){
    Std_ReturnType ret = E_OK;
    if (NULL == relay){
        ret = E_NOT_OK;
    }
    else {
        pin_config_t relay_0 = {.port = relay->port, .pin = relay->pin, .logic = relay->relay_state, .direction = dirc_OUTPUT };
        ret = gpio_pin_config_intialize (&relay_0);
    }
    return ret; 
}


/**
 * #brief write high on relay pin
 * @param relay pointer to the relay configuration
 * @return Std_ReturnType :
 *            E_ok     : the function done successfully
 *            E_NOT_OK : the function has issue           
 */
Std_ReturnType relay_on (const relay_t *relay){
    Std_ReturnType ret = E_OK;
    if (NULL == relay){
        ret = E_NOT_OK;
    }
    else {
        pin_config_t relay_0 = {.port = relay->port, .pin = relay->pin, .logic = relay->relay_state, .direction = dirc_OUTPUT };
        ret = gpio_pin_write_logic (&relay_0,HIGH);    
    }
    return ret;    
}

/**
 * #brief write low on relay pin
 * @param relay pointer to the relay configuration
 * @return Std_ReturnType :
 *            E_ok     : the function done successfully
 *            E_NOT_OK : the function has issue           
 */
Std_ReturnType relay_off (const relay_t *relay){
    Std_ReturnType ret = E_OK;
    if (NULL == relay){
        ret = E_NOT_OK;
    }
    else {
        pin_config_t relay_0 = {.port = relay->port, .pin = relay->pin, .logic = relay->relay_state, .direction = dirc_OUTPUT };
        ret = gpio_pin_write_logic (&relay_0,LOW);         
    }
    return ret;     
}