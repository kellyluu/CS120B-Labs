/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 4  Exercise 5 (challenge 2) 
 *	Exercise Description: digital combination deadbolt lock. Code can lock door now. 4 digit pin
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

enum States {start, locked, input, unlocked} State;
enum States prevState;
unsigned char lock = 0x00;
unsigned char currState = 0x00;
const unsigned char ucpasscode[7] = {0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01};
unsigned char uccounter = 0;


void Tick() {
	//transitions
	switch(State) {
		case start:
			State = locked;
			break;
			
		case locked:
			uccounter = 0;		
			if( (PINA >> 7) == 0x01) { // button inside house
				State = locked;
			} else if (PINA == ucpasscode[uccounter]) { // only #
				prevState = State;
				State = input;
				uccounter++;
			} else {
				State = locked;
			}
			break;
			
		case input:
			if( (PINA >> 7) == 0x01) { // button inside house
				State = locked;
				break;
			}
			if(PINA == ucpasscode[uccounter-1]){break;} //wait for previous button to be unpressed
			if(uccounter < 6){
				if (PINA == ucpasscode[uccounter]){
				State = input;
				uccounter++;
				}else{
					State = prevState;
				}
			}else{
				 if (PINA == ucpasscode[uccounter]){
					if (prevState == unlocked){
					State = locked;
					}else{State = unlocked;}
				}else{
					State = prevState;
				}
			}
			break;
			
		case unlocked:
			uccounter = 0;
			if ( (PINA >> 7) == 0x01) { // button inside house
				State = locked;
			} else if (PINA == ucpasscode[uccounter]) { // # pressed again to start locking
				prevState = unlocked;
				State = input;
				uccounter++;
			} else {
				State = unlocked;
			}
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
			
		case input:
			currState = 0x04;
			break;
		
		case unlocked:
			currState = 0x06;
			lock = 0x01;
						
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
	prevState = start;
	State = start;
    while (1) 
    {
		Tick();
    }
}