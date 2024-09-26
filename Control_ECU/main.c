/*
 * main.c
 *
 *  Created on: Mar 17, 2024
 *      Author: Yahia Hamimi
 */


#include <util/delay.h>
#include <avr/io.h>
#include "timer1.h"
#include "uart.h"
#include "twi.h"
#include "main.h"
#include "timer0_pwm.h"
#include "external_eeprom.h"
#include "buzzer.h"
#include "dcmotor.h"

/*******************************************************************************
 *                          Global Variables & Configurations                  *
 *******************************************************************************/
uint8 g_seconds = 0;
uint8 passwordCompareResult;
uint8 returnToCreatePasswordFlag = 0;
uint8 pass_arr[PASS_LENGTH];
uint8 reconfirmpass_arr[PASS_LENGTH];
uint8 memorycheck_arr[PASS_LENGTH];
UART_ConfigType UART_Configuration_data = {bit_8, even_parity, stop_1_bit, 9600};
TWI_ConfigType TWI_Configuration_data = {0x10, Fast_Mode};
Timer1_ConfigType Timer_Configuration_data = {0, 7812, F_cpu_1024, Compare_mode};

int main(void)
{
	CECU_init();
	while (1)
	{
		UART_sendByte(CONTROL_ECU_READY);
		createPassword();
		while (UART_recieveByte() != HMI_ECU_READY)
		{
		};
		UART_sendByte(CONTROL_ECU_WAITING_FOR_MENU_OPTION);
		mainOptionsMenuActions();
	}
}
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for CECU modules Initialization and activate interrupts
 * 1. initialize UART
 * 2. initialize DC Motor
 * 3. initialize Buzzer
 * 4. initialize TWI
 * 5. initialize Timer 1 and give it the address of the function to be called back
 */
void CECU_init(void)
{
	SREG |= (1 << 7);
	DcMotor_Init();
	Buzzer_init();
	UART_init(&UART_Configuration_data);
	TWI_init(&TWI_Configuration_data);
	Timer1_init(&Timer_Configuration_data);
	Timer1_setCallBack(callBackIncrementSeconds);
}

/*
 * Description :
 * Function responsible for storing password in the EEPROM
 */

void storePasswordInMemory(uint8 u8password[])
{
	uint8 counter = 0;
	for (counter = 0; counter < PASS_LENGTH; counter++)
	{
		EEPROM_writeByte(0x0311 + counter, u8password[counter]); /* Write byte in the external EEPROM */
		_delay_ms(10);
	}
}
/*
 * Description :
 * Function responsible for reading password from the EEPROM
 */
void readPasswordFromMemory(uint8 u8password[])
{
	uint8 counter = 0;
	for (counter = 0; counter < PASS_LENGTH; counter++)
	{
		EEPROM_readByte(0x0311 + counter, &u8password[counter]); /* Write byte in the external EEPROM */
		_delay_ms(10);
	}
}
/*
 * Description :
 * Function responsible for creating password:
 * 1. Receive the entered password from HMI
 * 2. Receive password re-confirmation from HMI
 * 3. Compare the two passwords and store it in memory
 * if it's valid password
 */
void createPassword(void)
{
	while (1)
	{
		recievePass();
		recievePassreconfirm();
		if (Passwordcheck(pass_arr,reconfirmpass_arr))
		{
			storePasswordInMemory(pass_arr);
			UART_sendByte(CONTROL_ECU_GOT_VALID_PASSWORD);
			break;
		}
		else
		UART_sendByte(PASSWORD_NOT_MATCHED);  //uart must recieve a value to break from the loop
	}
}

/*
 * Description :
 * Function responsible for Receive the entered password from HMI
 * and saving it to the global password array
 */
void recievePass(void)
{
uint8 loop_counter;
	for (loop_counter = 0; loop_counter < PASS_LENGTH; loop_counter++)
	{
		pass_arr[loop_counter] = UART_recieveByte();
	}
}

/*
 * Description :
 * Function responsible for Receive the entered password from HMI
 * and saving it to the global re-confirmation password array
 */
void recievePassreconfirm(void)
{
	uint8 loop_counter;
	for (loop_counter = 0; loop_counter < PASS_LENGTH; loop_counter++)
	{
		reconfirmpass_arr[loop_counter] = UART_recieveByte();
	}
}

/*
 * Description :
 * Function responsible for comparing two passwords and
 * return the boolean results
 */

boolean Passwordcheck(uint8 First_arr[],uint8 Second_arr[])
{
	uint8 loop_counter;
	for (loop_counter = 0; loop_counter < PASS_LENGTH; loop_counter++)
	{
		if (First_arr[loop_counter] != Second_arr[loop_counter])
			return FALSE;
	}
	return TRUE;
}


