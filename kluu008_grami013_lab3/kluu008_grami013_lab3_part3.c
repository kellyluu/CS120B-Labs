/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 3  Exercise 3
 *	Exercise Description: fuel tank level + seatbelt status
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
	unsigned char uctempA2 = 0x00;
	unsigned char uctempC = 0x00;
	PORTC = 0x00;
	
	while (1) 
    {
		uctempA2 = PINA;
		uctempA = uctempA2 & 0x0F;
		
		if(uctempA < 1)uctempC = 0x40;
		else if(uctempA < 3)uctempC = 0x60;
		else if(uctempA < 5)uctempC = 0x70;
		else if(uctempA < 7)uctempC = 0x38;
		else if(uctempA < 10)uctempC = 0x3C;
		else if(uctempA < 13)uctempC = 0x3E;
		else uctempC = 0x3F;
		
		//if((GetBit(uctempA2,4) && GetBit(uctempA2,5)) && !GetBit(uctempA2,6)) uctempC = uctempC + 0x80;
		if(GetBit(uctempA2,4)){
			if(GetBit(uctempA2,5)){
				if (!GetBit(uctempA2,6))
				{uctempC = uctempC | 0x80;
				}
			}
		}
		PORTC = uctempC;
		
    }
}

