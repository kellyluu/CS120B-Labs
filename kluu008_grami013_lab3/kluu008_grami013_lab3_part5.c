/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 3  Exercise 5
 *	Exercise Description: reading weight from seat for airbag setting
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
    DDRD = 0x00; PORTD = 0xFF;
    DDRB = 0xFE; PORTB = 0x01;
	
	unsigned short tempD = 0x00;
	unsigned short usWeight = 0x00;
		
	while (1) 
    {
		tempD = PIND;
		usWeight = (PIND << 1) + GetBit(PINB,0);
		if(usWeight <= 5){ //nothing in seat (both off)
				PORTB = 0x00;
		}else if(usWeight < 70){ //child in seat (b2 on b1 off)
				PORTB = 0x04;
		}else{ //adult in seat (b1 on b2 off)
				PORTB = 0x02;
				
			}

    }
}

