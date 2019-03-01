/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 11  Exercise 4
 *	Exercise Description: Task scheduler for keypad and LCD: CONGRATULATIONS!
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include <stdio.h>
#include "bit.h"
#include "io.c"

unsigned char GetKeypadKey() {
	PORTC = 0xEF; // Enable col 4 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('1'); }
	if (GetBit(PINC,1)==0) { return('4'); }
	if (GetBit(PINC,2)==0) { return('7'); }
	if (GetBit(PINC,3)==0) { return('*'); }
	// Check keys in col 2
	PORTC = 0xDF; // Enable col 5 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('2'); }
	if (GetBit(PINC,1)==0) { return('5'); }
	if (GetBit(PINC,2)==0) { return('8'); }
	if (GetBit(PINC,3)==0) { return('0'); }
	// ... *****FINISH*****
	// Check keys in col 3
	PORTC = 0xBF; // Enable col 6 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('3'); }
	if (GetBit(PINC,1)==0) { return('6'); }
	if (GetBit(PINC,2)==0) { return('9'); }
	if (GetBit(PINC,3)==0) { return('#'); }
	// Check keys in col 4
	PORTC = 0x7F; // Enable col 7 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('A'); }
	if (GetBit(PINC,1)==0) { return('B'); }
	if (GetBit(PINC,2)==0) { return('C'); }
	if (GetBit(PINC,3)==0) { return('D'); }
	return('\0'); // default value
}

typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;

unsigned char input; //temp variable to hold keypad input
unsigned char character;  //character to print to LCD
unsigned char column; //position on LCD

enum LCD_states {start, waitLCD, updateLCD} LCD_state;
	
int LCD_Tick(int n) {
	switch (LCD_state) { //actions
		case start:
			LCD_DisplayString(1, "Congratulations!");
			LCD_Cursor(1);
			break;
		case waitLCD:
			break;
		case updateLCD:
			if (input != '\0') {
				character = input;
				LCD_WriteData(character);
				if (column < 16) {
					column++;
					LCD_Cursor(column);
				}
				else {
					column = 1;
					LCD_Cursor(column);
				}
			}
			break;
		default:
			break;
	} //end actions
	switch (LCD_state) { //transitions
		case start:
			LCD_state = waitLCD;
			break;
		case waitLCD:
			LCD_state = (input != '\0') ? updateLCD : waitLCD;
			break;
		case updateLCD:
			LCD_state = (character == '\0') ? waitLCD : updateLCD; 
			break;
		default:
			break;
	}
		return LCD_state;
}

enum keypad_states {waitPress, press, waitRelease} keypad_state;
int keypad_Tick(int n) {
	switch(keypad_state) { //actions
		case waitPress:
			input = GetKeypadKey();
			break;
		case press:
			input = GetKeypadKey();
			character = input;
			break;
		case waitRelease:
			input = GetKeypadKey();
			character = '\0';
			break;
		default:
			break;	
	} //end actions
	switch(keypad_state) { //transitions
		case waitPress:
			keypad_state = (input != '\0') ? press : waitPress;
			break;
		case press:
			keypad_state = waitRelease; //(input != '\0') ? 
			break;
		case waitRelease:
			keypad_state = (input != '\0') ? waitRelease: waitPress;
			break;
		default:
			keypad_state = waitPress;
			break;
	} //end transitions
	return keypad_state;
}


int main() {
	// Set Data Direction Registers
	DDRC = 0xF0; PORTC = 0x0F; //keypad
	DDRA = 0xFF; PORTA = 0x00; //control lines
	DDRD = 0xFF; PORTD = 0x00; //data lines
	
	// Initializes the LCD display
	LCD_init();

	static task task1, task2;
	task *tasks[] = { &task1, &task2 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	// LCD_task
	task1.state = -1; //Task initial state.
	task1.period = 50;//Task Period.
	task1.elapsedTime = 50; //Task current elapsed time.
	task1.TickFct = &LCD_Tick;//Function pointer for the tick.
	// keypad_task
	task2.state = -1; //Task initial state.
	task2.period = 25;//Task Period.
	task2.elapsedTime = 25; //Task current elapsed time.
	task2.TickFct = &keypad_Tick;//Function pointer for the tick.
	
	TimerSet(1);
	TimerOn();
	character = NULL;
	input = NULL;
	while(1) {
	for ( unsigned short i = 0; i < numTasks; i++ ) {
		if ( tasks[i]->elapsedTime == tasks[i]->period ) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += 1;
	}
	while(!TimerFlag);
	TimerFlag = 0;
	}
	return 0;
}
