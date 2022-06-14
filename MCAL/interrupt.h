/*
 * interrupt.h
 *
 *  Created on: Mar 16, 2022
 *      Author: abdelhai Tahoon
 */

#ifndef MCAL_INTERRUPT_H_
#define MCAL_INTERRUPT_H_

#define ISR(vect_num)		void vect_num(void)__attribute__((signal));\
							void vect_num(void)

#define INT0_vect			__vector_1
#define INT1_vect			__vector_2
#define INT2_vect			__vector_3

#define ADC_vect 			__vector_16

#endif /* MCAL_INTERRUPT_H_ */
