/*
 * ADC_prog.h
 *
 *  Created on: Oct 23, 2021
 *      Author: abdelhai Tahoon
 */
#include "../../LIB/stdTypes.h"
#include "../../LIB/errorState.h"

#include "ADC_config.h"
#include "ADC_priv.h"

#include "../interrupt.h"

static volatile void (* ADC_pfunISRFun)(void*) =NULL;
static void * ADC_pvidISRParameter= NULL;


ES_t ADC_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK;

#if ADC_PRES == PRES_64
	ADCSRA &= ~(7<<0);
	ADCSRA |=  (6<<0);
#else
#error "ADC prescaler is wrong"
#endif

#if INIT_CHNL >=0 && INIT_CHNL <=31

	ADMUX &= 0xE0;
	ADMUX |= (INIT_CHNL <<0);

#else
#error "ADC Channel is Wrong"
#endif

#if INIT_MODE == NORMAL
	ADCSRA &= ~(1<<5);
#elif INIT_MODE == FREE_RUNINNG
	ADCSRA &= ~(1<<5);
	SFIOR  &= 0x1F;
	ADCSRA |=  (1<<5);
#elif INIT_MODE == TRIGGERED
	ADCSRA &= ~(1<<5);
	#if INIT_TRIG_SRC == EXT0
		SFIOR &= 0x1F;
		SFIOR |= (2<<5);
	#else
	#error "ADC trigger is wrong"
	#endif

#else
#error "Adc Mode is wrong"
#endif

#if INIT_INT_MODE == POLLING
		ADCSRA &= ~(1<<3);
#elif INIT_INT_MODE == ENABLED
		ADCSRA |= (1<<3);
#else
#error "ADC interrupt mode is wrong"
#endif

#if ADC_VREF == AVCC_REF
	ADMUX &= 0x3F;
	ADMUX |= (1<<6);
#else
#error "ADC voltage ref is wrong"
#endif


#if ADC_ADJ == LEFT_ADJ
	ADMUX |= (1<<5);
#elif ADC_ADJ == RIGHT_ADJ
	ADMUX &= ~(1<<5);
#else
#error"ADC Adjustment is wrong"
#endif

	return Local_enuErrorState;
}









ES_t ADC_enuStartConversion(void)
{
	ADCSRA |=(1<<6);
	return ES_OK;
}

ES_t ADC_enuReadHighRegister(u8 * Copy_pu8ReadValue)
{
	ES_t Local_enuErrorState = ES_NOK;

#if ADC_ADJ == LEFT_ADJ
	*Copy_pu8ReadValue = ADCH;

#elif ADC_ADJ == RIGHT_ADJ
	*Copy_pu8ReadValue  = (ADCL>>2);
	*Copy_pu8ReadValue |= (ADCH<<6);
#else
#error"ADC ADJ selection is wrong"
#endif


	return Local_enuErrorState;
}

ES_t ADC_enuRead(u16 * Copy_pu16ReadValue)
{
	ES_t Local_enuErrorState = ES_NOK;

#if ADC_ADJ == LEFT_ADJ
	*Copy_pu16ReadValue  = (ADCL>>6);
	*Copy_pu16ReadValue |= ((u16)ADCH<<2);
#elif ADC_ADJ == RIGHT_ADJ
	*Copy_pu16ReadValue  = ADCL;
	*Copy_pu16ReadValue |= ((u16)ADCH<<8);
#else
#error"ADC ADJ selection is wrong"
#endif

	return Local_enuErrorState;
}

ES_t ADC_enuCallBack(void (*Copy_pfunAppFun)(void*), void * Copy_pvidAppParameter)
{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_pfunAppFun != NULL)
	{
		ADC_pfunISRFun = Copy_pfunAppFun;
		ADC_pvidISRParameter = Copy_pvidAppParameter;
	}

	return Local_enuErrorState;
}

ES_t ADC_enuSelectChannel(u8 Copy_u8ChannelID)
{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_u8ChannelID <= 31 )
	{
		ADMUX &= ~0x1F; //0xE0 // 0b11100000
		ADMUX |= Copy_u8ChannelID;
	}
	else
	{

	}

	return Local_enuErrorState;
}

ES_t ADC_enuEnableAutoTrigger(u8 Copy_u8IntSource)
{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_u8IntSource <= 7)
	{
		ADCSRA &=~(1<<5);
		SFIOR  &= 0x1F;

		SFIOR  |= (Copy_u8IntSource <<5);
		ADCSRA |= (1<<5);
	}
	else
	{

	}

	return Local_enuErrorState;
}

ES_t ADC_enuDisableAutoTrigger(void)
{
	ADCSRA &=~(1<<5);
	return ES_OK;
}

ES_t ADC_enuPolling(void)
{
	while (! ((ADCSRA>>4)&1) );
	return ES_OK;
}


ES_t ADC_enuClearFlag(void)
{
	ADCSRA |=(1<<4);
	return ES_OK;
}


ES_t ADC_enuEnableADC(void)
{
	ES_t Local_enuErrorState = ES_NOK;

	ADCSRA |= (1<<7);

	return Local_enuErrorState;
}

ES_t ADC_enuDisableADC(void)
{
	ES_t Local_enuErrorState = ES_NOK;

	ADCSRA &=~(1<<7);

	return Local_enuErrorState;
}

ES_t ADC_enuEnableADC_INT(void)
{
	ES_t Local_enuErrorState = ES_NOK;

	ADCSRA |= (1<<3);

	return Local_enuErrorState;
}

ES_t ADC_enuDisableADC_INT(void)
{
	ES_t Local_enuErrorState = ES_NOK;

	ADCSRA &=~(1<<3);

	return Local_enuErrorState;
}

ISR(ADC_vect)
{
	if (ADC_pfunISRFun != NULL)
	{
		ADC_pfunISRFun( ADC_pvidISRParameter );
	}
}

