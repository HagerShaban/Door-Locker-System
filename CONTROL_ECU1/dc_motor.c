 /*
 *
 *  dc_motor.c
 * Description: Source file for DC Motor
 *
 */

#include "dc_motor.h"
#include "gpio.h"

void DcMotor_Init(void)
{

	 //Configure the two pins of the DC Motor as an output pins
	GPIO_setupPinDirection(DcMotor_PORT_ID, DcMotor_PIN1, PIN_OUTPUT);
	GPIO_setupPinDirection(DcMotor_PORT_ID, DcMotor_PIN2, PIN_OUTPUT);


	 //Stop the DC Motor at the beginning
	GPIO_writePin(DcMotor_PORT_ID, DcMotor_PIN1, LOGIC_LOW);
	GPIO_writePin(DcMotor_PORT_ID, DcMotor_PIN2, LOGIC_LOW);
}


void DcMotor_Rotate(DcMotor_State state, uint8 speed)
{


	 //Check the state and rotate the DC Motor
	if( state == CW )
	{
		GPIO_writePin(DcMotor_PORT_ID, DcMotor_PIN1, LOGIC_LOW);
		GPIO_writePin(DcMotor_PORT_ID, DcMotor_PIN2, LOGIC_HIGH);
	}
	else if ( state == A_CW )
	{
		GPIO_writePin(DcMotor_PORT_ID, DcMotor_PIN1, LOGIC_HIGH);
		GPIO_writePin(DcMotor_PORT_ID, DcMotor_PIN2, LOGIC_LOW);
	}
	else if ( state == OFF )
	{
		GPIO_writePin(DcMotor_PORT_ID, DcMotor_PIN1, LOGIC_LOW);
		GPIO_writePin(DcMotor_PORT_ID, DcMotor_PIN2, LOGIC_LOW);
	}
	else
		{
			/* Invalid Input State - Do Nothing */
		}

}
