/*
 * buzzer.h
 *
 *  Created on: Mar 16, 2024
 *      Author: Yahia Hamimi
 */

#ifndef BUZZER_H_
#define BUZZER_H_




#define BUZZER_PORT_ID PORTA_ID
#define BUZZER_PIN_ID PIN0_ID


void Buzzer_init();


void Buzzer_on(void);

void Buzzer_off(void);

#endif /* BUZZER_H_ */
