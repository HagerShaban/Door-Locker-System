 /*
 *
 * dc_motor.h
 * Description: header file for DC Motor
 *
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

#define DcMotor_PORT_ID 	PORTB_ID
#define DcMotor_PIN1 		PIN0_ID
#define DcMotor_PIN2 		PIN1_ID


typedef enum
{
	OFF, CW, A_CW
}DcMotor_State;

/*
 * Description :
 * Function responsible for setup the direction for the two motor pins through the GPIO driver.
 * Stop at the DC-Motor at the beginning through the GPIO driver.
 */
void DcMotor_Init(void);

/*
 * Description :
 * The function responsible for rotate the DC Motor CW/ or A-CW or stop the motor based on the state input state value.
 * Send the required duty cycle to the PWM driver based on the required speed value.
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif
