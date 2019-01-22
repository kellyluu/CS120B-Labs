/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 3  Exercise 2
 *	Exercise Description: fuel tank level
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
	DDRC = 0xFF; PORTC = 0x00;
	
	unsigned char uctempA = 0x00;
	PORTC = 0x00;
	
	while (1) 
    {
		uctempA = PINA & 0x0F;
		if(uctempA < 1)PORTC = 0x40;
		else if(uctempA < 3)PORTC = 0x60;
		else if(uctempA < 5)PORTC = 0x70;
		else if(uctempA < 7)PORTC = 0x38;
		else if(uctempA < 10)PORTC = 0x3C;
		else if(uctempA < 13)PORTC = 0x3E;
		else PORTC = 0x3F;
    }
}

