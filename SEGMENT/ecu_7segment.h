/* 
 * File:   ecu_7segment.h
 * Author: yusuf
 *
 * Created on April 20, 2023, 5:24 PM
 */
#ifndef ECU_7SEGMENT_H
#define	ECU_7SEGMENT_H

/* section : Includes   */
#include "ecu_7segment_cfg.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/* section : Micro Declaration  */

/* section : Micro Function Declaration  */

/* section : Data Type Declaration  */
typedef enum {
    COMMON_ANODE,
    COMMON_CATHODE
}segment_type_t;

typedef struct {
    pin_config_t segment_pin[4];
    segment_type_t segment_type;
}segment_t;
/* section : Function Declaration  */

Std_ReturnType segment_initialize (const segment_t *segment);
Std_ReturnType segment_write_number (const segment_t *segment,uint8 number);




#endif	/* ECU_7SEGMENT_H */

