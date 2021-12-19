#include "MotoStoke.h"
#include "stm32l476xx.h"
#include <stdbool.h>
#include "LCD.h"
static const int places_k = 600;
//static const int duty=21;
//static int cycle=0;
static const int speed =50;
static volatile bool chessBoard[8][8];//[row][column]
static volatile bool hit_wall_H =0;
static volatile bool hit_wall_V =0;
//static bool turn =1;
//static volatile bool magFlag = 0; //0 for off 1 for on

static bool deadSpaceT[16];
//static bool deadSpaceB[16];
void mag_Init(void){
	GPIOC->MODER &= 0xFFFCFFFF;
	GPIOC->MODER |= (0x1<<16);
	
	
}
void goToZero(void){
	move_V(-10);
	move_H(-10);
	resetMotor_H();
	resetMotor_V();
}

void chessBoardInit(void){//initialize boolboard
	for(int i =0; i <8; i++){
		for(int j = 0; j<8; j++){
			if((j < 2) || (j> 5)){
				chessBoard[i][j] = 1;
			}
			else{
				chessBoard[i][j] = 0;
			}
		}
			
	}
	
	clrDeadSpace();
	
}
void clrDeadSpace(void){
	for( int i =0; i<16; i++){//clear the deadSpace. Nothings dead from the get go. 
			deadSpaceT[i] = 0;
	}
	
}
//interupts are PC5, PC6
void motoStopper_init(void){
		//Enable the clock for port C
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
		//enable SYSCFG clock
		RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
		//Configure Ports 5,6 as Inputs
		GPIOC->MODER &= 0xFFFFC3FF;
		GPIOC->PUPDR &= 0xFFFFC3FF;
		GPIOC->PUPDR |= 0x00002800;
		//Select PC.5 as the trigger source of EXTI 5
		SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI5;
		SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PC;
		SYSCFG->EXTICR[1] &= ~(0x00F0u);
		SYSCFG->EXTICR[1] |= 0x0020u;
		//Select PC.6 as the trigger source of EXTI 6
		SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI6;
		SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PC;
		SYSCFG->EXTICR[1] &= ~(0x0F00u);
		SYSCFG->EXTICR[1] |= 0x0200u;
		//Enable rising edge trigger for EXTI_5, EXTI_6, 
		//Rising trigger selection register (RSTR)
		//0=disabled, 1= enabled
		EXTI->RTSR1 |= EXTI_RTSR1_RT5;
		EXTI->RTSR1 |= EXTI_RTSR1_RT6;
		//Disable falling edge trigger for EXTI_5, EXTI_6
		//Falling trigger selection register (FSTR)
		//0: disable; 1: enabled
		EXTI->FTSR1 &= ~EXTI_FTSR1_FT5;
		EXTI->FTSR1 &= ~EXTI_FTSR1_FT6;
		//Enable EXTI 5 & 6 interupt
		//INterrupt mask register: 0 = masked, 1= unmasked
		//Masked means that processor ignores the corresponding interupt.
		EXTI->IMR1 |= EXTI_IMR1_IM5;// Enable EXTI line 5
		EXTI->IMR1 |= EXTI_IMR1_IM6;// Enable EXTI line 6
		//Set EXTI 5 priority to 1
		NVIC_SetPriority(EXTI9_5_IRQn,1);
		//Set EXTI 6 priority to 1
		NVIC_SetPriority((EXTI9_5_IRQn>>1),1);
		//Enable EXTI 5 interrupt
		NVIC_EnableIRQ(EXTI9_5_IRQn);
		//Enable EXTI 6 interrupt
		NVIC_EnableIRQ((EXTI9_5_IRQn>>1));
	}

void EXTI9_5_IRQHandler(void){
		//Check for EXTI 5 interrupt flag

		if((GPIOC->IDR & (0x1<<6))>>6){
				hit_wall_V = 1;
				delay_ms2(2400);
			}
		//clear interrupt pending request
		EXTI->PR1 |= EXTI_PR1_PIF5;
		
		//Check for EXTI 6 interrupt flag

			if((GPIOC->IDR & (0x1<<5))>>5){
				hit_wall_H =1;
				
			}
		//clear interrupt pending request
		EXTI->PR1 |= EXTI_PR1_PIF6;
}


/*
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
*/
/*
//generates a PWM for the electromagnet
	void SysTick_Handler(void){
		if(magFlag){
			cycle++;
			if(cycle > 2){
					GPIOC->ODR &= ~(0x1u)<<8;
					if(cycle > 19){
						cycle = 0;
					}
			}
			else{
					GPIOC->ODR |= (0x1<<8);
			}
		}
		else{
			if(cycle > 1){
					GPIOC->ODR &= ~(0x1u)<<8;
					if(cycle > 19){
						cycle = 0;
					}
			}
			else{
					GPIOC->ODR |= (0x1<<8);
			}
		}
	}
*/

