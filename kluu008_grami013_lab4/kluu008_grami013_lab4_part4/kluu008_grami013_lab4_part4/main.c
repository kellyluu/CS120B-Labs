/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 4  Exercise 4 (challenge 1) 
 *	Exercise Description: digital combination deadbolt lock. Code can lock door now.
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/* assumptions:
 * - SM starts locked
 * - door is unlocked when # is pressed and released and as soon as Y is pressed (not on release)
 * - When PA7 (button inside house) is pressed, regardless of sequence or other buttons being pressed, door is locked
 * - when # is still pressed, any other input keeps the door locked (back to locked state)
 * - once unlocked, door stays unlocked until indoor button is pressed
 */
#include <avr/io.h>

//bit access functions
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

enum States {start, locked, input1, wait1, unlocked, input2, wait2} State;
unsigned char lock = 0x00;
unsigned char currState = 0x00;

void Tick() {
	//transitions
	switch(State) {
		case start:
			State = locked;
			break;
			
		case locked:
			if( (PINA >> 7) == 0x01) { // button inside house
				State = locked;
			} else if (PINA == 0x04) { // only #
				State = input1;
			} else {
				State = locked;
			}
			break;
			
		case input1:
			if( (PINA >> 7) == 0x01) { // button inside house
				State = locked;
			} else if(PINA == 0x04) { // # still pressed
				State = input1;
			} else if(PINA == 0x00) { // button released -> wait for new input
				State = wait1;
			} else {
				State = locked;
			}
			break;
			
		case wait1:
			if ( (PINA >> 7) == 0x01) { // button inside house
				State = locked;
			} else if (PINA == 0x02) { // y is pressed
				State = unlocked;
			} else if (PINA == 0x00) { // wait for y
				State = wait1;
			} else {
				State = locked;
			}
			break;
			
		case unlocked:
			if ( (PINA >> 7) == 0x01) { // button inside house
				State = locked;
			} else if (PINA == 0x04) { // # pressed again to start locking
				State = input2;
			} else {
				State = unlocked;
			}
			break;
			
		case input2:
			if( (PINA >> 7) == 0x01) { // button inside house
				State = locked;
			} else if(PINA == 0x04) { // # still pressed
				State = input2;
			} else if(PINA == 0x00) { // button released -> wait for new input
				State = wait2;
			} else {
				State = locked;
			}
			break; 
			
		case wait2:
			if ( (PINA >> 7) == 0x01) { // button inside house
				State = locked;
			} else if (PINA == 0x02) { // y is pressed -> door is locked
				State = locked;
			} else if (PINA == 0x00) { // wait for y
				State = wait2;
			} else {
				State = unlocked; // any other input keeps it locked
			}
			break;	
						
		default:
			State = start;
			break;
	}
	//state actions
	switch(State) {
		case start:
			currState = 0x01;
			break;
			
		case locked:
			lock = 0x00;
			currState = 0x02;
			break;
			
		case input1:
			currState = 0x04;
			break;
			
		case wait1:
			currState = 0x08;
			break;
			
		case unlocked:
			currState = 0x10;
			lock = 0x01;
			break;
			
		case input2:
			currState = 0x20;
			break;
		
		case wait2:
			currState = 0x40;
			break;		
			
		default:
			break;
	}
	PORTB = lock;
	PORTC = currState;
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // input
	DDRB = 0xFF; PORTB = 0x00; // output
	DDRC = 0xFF; PORTC = 0x00; // output
    lock = 0x00;
	currState = 0x00;
	State = start;
    while (1) 
    {
		Tick();
    }
}
