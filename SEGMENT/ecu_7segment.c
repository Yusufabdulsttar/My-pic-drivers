/* 
 * File:   ecu_7segment.h
 * Author: yusuf
 *
 * Created on April 20, 2023, 5:24 PM
 */

#include"ecu_7segment.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/**
 * @brief initialize the dc motor
 * @param segment pointer to segment configuration
 * @return Std_ReturnType :
 *            E_ok     : the function done successfully
 *            E_NOT_OK : the function has issue   
 */
Std_ReturnType segment_initialize (const segment_t *segment){
    Std_ReturnType ret = E_OK;
    if (NULL == segment){
        ret = E_NOT_OK;
    }
    else {
        ret = gpio_pin_config_intialize (&(segment->segment_pin[0]));
        ret = gpio_pin_config_intialize (&(segment->segment_pin[1]));
        ret = gpio_pin_config_intialize (&(segment->segment_pin[2]));
        ret = gpio_pin_config_intialize (&(segment->segment_pin[3]));
    }
    return ret;     
}

/**
 * @brief initialize the dc motor
 * @param segment pointer to segment configuration
 * @param number the number displayed on the segment
 * @return Std_ReturnType :
 *            E_ok     : the function done successfully
 *            E_NOT_OK : the function has issue   
 */
Std_ReturnType segment_write_number (const segment_t *segment,uint8 number){
    Std_ReturnType ret = E_OK;
    if (NULL == segment || number < 0 || number > 9){
        ret = E_NOT_OK;
    }
    else {
        if (segment->segment_type == COMMON_ANODE){
            ret = gpio_pin_write_logic (&(segment->segment_pin[0]),number & 0x01);
            ret = gpio_pin_write_logic (&(segment->segment_pin[1]),(number >> 1)& 0x01);
            ret = gpio_pin_write_logic (&(segment->segment_pin[2]),(number >> 2)& 0x01);
            ret = gpio_pin_write_logic (&(segment->segment_pin[3]),(number >> 3)& 0x01);
        }
        else if (segment->segment_type == COMMON_CATHODE){
            number = !number;
            ret = gpio_pin_write_logic (&(segment->segment_pin[0]),number & 0x01);
            ret = gpio_pin_write_logic (&(segment->segment_pin[1]),(number >> 1)& 0x01);
            ret = gpio_pin_write_logic (&(segment->segment_pin[2]),(number >> 2)& 0x01);
            ret = gpio_pin_write_logic (&(segment->segment_pin[3]),(number >> 3)& 0x01);           
        }
        
    }
}