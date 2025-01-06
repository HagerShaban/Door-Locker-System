/*
 * main.c
 * Description: Source file for Human Interface MCU
 * 				  This unit connected to ( Keypad / LCD )
 * 				  It is responsible of taking the actions and decisions from the user
 */

#include <avr/io.h>
#include <util/delay.h>
#include "main.h"
#include "uart.h"
#include "lcd.h"
#include "keypad.h"
#include "timer.h"



/* Global array to store the password inputed from the user */
uint8 g_inputPassword[PASSWORD_LENGTH];

/* Global Variable to store the status of the Password after comparing */
uint8 g_matchStatus = PASS_MIS_MATCHED;

/* Global Variable to keep track of the seconds counted by the timer */
uint8 g_tick = 0;

/* Global Variable to keep track of how many times the user has inputed the password incorrectly */
uint8 g_passwordMistakes = 0;

/* Global Variable to keep track of the command sent from the CONTROL MCU through UART */
uint8 g_command;


int main(void)
{

	/* Variable to store the pressed key */
	uint8 key_option;
	/* Enable Global Interrupts */
	SREG  |= ( 1 << 7 );

	/* Initialize the UART with Configuration */
	UART_ConfigType UART_Config = {9600,EIGHT_BITS, ONE_STOP_BIT,DISABLED};
	UART_init(&UART_Config);
	/* Initialize LCD */
	LCD_init();
	LCD_moveCursor(0, 4);
	LCD_displayString("Welcome");
	LCD_moveCursor(1, 0);
	_delay_ms(STAND_PRESENTATION_TIME);
	LCD_displayString("Use (=) as Enter");
	_delay_ms(STAND_PRESENTATION_TIME);
	LCD_clearScreen();

	/* Set the Password for the first time */
	HMI_newPassword();

	while(1)
	{

		/* Display the main options to the screen to make the user decide */
		HMI_mainOptions();

		/* Store which key has been pressed for later use */
		key_option = KEYPAD_getPressedKey();

		/* Depending on the pressed key, Perform some operation */
		switch (key_option)
		{

		case OPEN_DOOR:

			/* Ask the user to input a password */
			HMI_promptPassword();
			/* Ask CONTROL MCU to check the Password */
			HMI_sendCommand(SEND_CHECK_PASSWORD);
			/* Send the inputed password to the CONTROL MCU */
			HMI_sendPassword(g_inputPassword);
			/* Inform CONTROL MCU what the user has chosen */
			HMI_sendCommand(OPEN_DOOR);

			/* Receive the order command from CONTROL MCU */
			g_matchStatus = HMI_receiveCommand();

			/* In case the two passwords matches */
			if(g_matchStatus ==  OPENING_DOOR)
			{
				/* Begin unLocking and Locking the Door */
				HMI_openingDoor();
			}
			/* In case the two passwords did not match */
			else if(g_matchStatus == WRONG_PASSWORD)
			{
				/* Begin wrong operation protocol */
				HMI_wrongPassword();
			}

			break; /* End of open door case */

		case CHANGE_PASSWORD:

			/* Ask the user to input a password */
			HMI_promptPassword();
			/* Ask CONTROL MCU to check the Password */
			HMI_sendCommand(SEND_CHECK_PASSWORD);
			/* Send the inputed password to the CONTROL MCU */
			HMI_sendPassword(g_inputPassword);
			/* Inform CONTROL MCU what the user has chosen */
			HMI_sendCommand(CHANGE_PASSWORD);

			/* Receive the order command from CONTROL MCU */
			g_matchStatus = HMI_receiveCommand();

			/* In case the two passwords matches */
			if(g_matchStatus ==  CHANGING_PASSWORD)
			{
				/* Set new password for MCU */
				HMI_newPassword();
			}
			/* In case the two passwords did not match */
			else if(g_matchStatus == WRONG_PASSWORD)
			{
				/* Begin wrong operation protocol */
				HMI_wrongPassword();
			}

			break; /* End of change password case */

		}
	}
}

void HMI_TimerCallBackProcessing(void)
{
	g_tick++;
}



void HMI_startTimer(void)
{
	/* Setup Timer Configuration */
	TIMER_ConfigType TIMER_Config = { TIMER1,CTC_Mode, 0, F_CPU_1024, 7813 };

	/* Initialize the Timer */
	Timer_init(&TIMER_Config);

	/* Set Call Back function for the timer */
	Timer_setCallBack(HMI_TimerCallBackProcessing, TIMER1);
}



void HMI_sendCommand(uint8 g_command)
{
	/* Inform CONTROL MCU that you are to send */
	UART_sendByte(READY_TO_SEND);

	/* Wait until CONTROL MCU are ready to receive */
	while(UART_recieveByte() != READY_TO_RECEIVE);

	/* Send the required command to the CONTROL MCU */
	UART_sendByte(g_command);

	/* Wait until the CONTROL MCU receive the command */
	while(UART_recieveByte() != RECEIVE_DONE);
}



uint8 HMI_receiveCommand(void)
{
	/* Wait until the CONTROL MCU is ready to send */
	while(UART_recieveByte() != READY_TO_SEND);

	/* Inform the CONTROL MCU that you are ready to receive */
	UART_sendByte(READY_TO_RECEIVE);

	/* Receive the command from the CONTROL MCU */
	g_command = UART_recieveByte();

	/* Inform the CONTROL MCU that the receive has been done successfully */
	UART_sendByte(RECEIVE_DONE);

	return g_command;
}



