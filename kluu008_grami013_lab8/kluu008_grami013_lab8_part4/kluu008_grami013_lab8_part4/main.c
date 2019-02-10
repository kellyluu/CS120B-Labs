/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 8  Exercise 4
 *	Exercise Description: light meter (challenge)
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>


void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

enum States {start, update} state;
void tick() {
	unsigned short x = ADC; 
		switch(state) { //actions
			case start:
				PORTB = 0x00;
				break;
			case update:
				if (x >= 127) {
					PORTB = 0xFF;
				}
				else if (x >= 111) {
					PORTB = 0x7F;
				}
				else if (x >= 95) {
					PORTB = 0x3F;
				}
				else if (x >= 79) {
					PORTB = 0x1F;
				}
				else if (x >= 63) {
					PORTB = 0x0F;
				}
				else if (x >= 47) {
					PORTB = 0x07;
				}
				else if (x >= 32) {
					PORTB = 0x03;
				}
				else if (x >= 16) {
					PORTB = 0x01;
				}
				else if (x > 16) {
					PORTB = 0x00;
				}
				break;
			default:
				break;
		}
	switch(state) { //transitions
		case start:
			state = update;
			break;
		case update:
			state = update;
		default:
			state = start;
	}
}


int main(void) {
	//input
	DDRA = 0x00; PORTA = 0xFF;
	//output
	DDRB = 0xFF; PORTB = 0x00;
	state = start;
	ADC_init();
    while (1) {
		tick();
    }
}

