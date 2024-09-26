/******************************************************************************
 *
 * Module: DCMOTOR
 *
 * File Name: dcmotor.h
 *
 * Description: header file for the ATmega32 DC MOTOR driver
 *
 * Author: Yahia Hamimi
 *
 *******************************************************************************/

#ifndef DCMOTOR_H_
#define DCMOTOR_H_
#include"std_types.h"

/*******************************************************************************
 *                              Typedefs & Definitions                                  *
 *******************************************************************************/
#define DC_MOTOR_IN1_PORT_ID PORTB_ID
#define DC_MOTOR_IN1_PIN_ID PIN0_ID

#define DC_MOTOR_IN2_PORT_ID PORTB_ID
#define DC_MOTOR_IN2_PIN_ID PIN1_ID


typedef enum{
Stop,
CW,
A_CW
}DcMotor_State;

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/
extern uint8 g_dutyCycle;


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void DcMotor_Init(void);
void DcMotor_Rotate(DcMotor_State state,uint8 speed);


#endif /* DCMOTOR_H_ */
