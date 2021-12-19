#include "StopWatch.h"
#include "stm32l476xx.h"
#include "LCD.h"


  static int state = 0;
	static int decisecs = 0;
	static int sec = 0;
	static int min = 0;
	static bool rset = 1;
	static bool start = 0;
	static bool pause = 0;

	void SysTick_Initialize(uint32_t ticks){
		//Disable Systick IRQ and Systick counter
		SysTick->CTRL = 0;
	
		// set reload register
		SysTick->LOAD = ticks - 1;
		
		//set interupt priority of SysTick 
		//Make SysTick least urgent
		//__NVIC_PRIO_BITS: number of bits for priority levels, defined in CMSIS
		NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS)-1);
		
		//reset the SysTick counter value
		SysTick->VAL = 0;
		
		//Select processor clock
		//1=processor clock; 0 = external clock
		SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
		
		//Enables SysTick except request
		//1 = counting down to zero asserts the systick exception request
		//0 = couting down to zero does not assert the Systick exception request
		SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
		
		//enable Systick timer
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	}
	void SysTick_Handler(void){
		switch(state){//using a state machine to software debounce the btns
				case 0://Waiting
					if(rset){
						decisecs =0;
						sec=0;
						min=0;
						unsigned char toPrint[8] = {(min/10)+'0', (min%10)+'0', ':', (sec/10)+'0', (sec%10)+'0', '.', decisecs+'0', 0};
						LCD_DisplayString(0,toPrint);
					}
					rset = 0;
					if(start){
						state = 1;
					}
					
					break;
					
				case 1://Running
					start = 0;
					
						decisecs++;
						if(decisecs >9){
							sec++;
							decisecs = 0;
							if(sec > 59){
								min++;
								sec =0;
							}
						}
						unsigned char toPrint[8] = {(min/10)+'0', (min%10)+'0', ':', (sec/10)+'0', (sec%10)+'0', '.', decisecs+'0', 0};
						LCD_DisplayString(0, toPrint);
						
					
					
					if(rset){
						state = 0;
					}
					if(pause){
						state = 2;
					}
					
					break;
				
				case 2://Pause
					pause = 0;
					if(rset){
						state = 0;
					}
					if(start){
						state = 1;
					}
					break;
				
			}	
		//waiting state	
	
	//running state
		
		
	//reset state
	
	}
	
	void EXTI_Init(void){
		//enable SYSCFG clock
		RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
		
		//Enable the clock for port B
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
		
		//Configure Ports 0,1,2 as Inputs
		GPIOB->MODER &= 0xFFFFFFC0;
		GPIOB->PUPDR &= 0xFFFFFFC0;
		GPIOB->PUPDR |= 0xFFFFFF2A;
		
		//Select PB.0 as the trigger source of EXTI 0
		SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
		SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB;
		SYSCFG->EXTICR[0] &= ~(0x000Eu);
		
		//Select PB.1 as the trigger source of EXTI 1
		SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI1;
		SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PB;
		SYSCFG->EXTICR[0] &= ~(0x00E0u);
		
		//Select PB.2 as the trigger source of EXTI 2
		SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI2;
		SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PB;
		SYSCFG->EXTICR[0] &= ~(0x0E00u);
		
		//Enable rising edge trigger for EXTI 0, EXTI 1, EXTI2
		//Rising trigger selection register (RSTR)
		//0=disabled, 1= enabled
		EXTI->RTSR1 |= EXTI_RTSR1_RT0;
		EXTI->RTSR1 |= EXTI_RTSR1_RT1;
		EXTI->RTSR1 |= EXTI_RTSR1_RT2;

		//Disable falling edge trigger for EXTI0, EXTI1, EXTI2
		//Falling trigger selection register (FSTR)
		//0: disable; 1: enabled
		EXTI->FTSR1 &= ~EXTI_FTSR1_FT0;
		EXTI->FTSR1 &= ~EXTI_FTSR1_FT1;
		EXTI->FTSR1 &= ~EXTI_FTSR1_FT2;
		
		//Enable EXTI 0 interupt
		//INterrupt mask register: 0 = masked, 1= unmasked
		//Masked means that processor ignores the corresponding interupt.
		EXTI->IMR1 |= EXTI_IMR1_IM0;// Enable EXTI line 0
		EXTI->IMR1 |= EXTI_IMR1_IM1;// Enable EXTI line 0
		EXTI->IMR1 |= EXTI_IMR1_IM2;// Enable EXTI line 0
		
		//Set EXTI 0 priority to 1
		NVIC_SetPriority(EXTI0_IRQn,1);
		//Set EXTI 1 priority to 1
		NVIC_SetPriority(EXTI1_IRQn,1);
		//Set EXTI 2 priority to 1
		NVIC_SetPriority(EXTI2_IRQn,1);
		
		//Enable EXTI 0 interrupt
		NVIC_EnableIRQ(EXTI0_IRQn);
		//Enable EXTI 1 interrupt
		NVIC_EnableIRQ(EXTI1_IRQn);
		//Enable EXTI 2 interrupt
		NVIC_EnableIRQ(EXTI2_IRQn);
		
		
		
	}
	//Reset btn
	void EXTI0_IRQHandler(void){
		//Check for EXTI 0 interrupt flag
		if((EXTI->PR1 & EXTI_PR1_PIF0) == EXTI_PR1_PIF0){
			rset = 1;
		}
		
		//clear interrupt pending request
		EXTI->PR1 |= EXTI_PR1_PIF0;
	}
	//Pause btn
	void EXTI1_IRQHandler(void){
		//Check for EXTI 0 interrupt flag
		if((EXTI->PR1 & EXTI_PR1_PIF1) == EXTI_PR1_PIF1){
			pause = 1;
		}
		
		//clear interrupt pending request
		EXTI->PR1 |= EXTI_PR1_PIF1;
	}
	//Start btn
	void EXTI2_IRQHandler(void){
				//Check for EXTI 0 interrupt flag
		if((EXTI->PR1 & EXTI_PR1_PIF2) == EXTI_PR1_PIF2){
			start = 1;
		}
		
		//clear interrupt pending request
		EXTI->PR1 |= EXTI_PR1_PIF2;
		
	
	}
	
	
