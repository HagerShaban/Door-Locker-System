/*
 * buzzer.h
 * Description:source file for buzzer sensor
 */

#ifndef BUZZER_H_
#define BUZZER_H_
#include"std_types.h"

/*making the buzzer configurable*/
#define BUZZER_PORT_ID				PORTA_ID
#define BUZZER_PIN_ID				PIN0_ID


/*
 * Description :
 * Function responsible for initializing the buzzer
 */
void Buzzer_Init(void);
/*
 * Description :
 * Function responsible for turning on the buzzer
 */
void Buzzer_On(void);
/*
 * Description :
 * Function responsible for turning off the buzzer
 */
void Buzzer_Off(void);

#endif /* BUZZER_H_ */
