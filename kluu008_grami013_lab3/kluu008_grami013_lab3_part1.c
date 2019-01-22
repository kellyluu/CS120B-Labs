/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 3  Exercise 1
 *	Exercise Description: Count the number of 1s on ports A and B and output that number on port C. 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}



int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	unsigned char uctempA = 0x00;
	unsigned char uctempB = 0x00;
	unsigned char ucbitCount = 0x00;
		
	while (1) 
    {
		uctempA = PINA;
		uctempB = PINB;
		ucbitCount = 0;		
		for (int i = 0; i < 8; i++)
		{
			ucbitCount = GetBit(uctempA, i) + GetBit(uctempB, i) + ucbitCount;
		}
		PORTC = ucbitCount;
    }
}

