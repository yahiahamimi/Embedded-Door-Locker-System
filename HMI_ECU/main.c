/*
 * main.c
 *
 *  Created on: Mar 17, 2024
 *      Author: Yahia Hamimi
 */
#include "timer1.h"
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "main.h"
#include <avr/io.h>
#include <util/delay.h>

/*******************************************************************************
 *                         Global Variables & Configurations                   *
 *******************************************************************************/
uint8 g_seconds;
uint8 returnToCreatePasswordFlag = 0;
UART_ConfigType UART_Configuration_data = {bit_8, even_parity, stop_1_bit, 9600};
Timer1_ConfigType Timer_Configuration_data = {0, 7812, F_cpu_1024, Compare_mode};

/*
 * The project consists of two main parts, and these parts contain sub-parts:
 * 1. Create system password (logically is always the first step)
 * 2. Main menu options
 * Main Function always looping around these two options
 * as long as the system is running
 */
int main(void)
{
	HMI_ECU_init();
	while (1)
	{
		while (UART_recieveByte() != CONTROL_ECU_READY)
		{
		}
		passwordMenu();
		UART_sendByte(HMI_ECU_READY);
		while (UART_recieveByte() != CONTROL_ECU_WAITING_FOR_MENU_OPTION)
		{
		};
		mainOptionsMenu();
	}
}
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for HMI modules Initialization and activate interrupts
 * 1. initialize UART
 * 2. initialize LCD
 * 3. initialize Timer 1 and give it the address of the function to be called back
 */
void HMI_ECU_init(void)
{
	SREG |= (1 << 7); /* Enable global interrupt */
	UART_init(&UART_Configuration_data);
	LCD_init();
	Timer1_init(&Timer_Configuration_data);
	Timer1_setCallBack(callBackIncrementSeconds);
}

/*
 * Description :
 * 1. display the entered password encrypted into (*)
 * 2. Send the Password byte by byte to ECUC
 */
void sendPassword(void)
{
	_delay_ms(500); /* Press time */
	uint8 key = KEYPAD_getPressedKey();

	/* loop until user press the assigned enter key on keypad */
	while (key != KEYPAD_ENTER_BUTTON)
	{
		//if(key>=0&&key<=9){/*Don't receive any special character or symbol*/
		LCD_displayCharacter('*');
		UART_sendByte(key);
		_delay_ms(500);
		//}
		key = KEYPAD_getPressedKey();
	}
	LCD_clearScreen();
}

/*
 * Description :
 * Function responsible for entering password:
 * 1. Display The required text
 * 2. Call sendPassword function
 */

void enterPasswordoption(void)
{
	/* Always clear LCD first In case there was a text
	 * being displayed before this function call */
	LCD_clearScreen();
	LCD_displayString("plz enter pass:");
	LCD_moveCursor(1, 0);
	sendPassword();
}
/*
 * Description :
 * Function responsible for re-entering password:
 * 1. Display The required text
 * 2. Call sendPassword function
 */
void reEnterPasswordOption(void)
{
	LCD_clearScreen();
	LCD_displayString("plz re-enter the");
	LCD_moveCursor(1, 0);
	LCD_displayString("same pass:");
	LCD_moveCursor(1, 10);
	sendPassword();
}
/*
 * Description :
 * Function responsible for looping around entering & re-entering password
 * functions until having a valid reply from CECU
 */
void passwordMenu(void)
{
	do
	{
		_delay_ms(INPUT_DELAY);
		enterPasswordoption();
		_delay_ms(INPUT_DELAY);
		reEnterPasswordOption();
		_delay_ms(INPUT_DELAY);
	} while (UART_recieveByte() != CONTROL_ECU_GOT_VALID_PASSWORD);
}

/*
 * Description :
 * Function responsible for displaying the alarm message for 60 seconds
 */
