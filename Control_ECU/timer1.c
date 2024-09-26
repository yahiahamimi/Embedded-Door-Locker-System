/*
 * timer1.c
 *
 *  Created on: Mar 16, 2024
 *      Author: Yahia Hamimi
 */

#include "timer1.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include <avr/io.h>
#include <avr/interrupt.h>

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(TIMER1_OVF_vect)
{
	if (g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPA_vect)
{
	if (g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize Timer 1:
 * 1. Setup Timer 1 registers bits.
 * 2. Setup the operation mode (normal or compare).
 * 3. Setup the required prescaler.
 * 4. Set the initial and compare value.
 */
void Timer1_init(const Timer1_ConfigType *Config_Ptr)
{

	/************************** TCCR1A Description **************************
	 * 1. for normal port operation Disconnect OC1A and OC1B
	 *    So, COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
	 * 2. this driver support Normal & Compare mode not a PWM mode
	 *    then FOC1A & FOC1B is always high  so, FOC1A=1 FOC1B=1
	 * 3. For CTC OR Normal Mode WGM10=0 WGM11=0
	 ************************************************************************/
	TCCR1A = (1 << FOC1A) | (1 << FOC1B);

	/************************** TCCR1B Description **************************
	 * 1. input capture unit is disabled
	 *    So,don't care with ICNC1,ICS1 (initial value = 0)
	 * 2. WGM12,WGM13 Configurable by the user to control the mode
	 * 3. CS [12:10] Configurable by the user to control
	 *    the clk with the required prescaler or disable it
	 ************************************************************************/

	TCCR1B = (TCCR1B & 0xE0) | (Config_Ptr->prescaler) | ((Config_Ptr->mode) << WGM12);

	/* Enable Timer1 Overflow Interrupts */
	TIMSK = (TIMSK & 0xC3) | (1 << TOIE1);

	/* Set the initial value */
	TCNT1 = (Config_Ptr->initial_value);
	/* Set the compare value */
	if (Config_Ptr->mode == Compare_mode)
	{
		TIMSK |= (1 << OCIE1A);	/* Enable Timer1 Compare A Interrupts */
		OCR1A = (Config_Ptr->compare_value);
	}
}
/*
 * Description:
 * Function to disable the Timer1
 */
void Timer1_deInit(void)
{
	/* Clear All Timer1 Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	OCR1A = 0;
	/* Disable the Timer1 interrupt */
	CLEAR_BIT(TIMSK, TICIE1);

	/* Reset the global pointer value */
	g_callBackPtr = NULL_PTR;
}
/*
 * Description:
 * Function to set the Call Back function address.
 */
void Timer1_setCallBack(void (*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}
