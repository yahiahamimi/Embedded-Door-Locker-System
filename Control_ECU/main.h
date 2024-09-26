/*
 * main.h
 *
 *  Created on: Mar 17, 2024
 *      Author: Yahia Hamimi
 */

#ifndef MAIN_H_
#define MAIN_H_

/*******************************************************************************
 *                              Preprocessor Macros                         *
 *******************************************************************************/
#define CONTROL_ECU_READY 0x10
#define HMI_ECU_READY 0x12
#define CONTROL_ECU_GOT_VALID_PASSWORD 0x14
#define PASSWORD_NOT_MATCHED 0X15
#define CONTROL_ECU_WAITING_FOR_MENU_OPTION 0x16
#define CONTROL_ECU_GOT_MENU_OPTION 0x18
#define ALARM_TIMEOUT 0x20
#define DOOR_IS_UNLOCKED 0x22
#define SECURITY_BREACH 0x24
#define DOOR_IS_OPENED_SUCCESFULLY 0x26
#define OPEN_DOOR_OPTION_IS_DONE_SUCCESFULLY 0x28
#define DOOR_IS_CLOSED_SUCCESFULLY 0x30
#define NOT_A_WRONG_PASSWORD 0x32
#define RIGHT_PASSWORD 1
#define WRONG_PASSWORD 0
#define PASS_LENGTH 5
#define MAX_SPEED 100


/*******************************************************************************
 *                      Functions Prototypes                                  *
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
void CECU_init(void);

/*
 * Description :
 * Function responsible for storing password in the EEPROM
 */
void storePasswordInMemory(uint8 u8password[]);
/*
 * Description :
 * Function responsible for reading password from the EEPROM
 */
void readPasswordFromMemory(uint8 u8password[]);

/*
 * Description :
 * Function responsible for creating password:
 * 1. Receive the entered password from HMI
 * 2. Receive password re-confirmation from HMI
 * 3. Compare the two passwords and store it in memory
 * if it's valid password
 */
void createPassword(void);

/*
 * Description :
 * Function responsible for Receive the entered password from HMI
 * and saving it to the global password array
 */
void recievePass(void);
/*
 * Description :
 * Function responsible for Receive the entered password from HMI
 * and saving it to the global re-confirmation password array
 */
void recievePassreconfirm(void);

/*
 * Description :
 * Function responsible for comparing two passwords and
 * return the boolean results
 */
boolean Passwordcheck(uint8 First_arr[],uint8 Second_arr[]);

/*
 * Description :
 * Function looping around main menu and doing the received action from HMI
 * 1. Receive the chosen option from HMI
 * 2. Receive the entered password and compare it with the saved one in memory
 * 3. Based on the compare results do certain actions
 */
void mainOptionsMenuActions(void);

/*
 * Description :
 * Function responsible for activating the alarm buzzer for 60 seconds
 */
void securityAlarm(void);

/*
 * Description :
 * Function calledback in the ISR responsible for Incrementing seconds
 */
void callBackIncrementSeconds(void);

/*
 * Description :
 * Function responsible for
 * 1. Receiving password
 * 2. compare the password with the saved one in memory
 * and save results in global variable
 */
void passwordMemoryCompare(void);


#endif /* MAIN_H_ */
