/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 9  Exercise 3
 *	Exercise Description: melody (challenge)
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

// A F G G A
// D-F F C-Bb A-G
// D A F G-G A
// G-F  G-A-F
double A = 440.00;
double F = 349.23;
double G = 392.00;
double D = 293.66;
//double Bb = 415.30;
double C = 523.25;
double Csharp = 554.37;
double DD = 587.33;
// 440.00, 440.00, 440.00, 349.23, 349.23, 349.23, 392.00, 392.00, 392.00, 0, 392.00, 392.00, 392.00, 440.00, 440.00, 440.00, 440.00, 440.00, 0, 0,
// 293.66, 293.66, 293.66, 349.23, 349.23, 349.23, 0, 349.23, 349.23, 349.23, 0, 349.23, 349.23, 523.25, 523.25, 523.25, 440.00, 440.00, 440.00, 440.00, 392.00, 392.00, 392.00, 0, 0,
double songNotes[] = {0, 293.66, 293.66, 293.66, 440.00, 440.00, 440.00, 349.23, 349.23, 392.00, 392.00, 0, 392.00, 392.00, 392.00, 440.00, 440.00, 440.00, 440.00,
					392.00, 392.00, 392.00, 349.23, 349.23, 349.23, 392.00, 392.00, 392.00, 392.00, 392.00, 440.00, 440.00, 440.00, 440.00, 440.00, 349.23, 349.23, 349.23, 349.23, 349.23, 0, 0, 0, 0};

unsigned char arrIndex = 0;
unsigned char arrSize = (char)(sizeof(songNotes)/sizeof(songNotes[0])); 
unsigned char button = 0x00;
unsigned char count = 0x00;
enum States {start, waitPress, play, waitRelease} state;
void playNote() {
	set_PWM(songNotes[arrIndex]);
	if (arrIndex < arrSize) {
		arrIndex++;
	}
	else {
		arrIndex = 0;
	}
}
void Tick() {
	button = ~PINA & 0x01;
	switch(state) {
		case start:
			state = waitPress;
			set_PWM(0);
			break;
		case waitPress:
			count = 0;
			arrIndex = 0;
			set_PWM(0);
			if (button) {
				state = play;
			}
			else {
				state = waitPress;
			}
			break;
		case play:
			if (count < arrSize) {
				playNote();
				count++;
				state = play;
				
			}
			else if (button){
				state = waitRelease;
			}
			else {
				state = waitPress;
			}
			break;
		case waitRelease:
			count = 0;
			if (button) {
				state = waitRelease;
			}
			else {
				state = waitPress;
			}
		default:
			break;
	}
}

int main(void) {
	//input
	DDRA = 0x00; PORTA = 0xFF;
	//output
	DDRB = 0xFF; PORTB = 0x00;
	PWM_on();
	set_PWM(0);
	TimerSet(125);
	TimerOn();
	set_PWM(0);
	arrIndex = 0;
	while (1) {
		Tick();
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
	}
}
