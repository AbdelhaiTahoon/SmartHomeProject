/*
 * EXTI_priv.h
 *
 *  Created on: Mar 16, 2022
 *      Author: abdelhai Tahoon
 */

#ifndef MCAL_EXTI_EXTI_PRIV_H_
#define MCAL_EXTI_EXTI_PRIV_H_

#define LOW_LVL			65
#define ANY_LOGICAL		32
#define FALLING			97
#define RISING			67

#define INT0			0
#define INT1			1
#define INT2			2


#define ENABLE			37
#define DISABLE			10


#define MCUCR			*((volatile u8*)0x55)
#define MCUCSR			*((volatile u8*)0x54)
#define GICR			*((volatile u8*)0x5B)
#define GIFR			*((volatile u8*)0x5A)

#endif /* MCAL_EXTI_EXTI_PRIV_H_ */
