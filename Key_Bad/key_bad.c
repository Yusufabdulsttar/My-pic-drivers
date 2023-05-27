/* 
 * File:   key_bad.c
 * Author: yusuf
 *
 * Created on April 24, 2023, 1:11 PM
 */

#include "key_bad.h"
static const uint8 keypad_value [KEYPAD_ROW][KEYPAD_COL] = {
                                              {'7','8','9','/'},
                                              {'4','5','6','*'},
                                              {'1','2','3','-'},
                                              {'#','0','=','+'}
};

Std_ReturnType keypad_initialize (const keypad_t *keypad){
    Std_ReturnType ret = E_OK;
    if (NULL == keypad){
        ret = E_NOT_OK;
    }
    else {
        for (uint8 r = 0; r < KEYPAD_ROW; r++){
            ret = gpio_pin_config_intialize (&(keypad->keypad_row[r]));
        }
        for (uint8 c = 0; c < KEYPAD_COL; c++){
            ret = gpio_pin_direction_intialize (&(keypad->keypad_col[c]));
        }
    }
    return ret;   
} 



Std_ReturnType keypad_read (const keypad_t *keypad , uint8 *key){
    Std_ReturnType ret = E_OK;
    uint8 key_c = 0 , r = 0 ,c = 0;
    if (NULL == keypad || NULL == key){
        ret = E_NOT_OK;
    }
    else {
        for (r = 0; r < KEYPAD_ROW; r++){
            for (uint8 e = 0; e < KEYPAD_ROW; e++){
                ret = gpio_pin_write_logic (&(keypad->keypad_row[e]),(LOW));
            }
            gpio_pin_write_logic (&(keypad->keypad_row[r]),(HIGH));
            for (c = 0; c <= KEYPAD_COL; c++){
                ret = gpio_pin_read_logic (&(keypad->keypad_col[c]),&key_c);
                if (key_c == HIGH){
                    *key =  keypad_value[r][c]; 
                }
            }
        }
          
    }
    return ret;       
}
 