/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 5  Exercise 3 (challenge)
 *	Exercise Description: Festive lights on port B, PA0 changes the lights
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>

unsigned char button = 0x00;
unsigned char lights = 0x00;

enum States{start, allOn, evenBits, oddBits, outerOn, innerOn} state;

void tick(){ 
	
	switch(state){
		case start:
			lights = 0x00;
			state = allOn;
			break;
			
		case allOn:
			lights = 0x3F;
			if ((~PINA) & 0x01) {
				state = evenBits;
			}
			while ((~PINA) & 0x01) {
			}
			break;
			
		case evenBits:
			lights = 0x15;
			if ((~PINA) & 0x01) {
				state = oddBits;
			}
			while ((~PINA) & 0x01) {
			}
			break;
			
		case oddBits:
			lights = 0x2A;
			if ((~PINA) & 0x01) {
				state = outerOn;
			}
			while ((~PINA) & 0x01) {
			}
			break;
			
		case outerOn:
			lights = 0x21;
			if ((~PINA) & 0x01) {
				state = innerOn;
			}
			while ((~PINA) & 0x01) {
			}
			break;	
			 
			
		case innerOn:
			lights = 0x0C;
			if ((~PINA) & 0x01) {
				state = allOn;
			}
			while ((~PINA) & 0x01) {
			}
			break;			
		default:
			state = start;
			break;
	
	}
	PORTB = lights;				
}
			

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;    
	state = start;
    while (1){tick();}
}