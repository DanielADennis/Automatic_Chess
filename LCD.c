#include "LCD.h"
#include "stm32l476xx.h"


/*
delay_ms 
waits for ms milliseconds
Use a dummy variable arithmetic
that is pointless, but allows  it from not
just optimizing away the wait function
*/
void delay_ms(unsigned int ms) {
	volatile unsigned int i,j;

	for(i=0;i<ms;i++){
			for(j=0; j<20;j++){
			}
	}			
}

/*
LCD_WriteCom(com)//writes Commands in com to the LCD
switch rs to 0 to put in command mode
split 8 bit commmand into 2, 4 bit commands
Turn off enable
write com1 to LCD
turn on enable
waitms()
turn off enable
write com2 to LCD
turn on enable

*/
// Enable = PA1 
// RS == PA0
void LCD_WriteCom(unsigned char com) {
	unsigned char com1,com2; 
	com1 = com & 0xf0;
	com2 = (com <<4)&0xf0;
	//sending com1
	GPIOA->ODR &= 0xFFFC;// enable and rs -> 0
	GPIOA->ODR &= 0xFF0F; //clear bits 4567
	GPIOA->ODR |= com1; //com1 -> 4567
	GPIOA->ODR |= 0x2; // enable -> 1
	delay_ms(500);//wait a little
	//sending com2
	GPIOA->ODR &= 0xFFFC;// enable and rs -> 0
	GPIOA->ODR &= 0xFF0F; //clear bits 4567
	GPIOA->ODR |= com2; //com2 -> 4567
	GPIOA->ODR |= 0x2; // enable -> 1
	delay_ms(500);//wait a little
	
	GPIOA->ODR &= 0xFFFC;// enable and rs -> 0
	
	
}
/*
LCD_WriteData(dat) //writes data in dat to the LCD
switch rs to 1 to put in data mode
split 8 bit data to 2, 4 bit datas
write dat1 to LCD
write dat2 to LCD

*/
void LCD_WriteData(unsigned char dat) {
	 unsigned char data1,data2;
	data1 = dat & 0xf0;
	data2 = (dat <<4) & 0xf0;
	//sending dat1
	GPIOA->ODR &= 0xFFFC;// enable -> 0 and rs -> 0
	GPIOA->ODR |= 0x1; // rs -> 1
	GPIOA->ODR &= 0xFF0F; //clear bits 4567
	GPIOA->ODR |= data1; //dat1 -> 4567
	GPIOA->ODR |= 0x2; // enable -> 1
	delay_ms(50);//wait a little
	//sending dat2
	GPIOA->ODR &= 0xFFFC;// enable -> 0 and rs -> 0
	GPIOA->ODR |= 0x1; // rs -> 1
	GPIOA->ODR &= 0xFF0F; //clear bits 4567
	GPIOA->ODR |= data2; //dat2 -> 4567
	GPIOA->ODR |= 0x2; // enable -> 1
	delay_ms(50);//wait a little
	
	GPIOA->ODR &= 0xFFFC;// enable and rs -> 0
}

/*
LCD Initialize
enable the HSI clock
enables GPIOA clock 
Configure Ports 4,5,6,7 as Outputs
Configure Ports 0, 1 as Outputs
Send to 4567 the following commands, using Write_Com:
;4bit bus, 2-line display mode, 5x8 dot matrix
;display on, cursor moves left-to-right
;no display scrolling
;clear display
;set up CGRAM address to start at 0
*/
void LCD_Init(void){
	delay_ms(10000);
	//enable the HSI clock
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);
	
	//enable clock to GPIOA
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	//Configure Ports 4,5,6,7 as Outputs
	GPIOA->MODER &= 0xFFFF00FF;
	GPIOA->MODER |= 0x00005500;
	
	//Configure Ports 0, 1 as Outputs
	GPIOA->MODER &= 0xFFFFFFF0;
	GPIOA->MODER |= 0x5;
	
	//write commands to LCD
	LCD_WriteCom(0x28); // 4bit bus, 2 line display mode, 5x8 dot matrix
	LCD_WriteCom(0x0C); // display on, no cursor normally C
	LCD_WriteCom(0x06); // cusor moves left-to-right, no display scrolling
	LCD_WriteCom(0x01); // clear display
	LCD_WriteCom(0x40); // set up CGRAM address to start at 0
	
}

/*
LCD_Clear()
using LCD_WriteCom() send clear commmand

*/
void LCD_Clear(void){
		delay_ms(100);
  	LCD_WriteCom(0x1); // clear display
		delay_ms(100);
}

/*
LCD_DisplayString(line,*ptr)

if(line ==0)
	LCD_WriteData at ptr to line 0
if(line ==1)
	LCD_WriteData at ptr to line 1

*/
void LCD_DisplayString(unsigned int line, unsigned char *ptr) {
	int i;
	
	if(line == 0){
		LCD_WriteCom(0x80);
	}
	if(line == 1) {
		LCD_WriteCom(0x80+0x40);
		
	}
	for(i=0;ptr[i] != '\0'; i++){
		LCD_WriteData(ptr[i]);
	}
	
}