//PC8 for electromagnet.
//electomagnet is interupt driven like SysTick
void magnetEN(void){
	GPIOC->ODR |= (0x1<<8);
//	magFlag =1;
}
void magnetDIS(void){
	GPIOC->ODR &= ~(0x1u)<<8;
	//magFlag=0;
}
	
//electromagnet on with SysTick
//it takes a square wave to turn on.
//have conditional statement with the magFlag
//when magFlag = 1 input square wave into the PC8 and dont when magFlag =0

//global flag to determine if magnet is off or on. 

/*
delay_ms 
waits for ms milliseconds
Use a dummy variable arithmetic
that is pointless, but allows  it from not
just optimizing away the wait function
*/
void delay_ms2(unsigned int ms) {
	volatile unsigned int i,j;

	for(i=0;i<ms;i++){
			for(j=0; j<20;j++){
			}
	}			
}
void resetMotor_H(){
	
	GPIOC->ODR &= 0xFF7F;
	GPIOA->ODR &= 0xFEFF;
	GPIOA->ODR &= 0xFDFF;
	GPIOA->ODR &= 0xFBFF;
}
void resetMotor_V(){
			GPIOB->ODR &= 0xFDFF;
			GPIOB->ODR &= 0xFFDF;
			GPIOB->ODR &= 0xFFBF;
			GPIOB->ODR &= 0xFEFF;
}
//Enable Clock and GPIO pins needed for motor
// PA 8, PA 9, PA 10, PC 7
void motorInit_H(){
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
		//Configure Pins  PA 8, PA 9, PA 10, PC 7 as Outputs
		GPIOA->MODER &= 0xFFC0FFFF;
		GPIOC->MODER &= 0xFFFF3FFF;
	
		GPIOA->MODER |= 0x00150000;
		GPIOC->MODER |= 0x00004000;
	
}
void motorInit_V(){
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	
		//Configure Pins  PB5, PB6, PB8, PB9 as Outputs
		GPIOB->MODER &= 0xFFF0C3FF;
		GPIOB->MODER |= 0x00051400;

}
//PA 8, PA 9, PA 10, PC 7
void moveH_Help(int direction){
	if(direction > 0){
		
			GPIOA->ODR |= 0x0100;
			GPIOC->ODR &= 0xFF7F;
			delay_ms2(speed);
		
		
			GPIOA->ODR |= 0x0200;
			GPIOA->ODR &= 0xFEFF;
			delay_ms2(speed);
		
		
			GPIOA->ODR |= 0x0400;
			GPIOA->ODR &= 0xFDFF;
			delay_ms2(speed);
		
		
			GPIOC->ODR |= 0x0080;
			GPIOA->ODR &= 0xFBFF;
			delay_ms2(speed);
		
	}
	else{
			
			GPIOC->ODR |= 0x0080;
			GPIOA->ODR &= 0xFBFF;
			delay_ms2(speed);
		
		
			GPIOA->ODR |= 0x0400;
			GPIOA->ODR &= 0xFDFF;
			delay_ms2(speed);
		
		
			GPIOA->ODR |= 0x0200;
			GPIOA->ODR &= 0xFEFF;
			delay_ms2(speed);
		
		
			GPIOA->ODR |= 0x0100;
			GPIOC->ODR &= 0xFF7F;
			delay_ms2(speed);
		

	}
}
//PB5, PB6, PB8, PB9
void moveV_Help(int direction){
	if(direction < 0){
		
			GPIOB->ODR |= 0x0020;
			GPIOB->ODR &= 0xFDFF;
			delay_ms2(speed);
	
			GPIOB->ODR |= 0x0040;
			GPIOB->ODR &= 0xFFDF;
			delay_ms2(speed);

			GPIOB->ODR |= 0x0100;
			GPIOB->ODR &= 0xFFBF;
			delay_ms2(speed);

			GPIOB->ODR |= 0x0200;
			GPIOB->ODR &= 0xFEFF;
			delay_ms2(speed);

	}
	else{

			GPIOB->ODR |= 0x0200;
			GPIOB->ODR &= 0xFEFF;
			delay_ms2(speed);

			GPIOB->ODR |= 0x0100;
			GPIOB->ODR &= 0xFFBF;
			delay_ms2(speed);

			GPIOB->ODR |= 0x0040;
			GPIOB->ODR &= 0xFFDF;
			delay_ms2(speed);

			GPIOB->ODR |= 0x0020;
			GPIOB->ODR &= 0xFDFF;
			delay_ms2(speed);

	}



}

