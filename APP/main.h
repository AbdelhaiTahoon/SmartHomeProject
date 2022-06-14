/*
 * main.h
 *
 *  Created on: May 26, 2022
 *      Author: abdelhai Tahoon
 */

#ifndef APP_MAIN_H_
#define APP_MAIN_H_

/****************************************************************************/
/********************  	Includes Section		*****************/
/* Libraries 	 	--------------------------------------------*/
#include "../LIB/stdTypes.h"
#include "../LIB/errorState.h"

/* MCAL 	 		--------------------------------------------*/
#include "../MCAL/DIO/DIO_int.h"
#include "../MCAL/GIE/GIE_int.h"
#include "../MCAL/ADC/ADC_int.h"

/* HAL		 	 	--------------------------------------------*/
#include "../HAL/LCD/LCD_int.h"
#include "../HAL/Keypad/Keypad_int.h"

/* FREE_RTOS 	 	--------------------------------------------*/
#include "../FREE_RTOS/FreeRTOS.h"
#include "../FREE_RTOS/task.h"
#include "../FREE_RTOS/semphr.h"

/****************************************************************************/


/****************************************************************************/
/******************************  	MACROS Definition		*****************/
#define TRUE				1
#define FALSE 				0
#define DOOR_OPENING  		0
#define DOOR_CLOSED   		1
#define MAX_TEMPRATURE		22
#define ULTIMATE_GAS_VALUE	2000
#define MAX_TRIALS  		3
/****************************************************************************/


/****************************************************************************/
/**************** User Defined variables definition	*****************/
typedef struct{
	u8 LoginFlag			:1;
	u8 EmergencyFlag		:1;
	u8 EnteredEmergencyFlag	:1;
	u8 keypressedFlag		:1;
	u8 WrongPassFlag		:1;
	u8 DoorFlag				:1;
	u8 FanFlag				:1;
	u8 WinodwFlag			:1;
}SystemFags_t;
/****************************************************************************/

/****************************************************************************/
/**************** definition of system tasks	*****************/
void vTaskUserPanel	(void*pv);
void taskLoginPannel(void*pv);
void vtaskAdcRead	(void*pv);
void vTaskKeypad		(void*pv);
void vTaskEmergency	(void*pv);
void vtaskDoor		(void*pv);
void vTaskFan		(void*pv);
void vTaskWindow		(void*pv);
/****************************************************************************/



/****************************************************************************/
/**************** definition of App Function	*****************/
/* definition  of app Function*/
void vAdcIsrNotification(void*p);
void vSystemInitialization(void);

/****************************************************************************/


#endif /* APP_MAIN_H_ */
