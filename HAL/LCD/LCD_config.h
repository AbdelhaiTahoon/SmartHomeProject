/*
 * LCD_config.h
 *
 *  Created on: Mar 13, 2022
 *      Author: abdelhai Tahoon
 */

#ifndef HAL_LCD_LCD_CONFIG_H_
#define HAL_LCD_LCD_CONFIG_H_

/*			LCD MODE
 * 		EIGHT_BIT	or		FOUR_BIT	*/
#define LCD_MODE		EIGHT_BIT

/*			Control Pins		*/
#define RS_GRP			DIO_u8GROUP_B
#define RS_PIN			DIO_u8PIN1

#define RW_GRP			DIO_u8GROUP_B
#define RW_PIN			DIO_u8PIN2

#define EN_GRP			DIO_u8GROUP_B
#define EN_PIN			DIO_u8PIN3

/* 			Data Pins 			*/
#define D7_GRP			DIO_u8GROUP_D
#define D7_PIN			DIO_u8PIN7

#define D6_GRP			DIO_u8GROUP_D
#define D6_PIN			DIO_u8PIN6

#define D5_GRP			DIO_u8GROUP_D
#define D5_PIN			DIO_u8PIN5

#define D4_GRP			DIO_u8GROUP_D
#define D4_PIN			DIO_u8PIN4


// in board these are not connected
#define D3_GRP			DIO_u8GROUP_D
#define D3_PIN			DIO_u8PIN3

#define D2_GRP			DIO_u8GROUP_D
#define D2_PIN			DIO_u8PIN2

#define D1_GRP			DIO_u8GROUP_D
#define D1_PIN			DIO_u8PIN1

#define D0_GRP			DIO_u8GROUP_D
#define D0_PIN			DIO_u8PIN0

#endif /* HAL_LCD_LCD_CONFIG_H_ */
