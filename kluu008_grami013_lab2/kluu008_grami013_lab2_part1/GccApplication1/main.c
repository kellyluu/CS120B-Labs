/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 2  Exercise 1 
 *	Exercise Description: set LED based on garage sensor and light sensor
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	while(1)
	{
		// 1) Read input (rightmost 2 bits: 00000011)
		//PA1 = light sensor, PA0 = garage sensor, PBO = LED
		tmpA = PINA & 0x03;
		// 2) Perform computation
		// if PA0 is 1 and PA1 is 0, set PB0 = 1, else = 0
		if (tmpA == 0x01) {// true if PA0 is 1 and PA1 is 0
			tmpB = (tmpB & 0xFE) | 0x01; // Sets tmpB to bbbbbbb1
			// (clear rightmost bit, then set to 1)
		}
		else {
			tmpB = (tmpB & 0xFE) | 0x00; // Sets tmpB to bbbbbbb0
			// (clear rightmost bit, then set to 0)
		}
		// 3) Write output
		PORTB = tmpB;
	}
	return 0;
}

