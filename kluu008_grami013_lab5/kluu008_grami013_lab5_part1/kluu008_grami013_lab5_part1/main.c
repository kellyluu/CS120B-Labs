/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 5  Exercise 1 
 *	Exercise Description: car fuel-level sensor
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
/* Input: Fuel level sensor PA3..PA0
 * Output: LEDs PB5..PB0. 
 * B5 = 1-2
 * B5 B4 = 3-4
 * B5 B4 B3 = 5-6
 * B5 B4 B3 B2 = 7-9
 * B5 B4 B3 B2 B1 = 10-12
 * B5 B4 B3 B2 B1 B0 = 13-15
 * PB6 = low fuel (4 or less)
 */
int main(void)
{
	// input (buttons)
	DDRA = 0x00; PORTA = 0xFF;
	// output (LEDs)
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char button = 0x00;
	unsigned char led = 0x00;
	while (1) {
		button = (~PINA) & 0x0F;
		if (button < 1) {
			led = 0x40;
		} else if (button < 3) {
			led = 0x60;
		} else if (button < 5) {
			led = 0x70;
		} else if (button < 7) {
			led = 0x38;
		} else if (button < 10) {
			led = 0x3C;
		} else if (button < 13) {
			led = 0x3E;
		} else {
			led = 0x3F;
		}
		PORTC = led;
	}
}

