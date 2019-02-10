/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 8  Exercise 2
 *	Exercise Description: LEDs & photoresistor
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
	switch(state) {
		case start:
			state = update;
			break;
		case update:
			PORTB = (char)x;
			PORTD = (char)(x >> 8);
			break;
	}
}


int main(void) {
	//input
	DDRA = 0x00; PORTA = 0xFF;
	//output
	DDRB = 0xFF; PORTB = 0x00; // lower 8 of x
	DDRD = 0xFF; PORTD = 0x00; // upper 2 of x
	state = start;
	ADC_init();
    while (1) {
		tick();
    }
}

