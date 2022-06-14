/*
 * EXTI_int.h
 *
 *  Created on: Mar 16, 2022
 *      Author: abdelhai Tahoon
 */

#ifndef MCAL_EXTI_EXTI_INT_H_
#define MCAL_EXTI_EXTI_INT_H_

ES_t EXTI_enuInit(EXTI_t * Copy_pAstrEXTIConfig);

ES_t EXTI_enuSetSenseLvl(EXTI_t * Copy_pstrEXTIInfo, u8 Copy_u8SenseLvl);

ES_t EXTI_enuEnableInt(EXTI_t * Copy_pstrEXTIInfo);

ES_t EXTI_enuDisbleInt(EXTI_t * Copy_pstrEXTIInfo);

ES_t EXTI_enuCallBack(EXTI_t * Copy_pstrEXTIInfo, void (*Copy_pfunAppFun)(void*) , void * Copy_pvidAppParameter);

#endif /* MCAL_EXTI_EXTI_INT_H_ */
