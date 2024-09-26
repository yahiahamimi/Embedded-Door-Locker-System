/*
 * dcmotor.c
 *
 *  Created on: Feb 24, 2024
 *      Author: Yahia Hamimi
 */

#include "avr/io.h" /* To use the ADC Registers */
#include "dcmotor.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "gpio.h"

/*******************************************************************************
 *                          Global Variables                                   *
 *******************************************************************************/
uint8 g_dutyCycle;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void DcMotor_Init(void){
	GPIO_setupPinDirection(DC_MOTOR_IN1_PORT_ID,DC_MOTOR_IN1_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_IN2_PORT_ID,DC_MOTOR_IN2_PIN_ID,PIN_OUTPUT);

	GPIO_writePin(DC_MOTOR_IN1_PORT_ID,DC_MOTOR_IN1_PIN_ID,LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_IN2_PORT_ID,DC_MOTOR_IN2_PIN_ID,LOGIC_LOW);
}
void DcMotor_Rotate(DcMotor_State state,uint8 speed){
	switch(state){
	case Stop:
		GPIO_writePin(DC_MOTOR_IN1_PORT_ID,DC_MOTOR_IN1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_IN2_PORT_ID,DC_MOTOR_IN2_PIN_ID,LOGIC_LOW);
		break;
	case CW:
		GPIO_writePin(DC_MOTOR_IN1_PORT_ID,DC_MOTOR_IN1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_IN2_PORT_ID,DC_MOTOR_IN2_PIN_ID,LOGIC_HIGH);
		break;
	case A_CW:
		GPIO_writePin(DC_MOTOR_IN1_PORT_ID,DC_MOTOR_IN1_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_IN2_PORT_ID,DC_MOTOR_IN2_PIN_ID,LOGIC_LOW);
		break;
	}
	/* calculate the duty cycle based on the speed
	multiply by 255 as it is the maximum value for the timer*/
	g_dutyCycle = ((float)speed / 100)*255;
}

