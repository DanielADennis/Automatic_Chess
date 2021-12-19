#ifndef __STM32L476R_NUCLEO_STOPWATCH_H
#define __STM32L476R_NUCLEO_STOPWATCH_H
#include <stdbool.h>
#include <stdint.h>


	
	void SysTick_Initialize(uint32_t ticks);
	void SysTick_Handler(void);
	void EXTI_Init(void);
	void EXTI0_IRQHandler(void);
	void EXTI1_IRQHandler(void);
	void EXTI2_IRQHandler(void);


#endif
