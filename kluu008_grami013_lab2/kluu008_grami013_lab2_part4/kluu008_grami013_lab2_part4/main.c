/*	Partner(s) Name & E-mail: Kelly Luu kluu008@ucr.edu, Guillermo Ramirez grami013@ucr.edu
 *	Lab Section: 24
 *	Assignment: Lab 2  Exercise 4
 *	Exercise Description: amusement park ride
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
	/* An amusement park kid ride cart has three seats, with 8-bit weight sensors connected to ports A, B, and C (measuring from 0-255 kilograms). 
	Set PD0 to 1 if the cart's total passenger weight exceeds the maximum of 140 kg. 
	Also, the cart must be balanced: Set port PD1 to 1 if the difference between A and C exceeds 80 kg. 
	Can you also devise a way to inform the ride operator of the approximate weight using the remaining bits on D? 
	(Interesting note: Disneyland recently redid their "It's a Small World" ride because the average passenger weight has increased over the years, 
	causing more boats to get stuck on the bottom).
	(Hint: Use two intermediate variables to keep track of weight, one of the actual value and another being the shifted weight. 
	Binary shift right by one is the same as dividing by two and binary shift left by one is the same as multiplying by two.)
*/
	// three seats
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0x00; PORTC = 0xFF;
	// output
	DDRD = 0xFF; PORTD = 0x00;
	// constants
	const unsigned char weightLimit = 140;
	const unsigned char weightDifference = 80;
	//variables
	unsigned char seatWeight1 = 0;
	unsigned char seatWeight2;
	unsigned char seatWeight3 = 0x00;
	unsigned short totalWeight = 0;
	unsigned char shiftedWeight = 0;
	unsigned char numHundreds = 0;
	unsigned char numTens = 0;
	unsigned char seatError1 = 0x00;
	unsigned char seatError2 = 0x00;
	while(1){
		seatError1 = 0x00;
		seatError2 = 0x00;
		totalWeight = 0;
		
		seatWeight1 = PINA;
		seatWeight2 = PINB;
		seatWeight3 = PINC;
		totalWeight = seatWeight1 + seatWeight2 + seatWeight3;

		// check if total weight is greater than 140
		if (totalWeight > weightLimit){
			seatError1 = 0x01;
		}
		if(seatWeight1 > seatWeight3){
			if(seatWeight1 - seatWeight3 > weightDifference){
				seatError2 = 0x02;
			}
		}else{
			if(seatWeight3 - seatWeight1 > weightDifference){
				seatError2 = 0x02;
			}
		}
		//set upper 5 bits for weight
		//first 2 bits for 100s
		//next 3 bits for 10s
		numHundreds = totalWeight / 100;
		numTens = (totalWeight - (numHundreds * 100)) / 10;
		shiftedWeight = (numHundreds << 6) | (numTens << 3);
	
		if(seatError1 || seatError2){
			PORTD = (seatError1 + seatError2) | shiftedWeight;
		}else{
			PORTD = shiftedWeight;
		}
	}
}
