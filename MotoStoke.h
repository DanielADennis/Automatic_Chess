#ifndef __STM32L476R_NUCLEO_MOTOSTOKE_H
#define __STM32L476R_NUCLEO_MOTOSTOKE_H
#include <stdint.h>
void motorInit_V(void);//Setting up clocks and
void motorInit_H(void);//GPIO pins for Motors

void move_H(int);// takes a change in Horizontal coodinates and speed
void moveH_Help(int);//takes in a direction and speed
void delay_ms2(unsigned int ms);
void move_V(int);
void offset_V(int dir);
void offset_H(int dir);
void moveV_Help(int); //takes in a direction and speed
void resetMotor_H(void);
void resetMotor_V(void);
void motoStopper_init(void);
void EXTI9_5_IRQHandler(void);
void mag_Init(void);
void magnetEN(void);
void magnetDIS(void);
void goToZero(void);
void SysTick_Initialize(uint32_t ticks);
void SysTick_Handler(void);
void movePiece(int *v_pos, int *h_pos, int v_from, int v_to, int h_from, int h_to);
void chessBoardInit(void);
void killPiece(int v_pos, int h_pos, int v_to, int h_to);
void clrDeadSpace(void);
#endif
