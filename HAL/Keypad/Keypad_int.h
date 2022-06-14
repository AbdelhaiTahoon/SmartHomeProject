/*
 * Keypad_int.h
 *
 *  Created on: Mar 15, 2022
 *      Author: abdelhai Tahoon
 */

#ifndef HAL_KEYPAD_KEYPAD_INT_H_
#define HAL_KEYPAD_KEYPAD_INT_H_

ES_t Keypad_enuInit(void);

ES_t Keypad_GetPressedKey(u8 * Copy_pu8KeyValue);

#define KEYPAD_NOT_PRESSED			0xff

#endif /* HAL_KEYPAD_KEYPAD_INT_H_ */