void HMI_newPassword(void)
{
	/* Set its status at first as mis-matched */
	g_matchStatus = PASS_MIS_MATCHED;

	/* Loop until the HMI MCU get the same password */
	while(g_matchStatus == PASS_MIS_MATCHED)
	{
		LCD_clearScreen(); /* Clear Screen */
		LCD_displayString("  New Password  "); /* Inform the user that he will input new password */
		_delay_ms(STAND_PRESENTATION_TIME); /* Hold for Presentation Time */

		LCD_clearScreen(); /* Clear Screen */
		LCD_displayString("Enter Password"); /* Prompt the user to input the password for the first time */
		LCD_moveCursor(1,0); /* Move Cursor to the second line */
		HMI_getPassword(g_inputPassword); /* Get the password from the user */

		HMI_sendCommand(SEND_FIRST_PASSWORD); /* Inform the CONTROL MCU that you will send the first password */
		HMI_sendPassword(g_inputPassword); /* Send the password to the CONTROL MCU */


		LCD_clearScreen(); /* Clear Screen */
		LCD_displayString("ReEnter Password"); /* Prompt the user to input the password for the second time */
		LCD_moveCursor(1,0); /* Move Cursor to the second line */
		HMI_getPassword(g_inputPassword); /* Get the password from the user */

		HMI_sendCommand(SEND_SECOND_PASSWORD); /* Inform the CONTROL MCU that you will send the second password */
		HMI_sendPassword(g_inputPassword); /* Send the password to the CONTROL MCU */

		/* Wait until the is able to send the confirmation of the second password */
		g_matchStatus = HMI_receiveCommand();

		/* In case the Two Passwords did not match */
		if (g_matchStatus == PASS_MIS_MATCHED)
		{
			LCD_clearScreen(); /* Clear Screen */
			LCD_displayString("MISMATCHED Pass"); /* Display an Error Message */
			_delay_ms(STAND_PRESENTATION_TIME); /* Hold for Presentation Time */
		}
	}
}



void HMI_sendPassword(uint8 a_inputPassword[])
{
	uint8 counter;

	/* Loop on the passwords elements */
	for( counter = 0; counter < PASSWORD_LENGTH; counter++)
	{
		UART_sendByte(a_inputPassword[counter]); /* Send Password element by element to the CONTROL MCU */
		_delay_ms(SEND_RECEIVE_TIME);      /* Delay for the time gap for sending receiving time between the MCUs */
	}
}



void HMI_getPassword(uint8 a_inputPassword[])
{
	LCD_moveCursor(1, 0);

	uint8 counter = 0;
	uint8 password_key = 0; /* Variable to store the pressed key */

	/* Stop getting number after you get 5 characters */
	while( counter != PASSWORD_LENGTH )
	{
		password_key = KEYPAD_getPressedKey(); /* Get the get the key pressed and store it in the password array */

		if ( (password_key >= 0) && (password_key <= 9) )
		{
			LCD_displayCharacter('*'); /* Display asterisk for privacy */
			a_inputPassword[counter] = password_key;
			counter++;
		}
		_delay_ms(KEYPAD_CLICK_TIME); /* Delay time for keypad press */
	} /* End while loop */

	/* Don't leave until the user press (=) symbol */
	while ( KEYPAD_getPressedKey() != '=' );
}



void HMI_mainOptions(void)
{
	LCD_clearScreen(); /* Clear Screen */
	LCD_displayString("(+): Open Door"); /* Display the first option */
	LCD_moveCursor(1,0); /* Move to the next line */
	LCD_displayString("(-): Change Pass"); /* Display the second option */
}

void HMI_promptPassword(void)
{
	LCD_clearScreen(); /* Clear Screen */
	LCD_displayString("Enter Password :"); /* Prompt the user to write the password */
	HMI_getPassword(g_inputPassword); /* Takes the password and store it in an array */
}


void HMI_openingDoor(void)
{
	HMI_startTimer(); /* Start the timer to measure time period */

	/* Open the door for ( 15 sec ) */
	LCD_clearScreen(); /* Clear Screen */
	LCD_displayString("Door is Opening"); /* Display explanation message on LCD */
	while(g_tick < OPEN_DOOR_TIME); /* Count up to 15 */
    g_tick = 0; /* Reset counter to reuse it */

	/* Hold the door for ( 3 sec ) */
	LCD_clearScreen(); /* Clear Screen */
	LCD_displayString("Door is on Hold"); /* Display explanation message on LCD */
	while(g_tick < HOLD_DOOR_TIME); /* Count up to 3 */
    g_tick = 0; /* Reset counter to reuse it */

	/* Open the door for ( 15 sec ) */
	LCD_clearScreen(); /* Clear Screen */
	LCD_displayString("Door is Closing"); /* Display explanation message on LCD */
	while(g_tick < CLOSE_DOOR_TIME); /* Count up to 15 */
    g_tick = 0; /* Reset counter to reuse it */

    Timer_DeInit(TIMER1); /* Stop the timer */
    LCD_clearScreen(); /* Clear Screen */
}




void HMI_wrongPassword(void)
{
	g_passwordMistakes++; /* Increment the wrong counter */

	LCD_clearScreen(); /* Clear Screen */
	LCD_displayString(" Wrong Password "); /* Display explanation message on LCD */
	_delay_ms(STAND_PRESENTATION_TIME); /* Hold for Presentation Time */

	/* If the user entered the password 3 times wrong */
	if(g_passwordMistakes == MAX_NUM_OF_MISTAKES)
	{
		HMI_startTimer(); /* Start the timer to measure time period */

		LCD_clearScreen(); /* Clear Screen */
		LCD_displayString(" WARNING "); /* Display warning message on LCD */

		while(g_tick != WARNING_TIME); /* Display the message for one minute */

		/* Reset the counters */
		g_passwordMistakes = 0;
		g_tick = 0;

	    Timer_DeInit(TIMER1); /* Stop the timer */
	}

    LCD_clearScreen(); /* Clear Screen */
}
