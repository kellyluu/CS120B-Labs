/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 10  Exercise 2
 *	Exercise Description: 3 concurrent synchSMs: different periods
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

// synch SM timer variables & functions
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

// SM 1: 
enum tLED_states {tLED_start, tLED_0, tLED_1, tLED_2} tLED_state;
// SM 2:
enum bLED_states {bLED_start, bLED_on, bLED_off} bLED_state;
// SM 3:
enum cLED_states {cLED_start, cLED_output} cLED_state;
	
//global variables
unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;

void tLED_Tick() {
	switch (tLED_state) {
		case tLED_start:
			threeLEDs = 0x00;
			tLED_state = tLED_0;
			break;
		case tLED_0:
			threeLEDs = 0x01;
			tLED_state = tLED_1;
			break;
		case tLED_1:
			threeLEDs = 0x02;
			tLED_state = tLED_2;
			break;
		case tLED_2:
			threeLEDs = 0x04;
			tLED_state = tLED_0;
			break;
		default:
			break;		
	}
}

void bLED_Tick() {
	switch(bLED_state) {
		case bLED_start:
			blinkingLED = 0x00;
			bLED_state = bLED_on;
			break;
		case bLED_on:
			blinkingLED = 0x08;
			bLED_state = bLED_off;
			break;
		case bLED_off:
			blinkingLED = 0x00;
			bLED_state = bLED_on;
			break;
		default:
			break;
	}
}

void cLED_Tick() {
	switch(cLED_state) {
		case cLED_start:
			PORTB = 0x00;
			cLED_state = cLED_output;
			break;
		case cLED_output:
			PORTB = threeLEDs + blinkingLED;
			cLED_state = cLED_output;
			break;
		default:
			break;
	}
}


int main(void)
{
	//output: LEDs on PB0-PB3
	DDRB = 0xFF; PORTB = 0x00;
    // initialize outputs
	threeLEDs = 0x00;
	blinkingLED = 0x00;
	// initialize states
	tLED_state = tLED_start;
	bLED_state = bLED_start;
	cLED_state = cLED_start;
	unsigned char tLED_elapsed = 3;
	unsigned char bLED_elapsed = 10;
	
	TimerSet(100);
	TimerOn();
	while (1) {
		if (tLED_elapsed >= 3) {
			tLED_Tick();
			tLED_elapsed = 0;
		}
		if (bLED_elapsed >= 10) {
			bLED_Tick();
			bLED_elapsed = 0;
		}
		cLED_Tick();
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
		tLED_elapsed++;
		bLED_elapsed++;
	}
}

