/*
 * ADC_int.h
 *
 *  Created on: Oct 23, 2021
 *      Author: abdelhai Tahoon
 */

#ifndef MCAL_ADC_ADC_INT_H_
#define MCAL_ADC_ADC_INT_H_

ES_t ADC_enuInit(void);

ES_t ADC_enuStartConversion(void);

ES_t ADC_enuReadHighRegister(u8 * Copy_pu8ReadValue);

ES_t ADC_enuRead(u16 * Copy_pu16ReadValue);

ES_t ADC_enuCallBack(void (*Copy_pfunAppFun)(void*) , void * Copy_pvidAppParameter);

ES_t ADC_enuSelectChannel(u8 Copy_u8ChannelID);

ES_t ADC_enuEnableAutoTrigger(u8 Copy_u8IntSource);

ES_t ADC_enuDisableAutoTrigger(void);

ES_t ADC_enuPolling(void);

ES_t ADC_enuClearFlag(void);

ES_t ADC_enuEnableADC(void);

ES_t ADC_enuDisableADC(void);

ES_t ADC_enuEnableADC_INT(void);

ES_t ADC_enuDisableADC_INT(void);


#endif /* MCAL_ADC_ADC_INT_H_ */
