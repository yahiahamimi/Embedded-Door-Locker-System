/*
 * timer0_pwm.h
 *
 *  Created on: Feb 24, 2024
 *      Author: Yahia Hamimi
 */

#ifndef TIMER0_PWM_H_
#define TIMER0_PWM_H_

#include "avr/io.h"
#include "std_types.h"

/*******************************************************************************
 *                      Definitions                                            *
 *******************************************************************************/
#define PWM_OUTPUT_PORT_ID PORTB_ID
#define PWM_OUTPUT_PIN_ID PIN3_ID

/*******************************************************************************
 *                      Functions Decelerations                                  *
 *******************************************************************************/
void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* TIMER0_PWM_H_ */
