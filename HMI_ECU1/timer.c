 /*
 * timer.c
 * Description: Source file for timer
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include"common_macros.h"
#include "timer.h"

/*global variable for the call back function*/
static volatile void (*g_Timer0_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer1_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer2_callBackPtr)(void) = NULL_PTR;


/*TIMER0_ISR in case of compare and overflow modes*/

ISR(TIMER0_OVF_vect)
{
	if(g_Timer0_callBackPtr != NULL_PTR)
	{
		(*g_Timer0_callBackPtr)();
	}
}


ISR(TIMER0_COMP_vect)
{
	if(g_Timer0_callBackPtr != NULL_PTR)
	{
		(*g_Timer0_callBackPtr)();
	}
}

/*TIMER1_ISR in case of compare and overflow modes*/


ISR(TIMER1_OVF_vect)
{
	if(g_Timer1_callBackPtr != NULL_PTR)
	{
		(*g_Timer1_callBackPtr)();
	}
}


ISR(TIMER1_COMPA_vect)
{
	if(g_Timer1_callBackPtr != NULL_PTR)
	{
		(*g_Timer1_callBackPtr)();
	}
}

/*TIMER2_ISR in case of compare and overflow modes*/

ISR(TIMER2_OVF_vect)
{
	if(g_Timer2_callBackPtr != NULL_PTR)
	{
		(*g_Timer2_callBackPtr)();
	}
}

ISR(TIMER2_COMP_vect)
{
	if(g_Timer2_callBackPtr != NULL_PTR)
	{
		(*g_Timer2_callBackPtr)();
	}
}


/*
 * Description :
 * Function responsible for initializing the timer with some configurations
 */
void Timer_init(const TIMER_ConfigType * Config_Ptr)
{
	//switch case to choose which timer we will use

	switch(Config_Ptr->timer_ID)
	{
	case TIMER0:

	    //set Bit FOC0 in TCCR0 register
		TCCR0 |= ( 1 << FOC0 );

		/*
		 * Configure Timer wave generation in the selected mode
		 * Insert it depending on the configuration
		 */
		TCCR0 = ( TCCR0 & 0xF7 ) | ( ( Config_Ptr->timer_mode ) << 3 );

		//adjust the prescaler in TCCR0 register
		TCCR0 = ( TCCR0 & 0xF8 ) | ( Config_Ptr->timer_clock );

		//enable interrupt mode
		TIMSK |= (1<<(Config_Ptr->timer_mode));

		//initial value for the timer
		TCNT0 = (Config_Ptr->timer_intial_value);

		//enable required interrupt of modes(ovf,ctc) to execute ISR
		if ( Config_Ptr->timer_mode == CTC_Mode )
		{
			OCR0 = Config_Ptr->timer_compare_value;
			//enable compare mode flag
			TIMSK |= (1<<OCIE0);
		}
		else if ( Config_Ptr->timer_mode == CTC_Mode )
		{
			//enable overflow mode flag
			TIMSK |= (1<<TOIE0);
		}

		break; /* End Timer0 */

	case TIMER1:
		//adjust the prescaler in TCCR1A register
		TCCR1A = (1<<FOC1A) | (1<<FOC1B);

		TCCR1B = Config_Ptr->timer_clock;

		/*
		 * Configure Timer wave generation in the selected mode
		 * Insert it depending on the configuration
		 */
		TCCR1B = (TCCR1B & 0xF7) | ((Config_Ptr->timer_mode)<<WGM12);

		//Insert required initial value in TCNT1 register
		TCNT1 = Config_Ptr->timer_intial_value;

		//enable required interrupt of modes(ovf,ctc) to execute ISR
		if ( Config_Ptr->timer_mode == CTC_Mode )
		{
			//insert required compare value in OCR1A register
			OCR1A = Config_Ptr->timer_compare_value;
			//enable compare mode flag
			TIMSK |= (1<<OCIE1A);
		}
		else if ( Config_Ptr->timer_mode == CTC_Mode )
		{
			//enable overflow mode flag
			TIMSK |= (1<<TOIE1);
		}

		break; /* End Timer1 */


	case TIMER2:

	    //set Bit FOC2 in TCCR0 register
		TCCR0 |= ( 1 << FOC2 );
		/*
				 * Configure Timer wave generation in the selected mode
				 * Insert it depending on the configuration
				 */
		TCCR2 = ( TCCR2 & 0xF7 ) | ( ( Config_Ptr->timer_mode ) << 3 );


		TCCR2 = ( TCCR2 & 0xF8 ) | ( Config_Ptr->timer_clock );

		/*
		 * Enable the Required Intterupt mode
		 * Depending on configuration
		 */
		TIMSK |= (1<<(Config_Ptr->timer_mode));

		/* Insert the required initial value in TCNT0 register */
		TCNT2 = (Config_Ptr->timer_intial_value);

		//enable required interrupt of modes(ovf,ctc) to execute ISR
		if ( Config_Ptr->timer_mode == CTC_Mode )
		{
			//insert required compare value in OCR0 register
			OCR0 = Config_Ptr->timer_compare_value;
			//enable compare mode flag
			TIMSK |= (1<<OCIE2);
		}
		else if ( Config_Ptr->timer_mode == CTC_Mode )
		{
			//enable overflow mode flag
			TIMSK |= (1<<TOIE2);
		}

		break; /* End Timer2 */
	}
}





/*
 * Description :
 * Function to set the Call Back Function Address
 */
void Timer_setCallBack(void(*a_ptr)(void), TIMER_ID timer_number)
{
	switch(timer_number)
	{
	case TIMER0:
		/* Save the address of the Call back function in a global variable */
		g_Timer0_callBackPtr = a_ptr;
		break;

	case TIMER1:
		/* Save the address of the Call back function in a global variable */
		g_Timer1_callBackPtr = a_ptr;
		break;

	case TIMER2:
		/* Save the address of the Call back function in a global variable */
		g_Timer2_callBackPtr = a_ptr;
		break;
	}
}



/*
 * Description :
 * Function to stop the clock and DeInit the whole Timer
 */
void Timer_DeInit(TIMER_ID timer_number)
{
	switch(timer_number)
	{
	case TIMER0:
		/* put zero in all timer0 registers  */
		TCCR0 &= 0x00;
		TCNT0 &= 0x00;
		OCR0 &= 0x00;
		 /*disable the interrupt module of timer0*/
	    TIMSK &=~(1<<TOIE0) &~(1<<OCIE0);
		break;

	case TIMER1:
		/* put zero in all timer1 registers  */
		TCCR1A &= 0x00;
		TCCR1B &= 0x00;
		TCNT1 &= 0x0000;
		OCR1A &= 0x0000;
	    /*disable the interrupt module of timer1*/
	    TIMSK &=~(1<<TOIE1) &~(1<<OCIE1A);
		break;

	case TIMER2:
		/* put zero in all timer2 registers  */
		TCCR2 &= 0x00;
		TCNT2 &= 0x00;
		OCR2 &= 0x00;
		/*disable the interrupt module of timer2*/
	    TIMSK &=~(1<<TOIE2) &~(1<<OCIE2);
	    break;
	}
}
