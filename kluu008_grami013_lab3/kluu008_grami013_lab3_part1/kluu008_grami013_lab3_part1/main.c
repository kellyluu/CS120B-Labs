/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 3  Exercise 1 
 *	Exercise Description: count 1s on ports A & B and output on port C
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

//bit access functions
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
    unsigned inputA = 0x00;
	unsigned inputB = 0x00;
	unsigned numOnes = 0x00;
	
    while (1) 
    {
		numOnes = 0x00;
		inputA = PINA;
		inputB = PINB;
		for (unsigned i = 0; i < 8; i++) {
			if(GetBit(inputA, i)) {
				numOnes++;
			}
			if(GetBit(inputB, i)) {
				numOnes++;
			}
		}
		PORTC = numOnes;
    }
}

