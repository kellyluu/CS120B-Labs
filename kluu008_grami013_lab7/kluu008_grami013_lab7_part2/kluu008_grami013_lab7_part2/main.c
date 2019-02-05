/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 7  Exercise 2 (Challenge)
 *	Exercise Description: Light Game. Output score on LCD.
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"

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

enum States {start, zero, one, two, press, waitRelease, pause, resume} state;
unsigned char B = 0x00;
unsigned char button = 0x00;
unsigned char score = 0x05;
unsigned char lightTicks = 0x00;
const unsigned char* wait_r = "w_r";
const unsigned char* pause_r = "p_r";
const unsigned char* zero_r = "0_r";
const unsigned char* one_r = "1_r";
const unsigned char* two_r = "2_r";
const unsigned char* victory = "YOU WIN!";
void Tick() {
	button = ~PINA & 0x01;
	switch(state) { //actions
		case start:
			score = 0x05;
			LCD_ClearScreen();
			LCD_Cursor(1);
			LCD_WriteData( score + '0');
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
			LCD_ClearScreen();
			LCD_Cursor(1);
			if (B == 0x02) {
				score =  score < 9? (score+1):9;
				if (score == 9) {
					LCD_DisplayString(1, victory);
				} else {
					LCD_WriteData( score + '0');
				}
			} else {
				score =  score > 0? (score-1):0;
				LCD_WriteData( score + '0' );
			}
			break;
		case waitRelease:
			break;
		case pause:
			break;
		case resume:
			break;
		default:
			break;
}	
	switch(state) { //transitions
		case start:
			lightTicks = 0x00;
			state = zero;
			break;
		case zero:
			if (button) {
				state = press;
			} else if (lightTicks >= 3) {
				state = one;
				lightTicks = 0;
			} else {
				lightTicks++;
			}
			break;
		case one:
			if (button) {
				state = press;
			} else if (lightTicks >= 3) {
				state = two;
				lightTicks = 0;
			} else {
				lightTicks++;
			}
			break;
		case two:
			if (button) {
				state = press;
			} else if (lightTicks >= 3) {
				state = zero;
				lightTicks = 0;
			} else {
				lightTicks++;
			}
			break;
		case press:
			state = waitRelease;
			break;
		case waitRelease:
			state = button? waitRelease : pause;
			break;		
		case pause:
			state = button? resume : pause;
			break;
		case resume:
			if(!button) {
				if (score >= 9) {
					state = start;
				}
				else {
					state = zero;
				}
			} else {
				state = resume;
			}
			break;

			
		default:
			state = start;
			break;
	}
	
	PORTB = B;
}

void main()
{
	//input
	DDRA = 0x00; PORTA = 0xFF;
	//output
	DDRB = 0xFF; PORTB = 0x00; // LEDs
	DDRC = 0xFF; PORTC = 0x00; // LCD
	DDRD = 0xFF; PORTD = 0x00; // LCD
	button = 0x00;
	B = 0x00;
	score = 0x00;
	TimerSet(100);
	TimerOn();
	LCD_init();
	while(1) {
		// User code (i.e. synchSM calls)
		Tick();
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
		// Note: For the above a better style would use a synchSM with TickSM()
		// This example just illustrates the use of the ISR and flag
	}
}
