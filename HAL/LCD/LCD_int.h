/*
 * LCD_int.h
 *
 *  Created on: Mar 13, 2022
 *      Author: abdelhai Tahoon
 */

#ifndef HAL_LCD_LCD_INT_H_
#define HAL_LCD_LCD_INT_H_

ES_t LCD_enuInit(void);

ES_t LCD_enuWriteData(u8 Copy_u8Data);

ES_t LCD_enuWriteCommand(u8 Copy_u8Command);

ES_t LCD_enuWriteString(const char * Copy_pcString );

ES_t LCD_enuWriteIntegerNum(s32 Copy_s32Num);

ES_t LCD_enuWriteFloatNum(f64 Copy_s32FloatNum);

/*			row 1 -> 2 , col 1 -> 16 */
ES_t LCD_enuGoToPosition(u8 Copy_u8Row, u8 Copy_u8Col);

ES_t LCD_enuClear(void);
ES_t LCD_enuCursorReverse(void);
#endif /* HAL_LCD_LCD_INT_H_ */
