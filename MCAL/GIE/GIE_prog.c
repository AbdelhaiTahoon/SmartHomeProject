/*
 * GIE_prog.c
 *
 *  Created on: Mar 16, 2022
 *      Author: abdelhai Tahoon
 */
#include "../../LIB/stdTypes.h"
#include "../../LIB/errorState.h"

#include "GIE_priv.h"

ES_t GIE_enuInit(void)
{
	asm("CLI");
	//SREG &=~ (1<<7);
	return ES_OK;
}

ES_t GIE_enuEnable(void)
{
	asm("SEI");
	//SREG |=  (1<<7);
	return ES_OK;
}

ES_t GIE_enuDisable(void)
{
	asm("CLI");
	//SREG &=~ (1<<7);
	return ES_OK;
}
