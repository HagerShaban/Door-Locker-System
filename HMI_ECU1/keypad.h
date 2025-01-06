/*
 * keypad.h
 * Description: Header of the keypad module
 */
#ifndef KEYPAD_H_
#define KEYPAD_H_
#include"std_types.h"


#define NUM_OF_ROWS 4
#define NUM_OF_COLS 4
#define KEYPAD_PORT_ID PORTC_ID
#define KEYPAD_FIRST_ROW_PIN_ID PIN0_ID /*first pin connected to the first row*/
#define KEYPAD_FIRST_COL_PIN_ID PIN4_ID /*first pin connected to the first col*/
#define KEYPAD_BUTTON_PRESSED LOGIC_LOW


uint8 KEYPAD_getPressedKey(void);
#endif /* KEYPAD_H_ */
