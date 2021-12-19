#include "stm32l476xx.h"
#include "LCD.h"
#include <stdbool.h>
#include "KYPD.h"
#include "MotoStoke.h"
/*
//TESTING MAIN
int main(void){
	LCD_Init();
	LCD_Clear();
	LCD_Clear();
	LCD_Clear();
	init_KYPD();
	mag_Init();
	motoStopper_init();
	LCD_DisplayString(0, (unsigned char*)"Ready when");
  LCD_DisplayString(1, (unsigned char*)"you are");
	motorInit_H();
	motorInit_V();
	//SysTick_Initialize(16000);
//	magnetEN();
//	magnetDIS();
	goToZero();
	move_V(2);
	magnetEN();
	move_V(2);
//	move_H(1);
//	goToZero();
		//move_V(-4);
	//	move_V(-2, 15);
	//	move_H(places_k,15);
	//move_H(-places_k,15);
	
//	magnetDIS();

	
	while(1);
}
*/
//FUNCTIONAL MAIN
int main(void){
	LCD_Init();
	LCD_Clear();
	LCD_Clear();
	LCD_Clear();
	init_KYPD();
	chessBoardInit();
	motoStopper_init();
	motorInit_H();
	motorInit_V();
	mag_Init();
	LCD_DisplayString(0, (unsigned char*)"Going to");
  LCD_DisplayString(1, (unsigned char*)"zero (A1)");
	goToZero();
	LCD_Clear();
	LCD_DisplayString(0, (unsigned char*)"Ready when");
  LCD_DisplayString(1, (unsigned char*)"you are");
	int state = 3;
	unsigned int input;
	int row_from = 0;
	int column_from =0;
	int row_to = 0;
	int column_to = 0;
	int v_pos = 1;
	int h_pos = 1;
	LCD_WriteCom(0x80);
	
	while(1){
		switch(state){
			case 0://Getting input for PVP
				if((row_from == 0) | (column_from ==0)){// We have to determine where the source of the move is first
						
					input = kypd_in();//runs until a button is pressed and released
					if(((input > 4)&&(input < 9))||(input>12)){// if the input we get is less than 9 then we know it is a column number 
						LCD_Clear();
						LCD_WriteData(to_LCD(input));
						column_from = only_Numb(input);
					}
					else{// else its a row number
						LCD_Clear();
						LCD_WriteData(to_LCD(input));
						row_from = only_Numb(input);
					}
					if((row_from != 0) &(column_from !=0)){// if we have now obtained both a row and a column we are in a transition stage
						LCD_Clear();
						LCD_WriteCom(0x80);
						LCD_WriteData((unsigned char)('@' + column_from));// now display the final values for the row and column
						LCD_WriteData((unsigned char)('0'+ row_from));
					}
				}
				else{//then we need to find the destination of the piece
					
					input = kypd_in();
						if(((input > 4)&&(input < 9))||(input>12)){
							LCD_Clear();
							LCD_WriteCom(0x80);
							LCD_WriteData((unsigned char)('@' + column_from));
							LCD_WriteData((unsigned char)('0'+ row_from));
							LCD_WriteCom(0x80+0x40);
							LCD_WriteData(to_LCD(input));
							column_to = only_Numb(input);
						}
						else{
							LCD_Clear();
							LCD_WriteCom(0x80);
							LCD_WriteData((unsigned char)('@' + column_from));
							LCD_WriteData((unsigned char)('0'+ row_from));
							LCD_WriteCom(0x80+0x40);
							LCD_WriteData(to_LCD(input));
							row_to = only_Numb(input);
						}
						if((row_to != 0) &(column_to !=0)){// if we have recieved both destination coordinates then we will transition to the next state
								LCD_Clear();
								LCD_WriteCom(0x80);
								LCD_WriteData((unsigned char)('@' + column_from));
								LCD_WriteData((unsigned char)('0'+ row_from));
								LCD_WriteCom(0x80+0x40);
								LCD_WriteData((unsigned char)('@' + column_to));
								LCD_WriteData((unsigned char)('0'+ row_to));
								state = 1;
						}
				}
			break;
			case 1:
				//move Piece
				//first goto source position.
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to);
				state=2;
			break;
			case 2://reset inputs to 0
				v_pos = column_to;
				h_pos = row_to;
				row_from = 0;
				column_from =0;
				row_to = 0;
				column_to = 0;
				state = 0;
				LCD_Clear();
				LCD_DisplayString(0, (unsigned char*)"Ready when");
				LCD_DisplayString(1, (unsigned char*)"you are");
			break;

			default: state = 0;// just in case
			
			case 3: //menu selection
				LCD_Clear();
				LCD_WriteCom(0x80);
				LCD_WriteData("1: PVP");
				LCD_WriteCom(0x80+0x40);
				LCD_WriteData("2: Auto Playback");
				input = kypd_in();//runs until a button is pressed and released
				if(input == 1){// if the button pressed is "1" then do the go to the PVP case. ASSUMING 1 cooresponds to 1
					LCD_Clear();
					state = 0;
				}
				else if(input == 2){ // if the button pressed is "2" then do the go to the Autoplay case. ASSUMING 2 cooresponds to 2
					LCD_Clear();
					state = 4;
				}
				else{
					state = 3;
				}
				
			case 4: //Auto Playback
				LCD_Clear();
				LCD_WriteCom(0x80);
				LCD_WriteData('Lasker vs.Thomas');	
				LCD_WriteCom(0x80+0x40);
				LCD_WriteData('1912');
		
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //d2 d4
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //e7 e6
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //g1 f3
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //f7 f5
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //b1 c3
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //g8 f6
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //c1 g5
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //f8 e7
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //f6 g5
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //e7 f6
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //e2 e4
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //f5 e4
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //c3 e4
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //b7 b6
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //f3 e5
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //e8 g8
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //h8 f8
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //f1 d3
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //c8 b7
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //d1 h5
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //d8 e7
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //h5 h7
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //g8 h7
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //e4 f6
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //h7 h6
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //e5 g4
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //h6 g5
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //h2 h4
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //g5 f4
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //g2 g3
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //f4 f3
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //d3 e2
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //f3 g2
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //h1 h2
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //g2 g1
				movePiece(&v_pos,&h_pos, column_from, column_to, row_from, row_to); //e1 d2
		
				state = 3;
				
		}
			
		
		
		
		
	}
	
}


