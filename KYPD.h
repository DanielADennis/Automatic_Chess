#ifndef __STM32L476R_NUCLEO_KYPD_H
#define __STM32L476R_NUCLEO_KYPD_H

void init_KYPD(void);
unsigned int check_KYPD(void);
unsigned char to_LCD(unsigned int);
int only_Numb(unsigned int);
unsigned int kypd_in(void);
#endif
