/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 10  Exercise 4 (challenge)
 *	Exercise Description: 3 concurrent synchSMs: LEDs, button on PA2, and speaker on PB4, buttons on PA0 and PA1 select sound frequency
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

// method one: use a tick function to write to global variable to change length of period
// method two: use tick function to update count for speaker output SM (loop?)

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
// SM 4: button & speaker
enum speaker_states {speaker_start, buttonNotPress, buttonPressOn, buttonPressOff} speaker_state; // on/off
// SM 5: get frequency for timer period
enum frequency_states {freq_start, freq_noButton, freq_waitRelease} freq_state; // scale up/down

	
//global variables
// inputs:
unsigned char buttonInput = 0x00;
unsigned char adjustButtons = 0x00;
// outputs:
unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;
unsigned char speakerOn = 0x00;
volatile unsigned char speakerPitch = 0x00;

unsigned char count = 0x00;

void freq_tick() {
	adjustButtons = ~PINA & 0x03;
	switch(freq_state) { // switch frequency
		case freq_start:
			speakerPitch = 2;
			freq_state = freq_noButton;
			break;
		case freq_noButton:
			if ((adjustButtons == 0x01) && (speakerPitch < 6)) { //up
				speakerPitch++;
				freq_state = freq_waitRelease;
			}
			else if ((adjustButtons == 0x02) && (speakerPitch > 1)) { //down
				speakerPitch--;
				freq_state = freq_waitRelease;
			}
			else {
				freq_state = freq_noButton;
			}
			break;
		case freq_waitRelease:
			if (adjustButtons) {
				freq_state = freq_waitRelease;
			}
			else {
				freq_state = freq_noButton;
			}
			break;
		default:
			freq_state = freq_start;
			break;
	}
}
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

void speaker_Tick() {
	buttonInput = ~PINA & 0x04;
	
	switch(speaker_state) {
		case speaker_start:
			count = 0;
			speakerOn = 0x00;
			speaker_state = buttonNotPress;
			break;
		case buttonNotPress:
			count = 0;
			speakerOn = 0x00;
			speaker_state = (buttonInput) ? buttonPressOn : buttonNotPress;
			break;
		case buttonPressOn:
			speakerOn = 0x10;
			speaker_state = (buttonInput) ? buttonPressOff : buttonNotPress;
			break;
		case buttonPressOff:
			speakerOn = 0x00;
			speaker_state = (buttonInput) ? buttonPressOn : buttonNotPress;
			break;
		default:
			speaker_state = speaker_start;
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
			PORTB = threeLEDs + blinkingLED + speakerOn;
			cLED_state = cLED_output;
			break;
		default:
			break;
	}
}

int main(void)
{
	//input: button on PA2
	DDRA = 0x00; PORTA = 0xFF;
	//output: LEDs on PB0-PB3, Speaker on PB4
	DDRB = 0xFF; PORTB = 0x00;
    // initialize outputs
	threeLEDs = 0x00;
	blinkingLED = 0x00;
	// initialize states
	freq_state = freq_start;
	tLED_state = tLED_start;
	bLED_state = bLED_start;
	cLED_state = cLED_start;
	speaker_state = speaker_start;
	
	unsigned short tLED_elapsed = 300;
	unsigned short bLED_elapsed = 1000;
	unsigned char speaker_elapsed = 1;
	TimerSet(1); // period
	TimerOn();
	while (1) {
		
		if (tLED_elapsed >= 300) {
			tLED_Tick();
			tLED_elapsed = 0;
		}
		if (bLED_elapsed >= 1000) {
			bLED_Tick();
			bLED_elapsed = 0;
		}
		freq_tick();
		if (speaker_elapsed >= speakerPitch) {
			speaker_Tick();
			speaker_elapsed = 0;
		} 
		cLED_Tick();
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
		tLED_elapsed += 1;
		bLED_elapsed += 1;
		speaker_elapsed += 1;
	}
}

