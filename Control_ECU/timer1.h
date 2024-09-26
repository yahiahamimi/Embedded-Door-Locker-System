/*
 * timer1.h
 *
 *  Created on: Mar 16, 2024
 *      Author: yahia
 */

#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"

typedef enum{
	No_clk,
	F_cpu,
	F_cpu_8,
	F_cpu_64,
	F_cpu_256,
	F_cpu_1024,
	Ext_clk_falling,
	Ext_clk_raising
}Timer1_Prescaler;

typedef enum{
	Normal_mode,
	Compare_mode
}Timer1_Mode;


typedef struct {
uint16 initial_value;
uint16 compare_value; // it will be used in compare mode only.
Timer1_Prescaler prescaler;
Timer1_Mode mode;
}Timer1_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initialize the Timer driver.
 */

void Timer1_Init(const Timer1_ConfigType * Config_Ptr);

/*
 * Description: Function to disable the Timer1r
 */
void Timer1_deInit(void);

/*
 * Description: Function to set the Call Back function address.
 */
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER1_H_ */