void securityAlarmMessage(void)
{
	LCD_clearScreen();
	g_seconds = 0;
	LCD_displayString("Possible");
	LCD_moveCursor(1, 0);
	LCD_displayString("break-in!!");

	while (g_seconds <= 5)
	{
	}

	g_seconds = 0;
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
 * Function looping around main menu options
 * 1. Display the required options text
 * 2. Send the chosen text to the CECU
 * 3. Based on the chosen symbol and the password compare results received
 * from CECU display the required text for certain time
 */
void mainOptionsMenu(void)
{
	while (1)
	{
		uint8 option;
		while (1)
		{
			LCD_clearScreen();
			LCD_displayString("+  : Open Door");
			LCD_moveCursor(1, 0);
			LCD_displayString("-  : Change Pass");
			/* Don't Receive any key except +,- */
			uint8 key = KEYPAD_getPressedKey();
			if (key == '+' || key == '-')
			{
				/* Send the received key to CECU*/
				UART_sendByte(key);
				option = key;
				break;
			}
		}
		/* Wait until CECU confirms that it got the required option */
		while (UART_recieveByte() != CONTROL_ECU_GOT_MENU_OPTION)
		{
		}

		switch (option)
		{
		case '+':
		{
			_delay_ms(INPUT_DELAY);
			enterPasswordoption();
			_delay_ms(INPUT_DELAY);
			/* Receive the password compare result from CECU */
			uint8 recievedResult = UART_recieveByte();
			switch (recievedResult)
			{
			case WRONG_PASSWORD:
			{
				/*
				 * This part deals with the number of wrong password entries
				 * aside the first check we want to check two more times if the user
				 * fails to enter the right password three consecutive times,Security
				 * message will be displayed then break from this case
				 * In case the user enter a right pass any time before reaching three
				 * consecutive mistakes nothing will happen and the program will continue
				 * to right password option
				 */
				uint8 counter = 0;
				do
				{
					_delay_ms(INPUT_DELAY);
					enterPasswordoption();
					counter++;
					/*
					 * loop as long as the password compare result is wrong but the
					 * maximum number allowed for checking is two
					 */
				} while (UART_recieveByte() != RIGHT_PASSWORD && counter < 2);
				_delay_ms(INPUT_DELAY);
				if (UART_recieveByte() == SECURITY_BREACH)
				{
					securityAlarmMessage();
					/* Wait until CECU confirms that alarm time is done */
					while (UART_recieveByte() != ALARM_TIMEOUT)
					{
					}

					/* break and return to main menu options from the beginning */
					break;
				}
			}
			case RIGHT_PASSWORD:
			{
				/*
				 * This part deals with the number of Right password actions
				 */

				/* Reconfirm that HMI & CECU in the same phase with the same right password*/
				UART_sendByte(RIGHT_PASSWORD);
				/* The timer is enabled from the beginning each time we want to count events
				 * just make g_seconds=0
				 */
				g_seconds = 0;
				LCD_clearScreen();
				/* Display door state (unlocking) for 15 seconds */
				LCD_displayString("Door unlocking");
				while (g_seconds <= 3)
				{
				}

				g_seconds = 0;
				UART_sendByte(DOOR_IS_UNLOCKED);
				LCD_clearScreen();
				/* Wait until CECU confirms that the door was stayed open for 3 seconds */
				while (UART_recieveByte() != DOOR_IS_OPENED_SUCCESFULLY)
				{
				}
				/* Display door state (locking) for 15 seconds */
				LCD_clearScreen();
				g_seconds = 0;
				LCD_displayString("Door locking");
				while (g_seconds <= 3)
				{
				}

				g_seconds = 0;
				LCD_clearScreen();
				/* Wait until CECU confirms that the door is closed */
				while (UART_recieveByte() != DOOR_IS_CLOSED_SUCCESFULLY)
				{
				}
				/* Confirm that option display is fully done synchronized with CECU */
				UART_sendByte(OPEN_DOOR_OPTION_IS_DONE_SUCCESFULLY);
				break;
			}
			}
			break;
		}

		case '-':
		{
			_delay_ms(INPUT_DELAY);
			enterPasswordoption();
			_delay_ms(INPUT_DELAY);
			/* Receive the password compare result from CECU */
			uint8 recievedResult = UART_recieveByte();
			switch (recievedResult)
			{
			case WRONG_PASSWORD:
			{
				/*
				 * This part deals with the number of wrong password entries
				 * aside the first check we want to check two more times if the user
				 * fails to enter the right password three consecutive times,Security
				 * message will be displayed then break from this case
				 * In case the user enter a right pass any time before reaching three
				 * consecutive mistakes nothing will happen and the program will continue
				 * to right password option
				 */
				uint8 counter = 0;
				do
				{
					_delay_ms(INPUT_DELAY);
					enterPasswordoption();
					counter++;
				} while (UART_recieveByte() != RIGHT_PASSWORD && counter < 2);
				_delay_ms(INPUT_DELAY);
				if (UART_recieveByte() == SECURITY_BREACH)
				{
					securityAlarmMessage();
					/* Wait until CECU confirms that alarm time is done */
					while (UART_recieveByte() != ALARM_TIMEOUT)
					{
					}
					/* break and return to main menu options from the beginning */
					break;
				}
			}
			case RIGHT_PASSWORD:
			{
				/* If the password is right set a flag */
				UART_sendByte(RIGHT_PASSWORD);
				returnToCreatePasswordFlag++;
				break;
			}
			}

			break;
		}
		}
		if (returnToCreatePasswordFlag == 1)
		{
			/* If the flag is is set break from main menu options and return to password menu */
			returnToCreatePasswordFlag = 0;
			break;
		}
	}
}
