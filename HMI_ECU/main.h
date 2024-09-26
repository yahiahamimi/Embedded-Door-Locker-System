/*
 * main.h
 *
 *  Created on: Mar 17, 2024
 *      Author: Yahia Hamimi
 */

#ifndef MAIN_H_
#define MAIN_H_

/*******************************************************************************
 *                              Preprocessor Macros                            *
 *******************************************************************************/
#define CONTROL_ECU_READY 0x10
#define HMI_ECU_READY 0x12
#define CONTROL_ECU_GOT_VALID_PASSWORD 0x14
#define CONTROL_ECU_WAITING_FOR_MENU_OPTION 0x16
#define CONTROL_ECU_GOT_MENU_OPTION 0x18
#define ALARM_TIMEOUT 0x20
#define DOOR_IS_UNLOCKED 0x22
#define SECURITY_BREACH 0x24
#define DOOR_IS_OPENED_SUCCESFULLY 0x26
#define DOOR_IS_CLOSED_SUCCESFULLY 0x30
#define OPEN_DOOR_OPTION_IS_DONE_SUCCESFULLY 0x28
#define RIGHT_PASSWORD 1
#define WRONG_PASSWORD 0
#define PASS_LENGTH 5
#define KEYPAD_ENTER_BUTTON '='
#define INPUT_DELAY 30

/*******************************************************************************
 *                      Functions Prototypes                                  *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for HMI modules Initialization and activate interrupts
 * 1. initialize UART
 * 2. initialize LCD
 * 3. initialize Timer 1 and give it the address of the function to be called back
 */
void HMI_ECU_init(void);
/*
 * Description :
 * 1. display the entered password encrypted into (*)
 * 2. Send the Password byte by byte to ECUC
 */
void sendPassword(void);

/*
 * Description :
 * Function responsible for entering password:
 * 1. Display The required text
 * 2. Call sendPassword function
 */
void enterPasswordoption(void);

/*
 * Description :
 * Function responsible for re-entering password:
 * 1. Display The required text
 * 2. Call sendPassword function
 */
void reEnterPasswordOption(void);

/*
 * Description :
 * Function responsible for looping around entering & re-entering password
 * functions until having a valid reply from CECU
 */
void passwordMenu(void);

/*
 * Description :
 * Function responsible for displaying the alarm message for 60 seconds
 */
void securityAlarmMessage(void);

/*
 * Description :
 * Function calledback in the ISR responsible for Incrementing seconds
 */
void callBackIncrementSeconds(void);

/*
 * Description :
 * Function looping around main menu options
 * 1. Display the required options text
 * 2. Send the chosen text to the CECU
 * 3. Based on the chosen symbol and the password compare results received
 * from CECU display the required text for certain time
 */
void mainOptionsMenu(void);

#endif /* MAIN_H_ */
