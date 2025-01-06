/*
 * keypad.c
 * Description: Source of the keypad module
 */
#include"gpio.h"
#include"keypad.h"
#include"common_macros.h"

#if(NUM_OF_COLS == 4)
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8);
#elif(NUM_OF_COLS ==3)
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8);
#endif

uint8 KEYPAD_getPressedKey(void){
	uint8 row,col,keypad_port_value=0; // local variables for the keypad
	while(1){
		/* we need to output logic(high or low) to a specific col and loop for the rows
		 * */
		for(col=0;col<NUM_OF_COLS;col++){
			GPIO_setupPortDirection(KEYPAD_PORT_ID,PORT_INPUT);
			GPIO_setupPinDirection(KEYPAD_PORT_ID,KEYPAD_FIRST_COL_PIN_ID+col,PIN_OUTPUT);
#if(KEYPAD_BUTTON_PRESSED == LOGIC_LOW)
			keypad_port_value=~(1<<(KEYPAD_FIRST_COL_PIN_ID+col));
#else
			keypad_port_value=(1<<(KEYPAD_FIRST_COL_PIN_ID+col));
#endif
			GPIO_writePort(KEYPAD_PORT_ID,keypad_port_value);
			for(row=0;row<NUM_OF_ROWS;row++){
				if(GPIO_readPin(KEYPAD_PORT_ID,KEYPAD_FIRST_ROW_PIN_ID+row) == KEYPAD_BUTTON_PRESSED){
#if(NUM_OF_COLS == 4)
					return KEYPAD_4x4_adjustKeyNumber((row*NUM_OF_COLS)+col+1);
#elif(NUM_OF_COLS ==3)
					return KEYPAD_4x3_adjustKeyNumber((row*NUM_OF_COLS)+col+1);
#endif
				}

			}

		}
	}
}


#if(NUM_OF_COLS == 4)
/*this function is called by the modules itself so the application layer doesn't have the right to call it
 * so, we must make the function static..
 * */
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number){
	uint8 keypad_button=0;
	switch (button_number){
	case 1:
		keypad_button=7;
		break;
	case 2:
		keypad_button=8;
		break;
	case 3:
		keypad_button=9;
		break;
	case 4:
		keypad_button='/';
		break;
	case 5:
		keypad_button=4;
		break;
	case 6:
		keypad_button=5;
		break;
	case 7:
		keypad_button=6;
		break;
	case 8:
		keypad_button='*';
		break;
	case 9:
		keypad_button=1;
		break;
	case 10:
		keypad_button=2;
		break;
	case 11:
		keypad_button=3;
		break;
	case 12:
		keypad_button='-';
		break;
	case 13:
		keypad_button=13;
		break;
	case 14:
		keypad_button=0;
		break;
	case 15:
		keypad_button='=';
		break;
	case 16:
		keypad_button='+';
		break;
	default:
		keypad_button = button_number;
		break;
	}
	return keypad_button;
}
#elif(NUM_OF_COLS ==3)
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number){
	uint8 keypad_button=0;
	switch (button_number){
	case 10:
		keypad_button='*';
		break;
	case 11:
		keypad_button=0;
		break;
	case 12:
		keypad_button='#';
		break;
	default:
		keypad_button = button_number;
		break;
	}
	return keypad_button;
}
#endif
