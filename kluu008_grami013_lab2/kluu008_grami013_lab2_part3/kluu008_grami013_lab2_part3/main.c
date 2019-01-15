/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 2  Exercise 3
 *	Exercise Description: output number of available spaces and B7 represents if the lot is full
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char spot1 = 0x00;
	unsigned char spot2 = 0x00;
	unsigned char spot3 = 0x00;
	unsigned char spot4 = 0x00;
	unsigned char cntavail = 0x00;
    while (1) 
    {
		cntavail = 4;

		spot1 = PINA & 0x01 ;
		if (spot1 == 0x01) 
		{
			cntavail--;
		}
		spot2 = PINA & 0x02 ;
		if (spot2 == 0x02)
		{
			cntavail--;
		}
		spot3 = PINA & 0x04 ;
		if (spot3 == 0x04)
		{
			cntavail--;
		}
		spot4 = PINA & 0x08 ;
		if (spot4 == 0x08)
		{
			cntavail--;
		}

		if (cntavail == 0) {
			PORTC = 0x80;
			
		} else {
			PORTC = PORTC & 0x7F;
			PORTC = 0x0F & cntavail;
		}
    }
}