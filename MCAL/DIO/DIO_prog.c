/*
 * DIO_prog.c
 *
 *  Created on: Mar 9, 2022
 *      Author: abdelhai Tahoon
 */
//public libs
#include "../../LIB/stdTypes.h"
//#include "BIT_MATH.h"
#include "../../LIB/errorState.h"

//lower layer libs

//own libs
#include "DIO_config.h"
#include "DIO_priv.h"

ES_t DIO_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK;

	//body

	return Local_enuErrorState;
}

ES_t DIO_enuSetPinDirection(u8 Copy_u8GroupID , u8 Copy_u8PinID, u8 Copy_u8Direction)
{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_u8GroupID <= DIO_u8GROUP_D &&
			Copy_u8PinID <= DIO_u8PIN7 &&
			Copy_u8Direction<= DIO_u8OUTPUT)
	{
		if(Copy_u8Direction ==1)
		{
			switch (Copy_u8GroupID)
			{
			case DIO_u8GROUP_A:
				DDRA |=   (Copy_u8Direction << Copy_u8PinID);
				break;
			case DIO_u8GROUP_B:
				DDRB |=   (Copy_u8Direction << Copy_u8PinID);
				break;
			case DIO_u8GROUP_C:
				DDRC |=   (Copy_u8Direction << Copy_u8PinID);
				break;
			case DIO_u8GROUP_D:
				DDRD |=   (Copy_u8Direction << Copy_u8PinID);
				break;
			}

		}
		else if (Copy_u8Direction ==0)
		{
			switch (Copy_u8GroupID)
			{
			case DIO_u8GROUP_A:
				DDRA &= ~ (DIO_BIT_MASK     << Copy_u8PinID);
				break;
			case DIO_u8GROUP_B:
				DDRB &= ~ (DIO_BIT_MASK     << Copy_u8PinID);
				break;
			case DIO_u8GROUP_C:
				DDRC &= ~ (DIO_BIT_MASK     << Copy_u8PinID);
				break;
			case DIO_u8GROUP_D:
				DDRD &= ~ (DIO_BIT_MASK     << Copy_u8PinID);
				break;
			}
		}

		Local_enuErrorState = ES_OK;
	}
	else
	{
		Local_enuErrorState = ES_OUT_OF_RANGE;
	}


	return Local_enuErrorState;
}

ES_t DIO_enuSetPinValue(u8 Copy_u8GroupID , u8 Copy_u8PinID, u8 Copy_u8Value)
{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_u8GroupID <= DIO_u8GROUP_D &&
			Copy_u8PinID <= DIO_u8PIN7 &&
			(Copy_u8Value <= DIO_u8HIGH
					|| Copy_u8Value <= DIO_u8PULL_UP)
	)
	{


		if(Copy_u8Value ==1)
		{
			switch (Copy_u8GroupID)
			{
			case DIO_u8GROUP_A:
				PORTA |=   (Copy_u8Value << Copy_u8PinID);
				break;
			case DIO_u8GROUP_B:
				PORTB |=   (Copy_u8Value << Copy_u8PinID);
				break;
			case DIO_u8GROUP_C:
				PORTC |=   (Copy_u8Value << Copy_u8PinID);
				break;
			case DIO_u8GROUP_D:
				PORTD |=   (Copy_u8Value << Copy_u8PinID);
				break;
			}
		}
		else if(Copy_u8Value ==0)
		{
			switch (Copy_u8GroupID)
			{
			case DIO_u8GROUP_A:
				PORTA &= ~ (DIO_BIT_MASK << Copy_u8PinID);
				break;
			case DIO_u8GROUP_B:
				PORTB &= ~ (DIO_BIT_MASK << Copy_u8PinID);
				break;
			case DIO_u8GROUP_C:
				PORTC &= ~ (DIO_BIT_MASK << Copy_u8PinID);
				break;
			case DIO_u8GROUP_D:
				PORTD &= ~ (DIO_BIT_MASK << Copy_u8PinID);
				break;
			}

		}

		Local_enuErrorState = ES_OK;
	}
	else
	{
		Local_enuErrorState = ES_OUT_OF_RANGE;
	}

	return Local_enuErrorState;
}

ES_t DIO_enuTogglePinValue(u8 Copy_u8GroupID , u8 Copy_u8PinID)
{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_u8GroupID <= DIO_u8GROUP_D &&
			Copy_u8PinID <= DIO_u8PIN7)
	{
		switch (Copy_u8GroupID)
		{
		case DIO_u8GROUP_A:
			PORTA ^= (DIO_BIT_MASK << Copy_u8PinID);
			break;
		case DIO_u8GROUP_B:
			PORTB ^= (DIO_BIT_MASK << Copy_u8PinID);
			break;
		case DIO_u8GROUP_C:
			PORTC ^= (DIO_BIT_MASK << Copy_u8PinID);
			break;
		case DIO_u8GROUP_D:
			PORTD ^= (DIO_BIT_MASK << Copy_u8PinID);
			break;
		}
	}
	else
	{
		Local_enuErrorState = ES_OUT_OF_RANGE;
	}
	return Local_enuErrorState;
}

ES_t DIO_enuGetPinValue(u8 Copy_u8GroupID, u8 Copy_u8PinID, u8 * Copy_pu8Value)
{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_pu8Value != NULL)
	{
		if (Copy_u8GroupID <=DIO_u8GROUP_D  &&
				Copy_u8PinID <= DIO_u8PIN7)
		{
			switch (Copy_u8GroupID)
			{
			case DIO_u8GROUP_A:
				*Copy_pu8Value = ( (PINA >> Copy_u8PinID) & DIO_BIT_MASK );
				break;
			case DIO_u8GROUP_B:
				*Copy_pu8Value = ( (PINB >> Copy_u8PinID) & DIO_BIT_MASK );
				break;
			case DIO_u8GROUP_C:
				*Copy_pu8Value = ( (PINC >> Copy_u8PinID) & DIO_BIT_MASK );
				break;
			case DIO_u8GROUP_D:
				*Copy_pu8Value = ( (PIND >> Copy_u8PinID) & DIO_BIT_MASK );
				break;
			}
		}
		else
		{
			Local_enuErrorState = ES_OUT_OF_RANGE;
		}
	}
	else
	{
		Local_enuErrorState = ES_NULL_POINTER;
	}


	return Local_enuErrorState;
}