/*
 * Description :
 * Function looping around main menu and doing the received action from HMI
 * 1. Receive the chosen option from HMI
 * 2. Receive the entered password and compare it with the saved one in memory
 * 3. Based on the compare results do certain actions
 */

void mainOptionsMenuActions(void)
{
while(1){
	uint8 option;
	while(1){
		uint8 key = UART_recieveByte();
		if (key == '+' || key == '-')
		{
         option=key;
			UART_sendByte(CONTROL_ECU_GOT_MENU_OPTION);
			break;
		}
	}

		switch (option)
		{
		case '+':
		{
			passwordMemoryCompare();
			UART_sendByte(passwordCompareResult);
			switch (passwordCompareResult)
			{

			case WRONG_PASSWORD:
			{
				uint8 counter=0;
				do
							{
								passwordMemoryCompare();
								counter++;
								UART_sendByte(passwordCompareResult);

							} while (passwordCompareResult == WRONG_PASSWORD && counter < 2);
							if (counter == 2 && passwordCompareResult == WRONG_PASSWORD)
							{
								UART_sendByte(SECURITY_BREACH);
								securityAlarm();
								UART_sendByte(ALARM_TIMEOUT);
								break;
							}
							else
								UART_sendByte(NOT_A_WRONG_PASSWORD);
			}

			case RIGHT_PASSWORD:
			{
				while (UART_recieveByte() != RIGHT_PASSWORD);
				g_seconds = 0;
				while (g_seconds <= 3)
				{
					DcMotor_Rotate(CW, MAX_SPEED);
					PWM_Timer0_Start(g_dutyCycle);

				}
				g_seconds = 0;

				while (UART_recieveByte() != DOOR_IS_UNLOCKED)
				{
				}

				while (g_seconds <= 1)
				{
					DcMotor_Rotate(Stop, MAX_SPEED);
					PWM_Timer0_Start(g_dutyCycle);
				}
				g_seconds = 0;

				UART_sendByte(DOOR_IS_OPENED_SUCCESFULLY);

				while (g_seconds <= 3)
				{
						DcMotor_Rotate(A_CW, MAX_SPEED);
						PWM_Timer0_Start(g_dutyCycle);
				}
				DcMotor_Rotate(Stop, MAX_SPEED);
				UART_sendByte(DOOR_IS_CLOSED_SUCCESFULLY);
				g_seconds = 0;
			while (UART_recieveByte() != OPEN_DOOR_OPTION_IS_DONE_SUCCESFULLY)
			{}
			break;
			}

			}
			break;
			}

		case '-':
		{
			passwordMemoryCompare();
			UART_sendByte(passwordCompareResult);
			switch (passwordCompareResult)
			{
			case WRONG_PASSWORD:
			{
				uint8 counter=0;
				do
							{
								passwordMemoryCompare();
								counter++;
								UART_sendByte(passwordCompareResult);

							} while (passwordCompareResult == WRONG_PASSWORD && counter < 2);
							if (counter == 2 && passwordCompareResult == WRONG_PASSWORD)
							{
								UART_sendByte(SECURITY_BREACH);
								securityAlarm();
								UART_sendByte(ALARM_TIMEOUT);
								break;
							}
							else
								UART_sendByte(NOT_A_WRONG_PASSWORD);
			}
			case RIGHT_PASSWORD:
			{
				while (UART_recieveByte() != RIGHT_PASSWORD);
				returnToCreatePasswordFlag++;
				break;
			}

			}
			break;
		}
		}
			if (returnToCreatePasswordFlag == 1){
				returnToCreatePasswordFlag=0;
					break;
					}

	}
}
/*
 * Description :
 * Function calledback in the ISR responsible for Incrementing seconds
 */
void callBackIncrementSeconds(void)
{
	g_seconds++;
}
/*
 * Description :
 * Function responsible for activating the alarm buzzer for 60 seconds
 */
void securityAlarm(void)
{
	g_seconds = 0;
	while (g_seconds <= 5)
	{
		Buzzer_on();
	}
	Buzzer_off();
	g_seconds = 0;
}
/*
 * Description :
 * Function responsible for
 * 1. Receiving password
 * 2. compare the password with the saved one in memory
 * and save results in global variable
 */
void passwordMemoryCompare(void)
{
	recievePass();
	readPasswordFromMemory(memorycheck_arr);
	passwordCompareResult = Passwordcheck(pass_arr,memorycheck_arr);
}



