/*
 * EXTI_prog.c
 *
 *  Created on: Mar 16, 2022
 *      Author: abdelhai Tahoon
 */
#include "../../LIB/stdTypes.h"
#include "../../LIB/errorState.h"

#include "EXTI_config.h"
#include "EXTI_priv.h"

#include "../interrupt.h"


extern u8 EXTI_u8EXTINum;


static void (* EXTI_ApfunISRFun[3])(void*)={NULL,NULL,NULL};
static void * EXTI_ApvidISRParameter [3] ={NULL,NULL,NULL};


ES_t EXTI_enuInit(EXTI_t * Copy_pAstrEXTIConfig)
{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_pAstrEXTIConfig != NULL)
	{
		u8 Local_u8Iter=0;
		for(;Local_u8Iter < EXTI_u8EXTINum ; Local_u8Iter++)
		{
			if (Copy_pAstrEXTIConfig[Local_u8Iter].intNum == INT0 ||
					Copy_pAstrEXTIConfig[Local_u8Iter].intNum == INT1)
			{
				MCUCR &= ~ (3 << (2*Copy_pAstrEXTIConfig[Local_u8Iter].intNum) );

				switch (Copy_pAstrEXTIConfig[Local_u8Iter].senseLvl)
				{
				case LOW_LVL:
					Local_enuErrorState = ES_OK;
					break;
				case ANY_LOGICAL:
					MCUCR |=   (1 << (2*Copy_pAstrEXTIConfig[Local_u8Iter].intNum) );
					Local_enuErrorState = ES_OK;
					break;

				case FALLING:
					MCUCR |=   (2 << (2*Copy_pAstrEXTIConfig[Local_u8Iter].intNum) );
					Local_enuErrorState = ES_OK;
					break;

				case RISING:
					MCUCR |=   (3 << (2*Copy_pAstrEXTIConfig[Local_u8Iter].intNum) );
					Local_enuErrorState = ES_OK;
					break;

				default :
					Local_enuErrorState = ES_OUT_OF_RANGE;
				}

				if (Copy_pAstrEXTIConfig[Local_u8Iter].intMode == ENABLE)
				{
					GICR |= (1 << (6 + Copy_pAstrEXTIConfig[Local_u8Iter].intNum));
				}
				else if (Copy_pAstrEXTIConfig[Local_u8Iter].intMode == DISABLE)
				{
					GICR &=~(1 << (6 + Copy_pAstrEXTIConfig[Local_u8Iter].intNum));
				}
				else
				{
					Local_enuErrorState = ES_OUT_OF_RANGE;
				}


			}
			else if(Copy_pAstrEXTIConfig[Local_u8Iter].intNum == INT2)
			{
				MCUCSR &= ~ (1 << 6);

				switch (Copy_pAstrEXTIConfig[Local_u8Iter].senseLvl)
				{
				case FALLING:
					Local_enuErrorState = ES_OK;
					break;

				case RISING:
					MCUCSR |=   (1 << 6);
					Local_enuErrorState = ES_OK;
					break;

				default :
					Local_enuErrorState = ES_OUT_OF_RANGE;
				}

				if (Copy_pAstrEXTIConfig[Local_u8Iter].intMode == ENABLE)
				{
					GICR |= (1 << 5);
				}
				else if (Copy_pAstrEXTIConfig[Local_u8Iter].intMode == DISABLE)
				{
					GICR &=~(1 << 5);
				}
				else
				{
					Local_enuErrorState = ES_OUT_OF_RANGE;
				}

				Local_enuErrorState = ES_OK;
			}
			else
			{
				Local_enuErrorState = ES_OUT_OF_RANGE;
			}
		}
	}
	else
	{
		Local_enuErrorState = ES_NULL_POINTER;
	}

	return Local_enuErrorState;
}

ES_t EXTI_enuSetSenseLvl(EXTI_t * Copy_pstrEXTIInfo, u8 Copy_u8SenseLvl)
{
	ES_t Local_enuErrorState = ES_NOK;



	return Local_enuErrorState;
}

ES_t EXTI_enuEnableInt(EXTI_t * Copy_pstrEXTIInfo)
{
	ES_t Local_enuErrorState = ES_NOK;



	return Local_enuErrorState;
}

ES_t EXTI_enuDisbleInt(EXTI_t * Copy_pstrEXTIInfo)
{
	ES_t Local_enuErrorState = ES_NOK;



	return Local_enuErrorState;
}


ES_t EXTI_enuCallBack(EXTI_t * Copy_pstrEXTIInfo, void (*Copy_pfunAppFun)(void*) , void * Copy_pvidAppParameter)
{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_pstrEXTIInfo != NULL &&
			Copy_pfunAppFun != NULL)
	{
		if (Copy_pstrEXTIInfo -> intNum <= INT2)
		{
			EXTI_ApfunISRFun[ Copy_pstrEXTIInfo->intNum ] = Copy_pfunAppFun;
			EXTI_ApvidISRParameter[ Copy_pstrEXTIInfo->intNum ] = Copy_pvidAppParameter;
			Local_enuErrorState = ES_OK;
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


ISR(INT0_vect)
{
	if (EXTI_ApfunISRFun[INT0] != NULL)
	{
		EXTI_ApfunISRFun [INT0] ( EXTI_ApvidISRParameter[ INT0 ] );
	}
}


ISR(INT1_vect)
{
	if (EXTI_ApfunISRFun[INT1] != NULL)
	{
		EXTI_ApfunISRFun [INT1] ( EXTI_ApvidISRParameter[ INT1 ] );
	}
}


ISR(INT2_vect)
{
	if (EXTI_ApfunISRFun[INT2] != NULL)
	{
		EXTI_ApfunISRFun [INT2] ( EXTI_ApvidISRParameter[ INT2 ] );
	}
}
