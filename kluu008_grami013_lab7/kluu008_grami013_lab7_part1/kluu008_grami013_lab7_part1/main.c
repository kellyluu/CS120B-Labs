/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 7  Exercise 1
 *	Exercise Description: Counter on LCD controlled by A0 & A1
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "io.c"
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


//unsigned char button = 0x00;
enum States{Start, noButton, buttonOne, buttonTwo, bothButtons, increment, decrement} state;
unsigned char buttonInput = 0x00;
unsigned char number = 0x00;
unsigned char msCount = 0x00;
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
void tick(){ 
	buttonInput = ~PINA & 0x03;
	switch(state) { // ACTIONS
		case Start: 
			number = 0x07;
			msCount = 0;
			LCD_ClearScreen();
			LCD_Cursor(1);
			LCD_WriteData( number + '0' );
			break;
		case noButton:
			msCount = 0;
			LCD_ClearScreen();
			LCD_Cursor(1);
			LCD_WriteData( number + '0' );
			break;
		case buttonOne:
			msCount = 0;
			number = number < 9? (number+1):9;
			LCD_ClearScreen();
			LCD_Cursor(1);
			LCD_WriteData(number + '0' );
			break;
		case buttonTwo:
			msCount = 0;
			number = number > 0? (number-1):0;
			LCD_ClearScreen();
			LCD_Cursor(1);
			LCD_WriteData( number + '0' );
			break;
		case bothButtons:
			msCount = 0;
			number = 0;
			LCD_ClearScreen();
			LCD_Cursor(1);
			LCD_WriteData( number + '0' );
			break;
		case increment:
			if (msCount >= 10) {
				number = number < 9? (number+1):9;
				LCD_ClearScreen();
				LCD_Cursor(1);
				LCD_WriteData(number + '0' );
				msCount = 0;
			} else {
				msCount++;
			}
			break;
		case decrement:
			if (msCount >= 10) {
				number = number > 0? (number-1):0;
				LCD_ClearScreen();
				LCD_Cursor(1);
				LCD_WriteData(number + '0' );
				msCount = 0;
			} else {
				msCount++;
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
}


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00; 
	DDRB = 0xFF; PORTB = 0x07; 
    DDRD = 0xFF; PORTD = 0x00; 
	state = Start;
	number = 0x00;
	buttonInput = 0x00;
	TimerSet(100);
	TimerOn();
	//B = 0x00;
	LCD_init();
    while (1){
		tick();
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
	}
}