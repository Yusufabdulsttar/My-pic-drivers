/* 
 * File:   key_bad.h
 * Author: yusuf
 *
 * Created on April 24, 2023, 1:11 PM
 */

#ifndef KEY_BAD_H
#define	KEY_BAD_H

/* section : Includes   */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/* section : Micro Declaration  */
#define KEYPAD_ROW 4
#define KEYPAD_COL 4

/* section : Micro Function Declaration  */

/* section : Data Type Declaration  */
typedef struct {
    pin_config_t keypad_row [KEYPAD_ROW];
    pin_config_t keypad_col [KEYPAD_COL];
}keypad_t;

/* section : Function Declaration  */
Std_ReturnType keypad_initialize (const keypad_t *keypad); 
Std_ReturnType keypad_read (const keypad_t *keypad , uint8 *key);

#endif	/* KEY_BAD_H */

