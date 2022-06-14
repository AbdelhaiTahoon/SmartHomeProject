/*
 * DIO_int.h
 *
 *  Created on: Mar 9, 2022
 *      Author: abdelhai Tahoon
 */

#ifndef DIO_INT_H_
#define DIO_INT_H_

ES_t DIO_enuInit(void);

ES_t DIO_enuSetPinDirection(u8 Copy_u8GroupID , u8 Copy_u8PinID, u8 Copy_u8Direction);

ES_t DIO_enuSetPinValue(u8 Copy_u8GroupID , u8 Copy_u8PinID, u8 Copy_u8Value);

ES_t DIO_enuTogglePinValue(u8 Copy_u8GroupID , u8 Copy_u8PinID);

ES_t DIO_enuGetPinValue(u8 Copy_u8GroupID, u8 Copy_u8PinID, u8 * Copy_pu8Value);

#define DIO_u8GROUP_A			0
#define DIO_u8GROUP_B			1
#define DIO_u8GROUP_C			2
#define DIO_u8GROUP_D			3

#define DIO_u8PIN0				0
#define DIO_u8PIN1				1
#define DIO_u8PIN2				2
#define DIO_u8PIN3				3
#define DIO_u8PIN4				4
#define DIO_u8PIN5				5
#define DIO_u8PIN6				6
#define DIO_u8PIN7				7

#define DIO_u8INPUT				0
#define DIO_u8OUTPUT			1

#define DIO_u8LOW				0
#define DIO_u8HIGH				1

#define DIO_u8FLOAT				0
#define DIO_u8PULL_UP			1

#endif /* DIO_INT_H_ */
