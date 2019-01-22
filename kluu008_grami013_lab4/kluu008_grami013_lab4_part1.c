/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 4  Exercise 1
 *	Exercise Description: PA0 switches on LED between PB0 and PB1
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>

// bit access functions
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

enum SM_States {SM_start, SM_b0, SM_wait0, SM_wait1, SM_b1} SM_State;
unsigned char B = 0x00;

void Tick() {
	switch(SM_State) {
		case SM_start:
			SM_State = SM_b0;
			B = 0x01;
			break;
			
		case SM_b0:
			SM_State = (GetBit(PINA, 0)? SM_wait0 : SM_b0);
			break;

			break;
		case SM_wait0:
			B = 0x01;
			SM_State = (GetBit(PINA, 0)? SM_wait0 : SM_b1);
			break;	
			
		case SM_b1:
			SM_State = (GetBit(PINA, 0)? SM_wait1 : SM_b1);
			break;

		case SM_wait1:
			B = 0x02;
			SM_State = (GetBit(PINA, 0)? SM_wait1 : SM_b0);
			break;
			
		default:
			SM_State = SM_start;
			break;
	}
	PORTB = B;
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	// input
	DDRB = 0xFF; PORTB = 0x00;	// output

	B = 0x00;
	SM_State = SM_start;

    while (1) 
    {
		Tick();
    }
}

