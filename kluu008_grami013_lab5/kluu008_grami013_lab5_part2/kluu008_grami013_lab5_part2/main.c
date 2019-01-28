/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 5  Exercise 2
 *	Exercise Description: Counter on PortB controlled by A0 & A1
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>

unsigned char uctempB = 0x00;

enum States{Start, noButton, buttonOne, buttonTwo, bothButtons} state;


unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
void tick(){ 
	
	uctempB = PORTB;
	
	switch(state){
		case Start:
			PORTB = 0x07;
			state = noButton;
			break;
		case noButton:
			if (GetBit(~PINA,0) && !GetBit(~PINA,1))
			{
				state = buttonOne;
				uctempC = uctempC < 9? uctempC+1:9;
				PORTB = uctempC;
			}else if (!GetBit(~PINA,0) && GetBit(~PINA,1))
			{
				state = buttonTwo;
				uctempC = uctempC > 0? uctempC-1:0;
				PORTB = uctempC;
			}else if (GetBit(~PINA,0) && GetBit(~PINA,1))
			{
				state = bothButtons;
				PORTB = 0;
			}else{
				state = noButton;
			}
			break;
		case buttonOne:
			
			if (GetBit(~PINA,0) && !GetBit(~PINA,1))
			{
				state = buttonOne;
			}else if (!GetBit(~PINA,0) && GetBit(~PINA,1))
			{
				state = buttonTwo;
				uctempC = uctempC > 0? uctempC-1:0;
				PORTB = uctempC;
			}else if (GetBit(~PINA,0) && GetBit(~PINA,1))
			{
				state = bothButtons;
				PORTB = 0;
			}else{
				state = noButton;
			}
			break;
		case buttonTwo:
			if (GetBit(~PINA,0) && !GetBit(~PINA,1))
			{
				state = buttonOne;
				uctempC = uctempC < 9? uctempC+1:9;
				PORTB = uctempC;
			}else if (!GetBit(~PINA,0) && GetBit(~PINA,1))
			{
				state = buttonTwo;
			}else if (GetBit(~PINA,0) && GetBit(~PINA,1))
			{
				state = bothButtons;
				PORTB = 0;
				}else{
				state = noButton;
			}
			break;
		case bothButtons:
			if (GetBit(~PINA,0) || GetBit(~PINA,1))
			{
				state = bothButtons;
				PORTB = 0;
				}else{
				state = noButton;
			}
			break;
		default:
		state = Start;
		break;
	}
}


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x07;    /* Replace with your application code */
	state = Start;
	
    while (1){tick();}
}
