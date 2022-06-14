/*
 * EXTI_config.c
 *
 *  Created on: Mar 16, 2022
 *      Author: abdelhai Tahoon
 */
#include "../../LIB/stdTypes.h"
#include "../../LIB/errorState.h"

#include "EXTI_config.h"
#include "EXTI_priv.h"

u8 EXTI_u8EXTINum = EXTI_NUM;


EXTI_t EXTI_AstrEXTIConfig [ EXTI_NUM ]=
{
		{FALLING , ENABLE , INT0},
		{FALLING , ENABLE , INT2},
		{RISING  , ENABLE , INT1}
};

