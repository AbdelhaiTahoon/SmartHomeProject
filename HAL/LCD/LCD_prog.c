/*
 * LCD_prog.c
 *
 *  Created on: Mar 13, 2022
 *      Author: abdelhai Tahoon
 */
#include "../../LIB/stdTypes.h"
#include "../../LIB/errorState.h"

#include "../../MCAL/DIO/DIO_int.h"

#include "LCD_config.h"
#include "LCD_priv.h"

#include <util\delay.h>


ES_t LCD_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK;

	_delay_ms(35);

	DIO_enuSetPinDirection(RS_GRP, RS_PIN, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(RW_GRP, RW_PIN, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(EN_GRP, EN_PIN, DIO_u8OUTPUT);

	DIO_enuSetPinDirection(D7_GRP, D7_PIN, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(D6_GRP, D6_PIN, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(D5_GRP, D5_PIN, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(D4_GRP, D4_PIN, DIO_u8OUTPUT);

#if LCD_MODE == EIGHT_BIT

	DIO_enuSetPinDirection(D3_GRP, D3_PIN, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(D2_GRP, D2_PIN, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(D1_GRP, D1_PIN, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(D0_GRP, D0_PIN, DIO_u8OUTPUT);

	DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
	LCD_enuWriteNLatch(0x38);

#elif LCD_MODE == FOUR_BIT

	DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);

	DIO_enuSetPinValue(D7_GRP, D7_PIN,  0  );
	DIO_enuSetPinValue(D6_GRP, D6_PIN,  0  );
	DIO_enuSetPinValue(D5_GRP, D5_PIN,  1  );
	DIO_enuSetPinValue(D4_GRP, D4_PIN,  0  );

	DIO_enuSetPinValue(EN_GRP, EN_PIN, DIO_u8HIGH);
	_delay_ms(10);
	DIO_enuSetPinValue(EN_GRP, EN_PIN, DIO_u8LOW);
	_delay_ms(10);

	LCD_enuWriteNLatch(0x28);

#else
#error "LCD Mode Configuration is wrong\n it must be EIGHT_BIT or FOUR_BIT"
#endif

	//display on/off
	DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
	LCD_enuWriteNLatch(0x0F);

	//clear display
	DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
	LCD_enuWriteNLatch(0x01);

	//Entry set mode
	DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
	LCD_enuWriteNLatch(0x06);

	return Local_enuErrorState;
}

ES_t LCD_enuWriteData(u8 Copy_u8Data)
{
	ES_t Local_enuErrorState = ES_NOK;

	DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
	LCD_enuWriteNLatch (Copy_u8Data);

	return Local_enuErrorState;
}

ES_t LCD_enuWriteCommand(u8 Copy_u8Command)
{
	ES_t Local_enuErrorState = ES_NOK;

	DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
	LCD_enuWriteNLatch(Copy_u8Command);

	return Local_enuErrorState;
}

ES_t LCD_enuWriteString(const char * Copy_pcString )
{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_pcString != NULL)
	{
		while (*Copy_pcString)
		{
			DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
			LCD_enuWriteNLatch (*Copy_pcString++);
		}
	}
	else
	{
		Local_enuErrorState = ES_NULL_POINTER;
	}

	return Local_enuErrorState;
}

ES_t LCD_enuWriteIntegerNum(s32 Copy_s32Num)
{

	ES_t Local_enuErrorState = ES_NOK;

	if(Copy_s32Num <0){
		Copy_s32Num *= -1;
		DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
		LCD_enuWriteNLatch ('-');
	}

	LCD_enuPrintInt (Copy_s32Num);


	return Local_enuErrorState;

	/*this code is using array*/
	/*
 	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_Au8Digits[12]={0}, Local_u8Digit = 0;
	if (Copy_s32Num==0)
	{
		DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
		LCD_enuWriteNLatch ('0');
	}
	if (Copy_s32Num <0)
	{
		DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
		LCD_enuWriteNLatch ('-');
		Copy_s32Num *= -1;
	}
	while (Copy_s32Num)
	{
		Local_u8Digit ++;
		Local_Au8Digits[Local_u8Digit] = Copy_s32Num%10 + '0';
		Copy_s32Num /=10;
	}
	while (Local_u8Digit)
	{
		DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
		LCD_enuWriteNLatch (Local_Au8Digits[Local_u8Digit--]);
	}
	return Local_enuErrorState;
	 */

}
ES_t LCD_enuWriteFloatNum(f64 Copy_s32FloatNum){
	ES_t Local_enuErrorState = ES_NOK;

	if(Copy_s32FloatNum <0){
		Copy_s32FloatNum *= -1 ;
		DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
		LCD_enuWriteNLatch ('-');
	}

	s32 LOC_s32int_part = (int) Copy_s32FloatNum;
	//print the integer part
	LCD_enuPrintInt(LOC_s32int_part);


	// converting the floating part to integer
	s32 LOC_s32float_part = ((Copy_s32FloatNum - LOC_s32int_part)*1000000);

	//print the dot  (.)
	if(LOC_s32float_part != 0){
		DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
		LCD_enuWriteNLatch ('.');
	}




	//print the floating part
	//LCD_enuPrintInt(LOC_s32float_part);

	s32 LOC_s32reverse =0;
	u8 LOC_remainder =0;
	while(LOC_s32float_part)
		{
			LOC_remainder = (u8)(LOC_s32float_part % 10);
			LOC_s32reverse = LOC_s32reverse *10 + LOC_remainder;
			LOC_s32float_part /=10;
		}
	/* printed the reversed float number On lcd */
	LOC_remainder=0;
	while(LOC_s32reverse)
	{
		LOC_remainder = (u8)((LOC_s32reverse % 10)+('0')) ;
		DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
		LCD_enuWriteNLatch (LOC_remainder);
		LOC_s32reverse /=10;
	}

	Local_enuErrorState = ES_OK;
	return Local_enuErrorState;

}


ES_t LCD_enuGoToPosition(u8 Copy_u8Row, u8 Copy_u8Col)
{

	ES_t Local_enuErrorState = ES_NOK;
	u8 arr[4] = {0x80,0xC0,0x90,0xD0};

	if (Copy_u8Row >=1 && Copy_u8Row <=4 &&
			Copy_u8Col >=1 && Copy_u8Col <=16 )
	{
		LCD_enuWriteCommand(arr[Copy_u8Row-1]+Copy_u8Col-1);
	}
	else
	{
		Local_enuErrorState = ES_OUT_OF_RANGE;
	}

/*
	if (Copy_u8Row >=1 && Copy_u8Row <=2 &&
			Copy_u8Col >=1 && Copy_u8Col <=16 )
	{
		DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
		LCD_enuWriteNLatch(0x80 + (Copy_u8Row-1)*0x40 + (Copy_u8Col-1));
	}
	else
	{
		Local_enuErrorState = ES_OUT_OF_RANGE;
	}
*/

	return Local_enuErrorState;
}

ES_t LCD_enuClear(void){
	ES_t Local_enuErrorState = ES_NOK;
	DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
	LCD_enuWriteNLatch(1);
	return Local_enuErrorState;

}

ES_t LCD_enuCursorReverse(void){
	ES_t Local_enuErrorState = ES_NOK;
	DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
	LCD_enuWriteNLatch(0x4);
	Local_enuErrorState = ES_OK;
	return Local_enuErrorState;

}

//static ES_t enuPrintInt(s32 Copy_s32Num){

//}



static ES_t LCD_enuWriteNLatch (u8 Copy_u8Byte)
{
	ES_t Local_enuErrorState = ES_NOK;

	DIO_enuSetPinValue(RW_GRP , RW_PIN , DIO_u8LOW);
	DIO_enuSetPinValue(EN_GRP , EN_PIN , DIO_u8LOW);

#if LCD_MODE == EIGHT_BIT
	DIO_enuSetPinValue(D7_GRP, D7_PIN,  ((Copy_u8Byte>>7)&1)  );
	DIO_enuSetPinValue(D6_GRP, D6_PIN,  ((Copy_u8Byte>>6)&1)  );
	DIO_enuSetPinValue(D5_GRP, D5_PIN,  ((Copy_u8Byte>>5)&1)  );
	DIO_enuSetPinValue(D4_GRP, D4_PIN,  ((Copy_u8Byte>>4)&1)  );
	DIO_enuSetPinValue(D3_GRP, D3_PIN,  ((Copy_u8Byte>>3)&1)  );
	DIO_enuSetPinValue(D2_GRP, D2_PIN,  ((Copy_u8Byte>>2)&1)  );
	DIO_enuSetPinValue(D1_GRP, D1_PIN,  ((Copy_u8Byte>>1)&1)  );
	DIO_enuSetPinValue(D0_GRP, D0_PIN,  ((Copy_u8Byte>>0)&1)  );

	DIO_enuSetPinValue(EN_GRP, EN_PIN, DIO_u8HIGH);
	_delay_ms(10);
	DIO_enuSetPinValue(EN_GRP, EN_PIN, DIO_u8LOW);
	_delay_ms(10);

#elif LCD_MODE == FOUR_BIT
	DIO_enuSetPinValue(D7_GRP, D7_PIN,  ((Copy_u8Byte>>7)&1)  );
	DIO_enuSetPinValue(D6_GRP, D6_PIN,  ((Copy_u8Byte>>6)&1)  );
	DIO_enuSetPinValue(D5_GRP, D5_PIN,  ((Copy_u8Byte>>5)&1)  );
	DIO_enuSetPinValue(D4_GRP, D4_PIN,  ((Copy_u8Byte>>4)&1)  );

	DIO_enuSetPinValue(EN_GRP, EN_PIN, DIO_u8HIGH);
	_delay_ms(10);
	DIO_enuSetPinValue(EN_GRP, EN_PIN, DIO_u8LOW);
	_delay_ms(10);

	DIO_enuSetPinValue(D7_GRP, D7_PIN,  ((Copy_u8Byte>>3)&1)  );
	DIO_enuSetPinValue(D6_GRP, D6_PIN,  ((Copy_u8Byte>>2)&1)  );
	DIO_enuSetPinValue(D5_GRP, D5_PIN,  ((Copy_u8Byte>>1)&1)  );
	DIO_enuSetPinValue(D4_GRP, D4_PIN,  ((Copy_u8Byte>>0)&1)  );

	DIO_enuSetPinValue(EN_GRP, EN_PIN, DIO_u8HIGH);
	_delay_ms(10);
	DIO_enuSetPinValue(EN_GRP, EN_PIN, DIO_u8LOW);
	_delay_ms(10);

#else
#error "LCD Mode Configuration is wrong\n it must be EIGHT_BIT or FOUR_BIT"
#endif
	return Local_enuErrorState;
}
static ES_t LCD_enuPrintInt (s32 Copy_s32Num)
{
	ES_t Local_enuErrorState = ES_NOK;

	s32 LOC_s32reverse =0;
	u8 LOC_remainder =0;
	u8 LOC_right_zero_nums =0;
	s32 LOC_CheckNum;

	LOC_CheckNum = Copy_s32Num;
	// checking numbers of zero on the rigth
	while(LOC_CheckNum){
		if(LOC_CheckNum %10 ==0){
			LOC_right_zero_nums++;
		}
		else
		{
			break;
		}
		LOC_CheckNum/=10;
	}


	/* reversing the integer Number to print on LCD*/
	while(Copy_s32Num)
	{
		LOC_remainder = (u8)(Copy_s32Num % 10);
		LOC_s32reverse = LOC_s32reverse *10 + LOC_remainder;
		Copy_s32Num /=10;
	}



	if(LOC_s32reverse == 0){
		DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
		LCD_enuWriteNLatch ('0');
	}

	/* printed the reversed number On lcd */
	LOC_remainder=0;
	while(LOC_s32reverse)
	{
		LOC_remainder = (u8)((LOC_s32reverse % 10)+('0')) ;
		DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
		LCD_enuWriteNLatch (LOC_remainder);
		LOC_s32reverse /=10;
	}

	/* checking if there is zeros in the right then printing it */

	while(LOC_right_zero_nums){
		DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
		LCD_enuWriteNLatch ('0');
		LOC_right_zero_nums--;
	}

	return Local_enuErrorState;
}