void move_V(int places){
	int upper;
	hit_wall_V=0;
	int drctn;
	if(places >0){
		upper = places;
		drctn = 1;
	}
	else{
		upper = -1*(places);
		drctn = -1;
	}
	for( int j = 0; j< places_k;j++){		
		for( int i = 0; (i < upper); i++){
			if(hit_wall_V){
				i = upper;
				j = places_k;
				delay_ms2(240);
			}
			else{
				moveV_Help(drctn);
			}
		}
	}
	

	while(hit_wall_V){
		if(!((GPIOC->IDR & (0x1<<6))>>6)){
			hit_wall_V = 0;
		}
		for( int j = 0; j< places_k/5;j++){		
			moveV_Help(-drctn);	
		}
	}
	
	resetMotor_H();
	resetMotor_V();
}
void move_H(int places){
	int upper;
	int drctn;
	hit_wall_H =0;
	if(places >0){
		upper = places;
		drctn =1;
	}
	else{
		upper = -1*(places);
		drctn = -1;
	}
	//offset
	
	//move piece
	for(int j =0; j < places_k; j++){//moves one space
		for( int i = 0; (i < upper) ; i++){//moves one space "upper" times
			
			if(hit_wall_H){
				i = upper;
				delay_ms2(100);
				j= places_k;
			}
			else{
				moveH_Help(drctn);	
			}
		}
	}
	//interupt corrections
	while(hit_wall_H){
		if(!((GPIOC->IDR & (0x1<<5))>>5)){
			hit_wall_H = 0;
		}
		for( int j = 0; j< places_k/3;j++){		
			moveH_Help(-drctn);	
		}
	}
	
	
	resetMotor_V();
	resetMotor_H();
}

void killPiece(int v_pos, int h_pos, int v_to, int h_to){
	//move piece at [v_to][h_to] to deadspace and come back
	LCD_Clear();
	LCD_DisplayString(0, (unsigned char*)"Killing a");
  LCD_DisplayString(1, (unsigned char*)"piece");
	int deadex=0;
	
	
		
		for(int i = 0; i < 16; i++){
			
				if(!(deadSpaceT[i])){
					deadex = i+1;
					i = 16;
					deadSpaceT[i] = 1;
				}
			
		}
		if(deadex ==0){
			clrDeadSpace();
			for(int i = 0; i < 16; i++){
				if(!(deadSpaceT[i])){
					deadex = i+1;
					i = 16;
					deadSpaceT[i] = 1;
				}
			}
		}
		
		//move magnet to dead piece
		move_H(v_to-v_pos);
		move_V(h_to-h_pos);
		//turn on magnet
		magnetEN();
		
			//move piece to deadSpace
			
			offset_H(-1);
			move_V(h_to-(deadex/2));
			offset_V(-1);
			move_H(-v_to);
			offset_H(-1);
			if(deadex%2==0){
				offset_V(1);
			}
		
	
	
	
	magnetDIS();
	//move magnet back to orignal position
	goToZero();
	move_V(v_pos-1);
	move_H(h_pos-1);
}

void movePiece(int *v_pos, int *h_pos, int v_from, int v_to, int h_from, int h_to){//pos past by reference to update in the function
	// It takes too long per move as is. 
	// check for killed piece
	if(chessBoard[h_to][v_to]){
		//handle that here
		killPiece(*v_pos, *h_pos, v_to, h_to);
	}
	LCD_Clear();
	LCD_DisplayString(0, (unsigned char*)"Going to");
  LCD_DisplayString(1, (unsigned char*)"grab piece");
	//move from vertically
	move_H(v_from-(*v_pos));
	//move from horizontally
	move_V(h_from-(*h_pos));
	//turn on electromagnet
	*v_pos = v_from;
	*h_pos = h_from;
	magnetEN();
	LCD_DisplayString(0, (unsigned char*)"Moving piece");
  LCD_DisplayString(1, (unsigned char*)"to destination");
	//moveTo vertically
	if(v_to-(*v_pos)!= 0){
		offset_V(1);
		move_H(v_to-(*v_pos));
	}
	//moveTo horizontally
	if(h_to-(*h_pos) != 0){
		offset_H(1);
		move_V(h_to-(*h_pos));
	}
	if(v_to-(*v_pos)!= 0){
		offset_V(-1);
	}
	if(h_to-(*h_pos) != 0){
		offset_H(-1);
	}
	magnetDIS();
	
	//update virtual bool board
	chessBoard[v_from-1][h_from-1] = 0;
	chessBoard[v_to-1][h_to-1] = 1;
	
}

void offset_V(int dir){
	for(int i =0; i< (places_k/2); i++){
		moveV_Help(dir);//offset H to move Vertical
	}
	resetMotor_V();
}
void offset_H(int dir){
	for(int i =0; i< (places_k/2); i++){
		moveH_Help(dir);//offset H to move Vertical
	}
	resetMotor_H();
	
}
