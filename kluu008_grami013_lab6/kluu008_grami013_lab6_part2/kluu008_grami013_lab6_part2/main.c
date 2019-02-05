/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 6  Exercise 2 (Challenge)
 *	Exercise Description: Light Game. Blink 3 LEDs connected to PB0 PB1 PB2 300ms each. Pressing button (PA0) causes sequence to stop and current light to stay on. 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

// TimerISR() sets this to 1. C programmer should clear to 0.
volatile unsigned char TimerFlag = 0; 

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum States {start, zero, one, two, press, wait} state;
unsigned char B = 0x00;
unsigned char button = 0x00;
void Tick() {
	button = ~PINA & 0x01;
	switch(state) { //transitions
		case start:
			state = zero;
			break;
			
		case zero:
			state = button? press : one;
			break;
			
		case one:
			state = button? press : two;
			break;
			
		case two:
			state = button? press : zero;
			break;
			
		case press:
			state = button? press : wait;
			break;
		case wait:
			state = button? zero : wait;
			break;		
		default:
			state = start;
			break;
	}
	switch(state) { //actions
		case start:
			B = 0x00;
			break;
		case zero:
			B = 0x01;
			break;
		case one:
			B = 0x02;
			break;
		case two:
			B = 0x04;
			break;
		case press:
			break;	
		case wait:
			break;			
		default:
			break;
	}
	PORTB = B;
}

void main()
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; // Set port B to output
	PORTB = 0x00; // Init port B to 0s
	button = 0x00;
	TimerSet(300);
	TimerOn();
	B = 0x00;
	while(1) {
		// User code (i.e. synchSM calls)
		Tick();
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
		// Note: For the above a better style would use a synchSM with TickSM()
		// This example just illustrates the use of the ISR and flag
	}
}
