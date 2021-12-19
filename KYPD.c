#include "KYPD.h"
#include "LCD.h"
#include "stm32l476xx.h"

void init_KYPD(void){
	//Initialize Clocks for GPIO Ports B&C
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	
	//Configure Pins C0, C1, C2, C3 as Inputs
	GPIOC->MODER &= 0xFFFFFF00;
	//Configure Pins B0, B1, B2, B3 as Outputs, set as open drain
	GPIOB->MODER &= 0xFFFFFF00;
	GPIOB->MODER |= 0x00000055;
	
	GPIOB->OTYPER |=0xF;
	
	
}

//Rows are B ports Columns are C ports
unsigned int check_KYPD(void){
	//Check row 1
	GPIOB->ODR &= 0xFFFFFFF0;
	GPIOB->ODR |= 0xE;
	delay_ms(50);
		// column 1
		//	int a = (GPIOC->IDR & 0xE);
			if(GPIOC->IDR == 0xE){
				return 1;
			}
		// column 2
		//	a = (GPIOC->IDR & 0xD);
			if(GPIOC->IDR == 0xD){
				return 5;
			}
		// column 3
		//	a = (GPIOC->IDR & 0xB);
			if(GPIOC->IDR == 0xB){
				return 9;
			}
		// column 4
		//	a = (GPIOC->IDR & 0x7);
			if(GPIOC->IDR == 0x7){
				return 13;
			}
	//Check row 2 
	GPIOB->ODR &= 0xFFFFFFF0;
	GPIOB->ODR |= 0xD;
			delay_ms(50);
		// column 1
			//a = (GPIOC->IDR & 0xE);
			if(GPIOC->IDR == 0xE){
				return 2;
			}
		// column 2
			//a = (GPIOC->IDR & 0xD);
			if(GPIOC->IDR == 0xD){
				return 6;
			}
		// column 3
			//a = (GPIOC->IDR & 0xB);
			if(GPIOC->IDR == 0xB){
				return 10;
			}
		// column 4
			//a = (GPIOC->IDR & 0x7);
			if(GPIOC->IDR == 0x7){
				return 14;
			}
	
	//Check row 3 
	GPIOB->ODR &= 0xFFFFFFF0;
	GPIOB->ODR |= 0xB;	
	delay_ms(50);
		// column 1
			//a = (GPIOC->IDR & 0xE);
			if(GPIOC->IDR == 0xE){
				return 3;
			}
		// column 2
			//a = (GPIOC->IDR & 0xD);
			if(GPIOC->IDR == 0xD){
				return 7;
			}
		// column 3
			//a = (GPIOC->IDR & 0xB);
			if(GPIOC->IDR == 0xB){
				return 11;
			}
		// column 4
			//a = (GPIOC->IDR & 0x7);
			if(GPIOC->IDR == 0x7){
				return 15;
			}
	
	//Check row 4 
	GPIOB->ODR &= 0xFFFFFFF0;
	GPIOB->ODR |= 0x7;
	delay_ms(50);
		// column 1
			//a = (GPIOC->IDR & 0xE);
			if(GPIOC->IDR == 0xE){
				return 4;
			}
		// column 2
			//a = (GPIOC->IDR & 0xD);
			if(GPIOC->IDR == 0xD){
				return 8;
			}
		// column 3
			//a = (GPIOC->IDR & 0xB);
			if(GPIOC->IDR == 0xB){
				return 12;
			}
		// column 4
			//a = (GPIOC->IDR & 0x7);
			if(GPIOC->IDR == 0x7){
				return 16;
			}
return 0;
}

unsigned char to_LCD(unsigned int a){
	volatile unsigned char r[16] = {'5','6','7','8','A', 'B', 'C', 'D', '1', '2', '3', '4', 'E', 'F', 'G', 'H'};
	volatile unsigned char temp = r[a-1];
	return temp; 
	
}
int only_Numb(unsigned int a){
	volatile int r[16] = {5,6,7,8,1, 2, 3, 4, 1, 2, 3, 4, 5, 6, 7, 8};
	volatile int temp = r[a-1];
	return temp; 
	
}
unsigned int kypd_in(){
	unsigned int a = 0;
		while(a==0){//Waits until button is pressed and stores it in 'a'
		
			a = check_KYPD();
			
			delay_ms(50);
		}
		unsigned int b = a;
		while(b == a){//waits until btn is let go to continue
			b = check_KYPD();
			delay_ms(50);
		}
		return a;
}
