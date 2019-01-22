/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 3  Exercise 4
 *	Exercise Description: PINA copied and shifted to PORTB & PORTC
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
	    DDRA = 0x00; PORTA = 0xFF;
	    DDRB = 0xFF; PORTB = 0x00;
	    DDRC = 0xFF; PORTC = 0x00;
		
		unsigned char uctempA = 0x00;
    /* Replace with your application code */
    while (1) 
    {
		uctempA = PINA;
		PORTB = (uctempA >> 4);
		PORTC = (uctempA << 4);
			
    }
}

