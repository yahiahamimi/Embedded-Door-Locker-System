/*
 * timer0_pwm.c
 *
 *  Created on: Feb 24, 2024
 *      Author: Yahia Hamimi
 */
#include "timer0_pwm.h"
#include "gpio.h"


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void PWM_Timer0_Start(uint8 duty_cycle){
	TCNT0=0; //set initial value to 0
	OCR0=duty_cycle; //set compare value to the chosen duty cycle;

	/*Setup the direction for OC0 as output pin through the GPIO driver*/
	GPIO_setupPinDirection(PWM_OUTPUT_PORT_ID,PWM_OUTPUT_PIN_ID,PIN_OUTPUT);

	/* For frequency 500Hz and F_cpu=1MHz we choose pre_scaler N=8*/

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0|=(1<<WGM00)|(1<<WGM01)|(1<<COM01)|(1<<CS01);
}
