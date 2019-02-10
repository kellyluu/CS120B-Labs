/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 9  Exercise 2
 *	Exercise Description: 8 note scale
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

// PWM functions
void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; }
		else { TCCR0B |= 0x03; }
			
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		else if (frequency > 31250) { OCR0A = 0x0000; }
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }
		TCNT0 = 0;
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR0A = (1 << COM0A0) | (1 << WGM00);
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}

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

//global variables 
const double C4 = 261.63;
const double D = 293.66;
const double E = 329.63;
const double F = 349.23;
const double G = 392.00;
const double A = 440.00;
const double B = 493.88;
const double C5 = 523.25;

double noteArr[] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
unsigned char arrIndex = 0;

enum sound {sound_start, off, on_wait, on, off_wait} sound_state;
enum note {note_start, no_button, waitRelease} note_state;
	
unsigned char soundButton = 0x00;
unsigned char scaleButtons = 0x00;

void Tick() {
	soundButton = ~PINA & 0x01;
	scaleButtons = ~PINA & 0x06;
	// SET SOUND ON/OFF
	switch(sound_state) {
		case sound_start:
			sound_state = off;
			break;
		case off:
			if (soundButton) {
				set_PWM(noteArr[arrIndex]);
				sound_state = on_wait;
			}
			else {
				set_PWM(0);
				sound_state = off;
			}
			break;
		case on_wait:
			if (soundButton) {
				set_PWM(noteArr[arrIndex]);
				sound_state = on_wait;
			}
			else {
				set_PWM(noteArr[arrIndex]);
				sound_state = on;
			}
			break;
		case on:
			if (soundButton) {
				set_PWM(0);
				sound_state = off_wait;
			}
			else {
				set_PWM(noteArr[arrIndex]);
				sound_state = on;
			}
			break;
		case off_wait:
			if (soundButton) {
				sound_state = off_wait;
			}
			else {
				sound_state = off;
			}
			break;
		default:
			break;		
	} // end of sound button synchSM
		
	switch(note_state) { // switch note states
		case note_start:
			arrIndex = 0;
			note_state = no_button;
			break;
		case no_button:
			if ((scaleButtons == 0x04) && (arrIndex < 7)) { //up
				arrIndex++;
				note_state = waitRelease;
			}
			else if ((scaleButtons == 0x02) && (arrIndex > 0)) { //down
				arrIndex--;
				note_state = waitRelease;
			}
			else {
				note_state = no_button;
			}
			break;
		case waitRelease:
			if (scaleButtons == 0x02 || scaleButtons == 0x04 || scaleButtons == 0x06) {
				note_state = waitRelease;
			}
			else {
				note_state = no_button;
			}
			break;
		default:
			break;
	} // end of note synchSM
}

int main(void) {
	//input
	DDRA = 0x00; PORTA = 0xFF;
	//output
	DDRB = 0xFF; PORTB = 0x00;
	PWM_on();
	set_PWM(0);
	TimerSet(100);
	TimerOn();
	sound_state = sound_start;
	note_state = note_start;
	while (1) {
		Tick();
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
	}
}
