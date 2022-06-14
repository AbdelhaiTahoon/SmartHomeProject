/*
 * Keypad_prog.c
 *
 *  Created on: Mar 15, 2022
 *      Author: abdelhai Tahoon
 */
#include "../../LIB/stdTypes.h"
#include "../../LIB/errorState.h"


#include "../../MCAL/DIO/DIO_int.h"


#include "Keypad_config.h"
#include "Keypad_priv.h"

#include <util/delay.h>

ES_t Keypad_enuInit(void)
{
	ES_t Local_enuErrorState =ES_NOK;

	DIO_enuSetPinDirection(C1_GRP , C1_PIN , DIO_u8OUTPUT);
	DIO_enuSetPinDirection(C2_GRP , C2_PIN , DIO_u8OUTPUT);
	DIO_enuSetPinDirection(C3_GRP , C3_PIN , DIO_u8OUTPUT);
	DIO_enuSetPinDirection(C4_GRP , C4_PIN , DIO_u8OUTPUT);

	DIO_enuSetPinValue(C1_GRP , C1_PIN , DIO_u8HIGH);
	DIO_enuSetPinValue(C2_GRP , C2_PIN , DIO_u8HIGH);
	DIO_enuSetPinValue(C3_GRP , C3_PIN , DIO_u8HIGH);
	DIO_enuSetPinValue(C4_GRP , C4_PIN , DIO_u8HIGH);

	DIO_enuSetPinDirection(R1_GRP , R1_PIN , DIO_u8INPUT);
	DIO_enuSetPinDirection(R2_GRP , R2_PIN , DIO_u8INPUT);
	DIO_enuSetPinDirection(R3_GRP , R3_PIN , DIO_u8INPUT);
	DIO_enuSetPinDirection(R4_GRP , R4_PIN , DIO_u8INPUT);

	DIO_enuSetPinValue(R1_GRP , R1_PIN , DIO_u8PULL_UP);
	DIO_enuSetPinValue(R2_GRP , R2_PIN , DIO_u8PULL_UP);
	DIO_enuSetPinValue(R3_GRP , R3_PIN , DIO_u8PULL_UP);
	DIO_enuSetPinValue(R4_GRP , R4_PIN , DIO_u8PULL_UP);

	return Local_enuErrorState;
}

ES_t Keypad_GetPressedKey(u8 * Copy_pu8KeyValue)
{
	ES_t Local_enuErrorState =ES_NOK;

	u8 Local_u8RowIter = 0;
	u8 Local_u8ColIter = 0;

	u8 Local_Au8RowGrp[]={R1_GRP, R2_GRP, R3_GRP, R4_GRP};
	u8 Local_Au8RowPin[]={R1_PIN, R2_PIN, R3_PIN, R4_PIN};

	u8 Local_Au8ColGrp[]={C1_GRP, C2_GRP, C3_GRP, C4_GRP};
	u8 Local_Au8ColPin[]={C1_PIN, C2_PIN, C3_PIN, C4_PIN};

	u8 Local_u8RowValue=96 , Local_u8Flag=0;

	for (;Local_u8ColIter < COL_NUM ; Local_u8ColIter++ )
	{
		DIO_enuSetPinValue(Local_Au8ColGrp[Local_u8ColIter],
			Local_Au8ColPin[Local_u8ColIter], DIO_u8LOW   );

		for (Local_u8RowIter=0; Local_u8RowIter<ROW_NUM; Local_u8RowIter++)
		{
			DIO_enuGetPinValue(Local_Au8RowGrp[Local_u8RowIter],
					Local_Au8RowPin[Local_u8RowIter], &Local_u8RowValue);
			if (! Local_u8RowValue)
			{
				_delay_ms(10);
				DIO_enuGetPinValue(Local_Au8RowGrp[Local_u8RowIter],
					Local_Au8RowPin[Local_u8RowIter], &Local_u8RowValue);
				if (! Local_u8RowValue)
				{
					while (! Local_u8RowValue)
						DIO_enuGetPinValue(Local_Au8RowGrp[Local_u8RowIter],
							Local_Au8RowPin[Local_u8RowIter], &Local_u8RowValue);

					u8 Local_Au8KeyValues[ROW_NUM][COL_NUM] =KEYS_VALUES;
					*Copy_pu8KeyValue = Local_Au8KeyValues[Local_u8RowIter][Local_u8ColIter];
					Local_u8Flag =1;
				}
			}
		}

		DIO_enuSetPinValue(Local_Au8ColGrp[Local_u8ColIter],
			Local_Au8ColPin[Local_u8ColIter], DIO_u8HIGH   );

	}
	if (! Local_u8Flag)
	{
		*Copy_pu8KeyValue = 0xff;
	}


	return Local_enuErrorState;
}
