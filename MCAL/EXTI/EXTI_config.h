/*
 * EXTI_config.h
 *
 *  Created on: Mar 16, 2022
 *      Author: abdelhai Tahoon
 */

#ifndef MCAL_EXTI_EXTI_CONFIG_H_
#define MCAL_EXTI_EXTI_CONFIG_H_

#define EXTI_NUM		3

typedef struct
{
	u8 senseLvl;
	u8 intMode;
	u8 intNum;
}EXTI_t;

#endif /* MCAL_EXTI_EXTI_CONFIG_H_ */
