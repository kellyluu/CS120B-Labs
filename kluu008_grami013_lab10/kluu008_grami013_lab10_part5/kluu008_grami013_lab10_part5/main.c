/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 10  Exercise 5
 *	Exercise Description: Counter on PORTB leds controlled by A0 & A1
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
//synch SM timer variables and functions
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1=0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}
void TimerOff() {
	TCCR1B = 0x00;
}
void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

// STATE MACHINE
enum States{Start, noButton, buttonOne, buttonTwo, bothButtons, increment, decrement} state;
unsigned char buttonInput = 0x00;
unsigned char number = 0x00;
unsigned char tickLength = 0;
unsigned char count = 0;
unsigned char three = 0;
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
void tick(){ 
	buttonInput = ~PINA & 0x03;
	switch(state) { // ACTIONS
		case Start: 
			number = 0x05;
			tickLength = 10;
			three = 0;
			break;
		case noButton:
			count = 0;
			tickLength = 10;
			three = 0;
			break;
		case buttonOne:
			count = 0;
			number = number < 9? (number+1):9;
			break;
		case buttonTwo:
			count = 0;
			number = number > 0? (number-1):0;
			break;
		case bothButtons:
			count = 0;
			number = 0;
			tickLength = 10;
			three = 0;
			break;
		case increment:
			if (three != 3) {
				if (count >= tickLength) {
					number = number < 9? (number+1):9;
					count = 0;
					three++;
				} else {
					count++;
				}
			} else {
				tickLength = 4;
				if (count >= tickLength) {
					number = number < 9? (number+1):9;
					count = 0;
				} else {
					count++;
				}
			}
			break;
		case decrement:
			if (three != 3) {
				if (count >= tickLength) {
					number = number > 0? (number-1):0;
					count = 0;
					three++;
					} else {
					count++;
				}
			} else {
				tickLength = 4;
				if (count >= tickLength) {
					number = number > 0? (number-1):0;
					count = 0;
				} else {
					count++;
				}
			}
			break;			
		default:
			break;
	}
	switch(state) { // TRANSITIONS
		case Start:
			state = noButton;
			break;
		case noButton:
			if (buttonInput == 0x01) {
				state = buttonOne;
			} else if (buttonInput == 0x02) {
				state = buttonTwo;
			}else if (buttonInput == 0x03) {
				state = bothButtons;
			}else{
				state = noButton;
			}
			break;
		case buttonOne:
			if (buttonInput == 0x01) {
				state = increment;
			} else if (buttonInput == 0x02) {
				state = buttonTwo;
			} else if (buttonInput == 0x03) {
				state = bothButtons;
			} else{
				state = noButton;
			}
			break;
		case buttonTwo:
			if (buttonInput == 0x01) {
				state = buttonOne;
			} else if (buttonInput == 0x02) {
				state = decrement;
			} else if (buttonInput == 0x03) {
				state = bothButtons;
			} else{
				state = noButton;
			}
			break;
		case bothButtons:
			if (buttonInput == 0x03) {
				state = bothButtons;
			} else{
				state = noButton;
			}
			break;
		case increment:
			if (buttonInput == 0x01) {
				state = increment;
			} else if (buttonInput == 0x02) {
				state = buttonTwo;
			} else if(buttonInput == 0x03) {
				state = bothButtons;
			} else {
				state = noButton;
			}
			break;
		case decrement:
			if (buttonInput == 0x02) {
				state = decrement;
				} else if (buttonInput == 0x01) {
				state = buttonOne;
				} else if(buttonInput == 0x03) {
				state = bothButtons;
				} else {
				state = noButton;
			}
			break;
		default:
			state = Start;
			break;
	}
	PORTB = number;
}


int main(void)
{
	//input: buttons on PA0 and PA1
    DDRA = 0x00; PORTA = 0xFF;
	//output: 4 LEDs on PORTB
	DDRB = 0xFF; PORTB = 0x07; 
	state = Start;
	number = 0x00;
	buttonInput = 0x00;
	TimerSet(100);
	TimerOn();
    while (1){
		tick();
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
	}
}