/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 11  Exercise 3
 *	Exercise Description: Task scheduler for keypad and LCD
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

unsigned char x;
int SM1_Tick(int n) {
	x = GetKeypadKey();
	switch (x) {
		case '\0': break;
		case '1': LCD_DisplayString (1, "1");break;
		case '2': LCD_DisplayString (1, "2");break;
		case '3': LCD_DisplayString (1, "3");break;
		case '4': LCD_DisplayString (1, "4");break;
		case '5': LCD_DisplayString (1, "5");break;
		case '6': LCD_DisplayString (1, "6");break;
		case '7': LCD_DisplayString (1, "7");break;
		case '8': LCD_DisplayString (1, "8");break;
		case '9': LCD_DisplayString (1, "9");break;
		case 'A': LCD_DisplayString (1, "A");break;
		case 'B': LCD_DisplayString (1, "B");break;
		case 'C': LCD_DisplayString (1, "C");break;
		case 'D': LCD_DisplayString (1, "D");break;
		case '*': LCD_DisplayString (1, "*");break;
		case '0': LCD_DisplayString (1, "0");break;
		case '#': LCD_DisplayString (1, "#");break;
		default: LCD_DisplayString (1, " ");break;
	}
	return x;
}

int main() {
	// Set Data Direction Registers
	DDRC = 0xF0; PORTC = 0x0F; //keypad
	DDRA = 0xFF; PORTA = 0x00; //control lines
	DDRD = 0xFF; PORTD = 0x00; //data lines
	
	// Initializes the LCD display
	LCD_init();
	TimerSet(1);
	TimerOn();
	
	static task task1;
	task1.state = 0;//Task initial state.
	task1.period = 500;//Task Period.
	task1.elapsedTime = 500; //Task current elapsed time.
	task1.TickFct = &SM1_Tick;//Function pointer for the tick.
	task *tasks[] = { &task1 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
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
