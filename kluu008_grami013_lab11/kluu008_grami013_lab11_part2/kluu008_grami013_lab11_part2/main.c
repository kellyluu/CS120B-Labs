/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 11  Exercise 2
 *	Exercise Description: Task scheduler for message scrolling on LCD
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

typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;

enum LCD_States {sA, sB, sC, sD, sE, sF, sG, sH, sI, sJ, sK, sL, sM, sN, sO, sP, sQ, sR, sS, sT, sU, sV, sW} state;
unsigned char* message = NULL;

int SM1_Tick(){
	switch (state) {
		case sA: message = "CS120B is Legen"; state = sB; break;
		case sB: message = "S120B is Legend"; state = sC; break;
		case sC: message = "120B is Legend."; state = sD; break;
		case sD: message = "20B is Legend.."; state = sE; break;
		case sE: message = "0B is Legend..."; state = sF; break;
		case sF: message = "B is Legend... "; state = sG; break;
		case sG: message = " is Legend... w"; state = sH; break;
		case sH: message = "is Legend... wa"; state = sI; break;
		case sI: message = "s Legend... wai"; state = sJ; break;
		case sJ: message = " Legend... wait"; state = sK; break;
		case sK: message = "Legend... wait "; state = sL; break;
		case sL: message = "egend... wait f"; state = sM; break;
		case sM: message = "gend... wait fo"; state = sN; break;
		case sN: message = "end... wait for"; state = sO; break;
		case sO: message = "nd... wait for "; state = sP; break;
		case sP: message = "d... wait for i"; state = sQ; break;
		case sQ: message = "... wait for it"; state = sR; break;
		case sR: message = ".. wait for it "; state = sS; break;
		case sS: message = ". wait for it D"; state = sT; break;
		case sT: message = " wait for it DA"; state = sU; break;
		case sU: message = "wait for it DAR"; state = sV; break;
		case sV: message = "ait for it DARY"; state = sW; break;
		case sW: message = "it for it DARY!"; state = sA; break;
		default: message = "DEFAULT"; break;
	}
	LCD_ClearScreen();
	LCD_DisplayString(1, message);
	return state;
}

// Implement scheduler code from PES.
int main() {
	// Set Data Direction Registers
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
