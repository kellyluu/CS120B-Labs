/*
 * sample.c
 *
 * Created: 1/11/2019 8:53:47 PM
 * Author : Kelly
 */ 

#include <avr/io.h>
/*An amusement park kid ride cart has three seats, with 8-bit weight sensors connected to ports A, B, and C (measuring from 0-255 kilograms). 
Set PD0 to 1 if the cart's total passenger weight exceeds the maximum of 140 kg. 
Also, the cart must be balanced: Set port PD1 to 1 if the difference between A and C exceeds 80 kg. 
Can you also devise a way to inform the ride operator of the approximate weight using the remaining bits on D? 
(Interesting note: Disneyland recently redid their "It's a Small World" ride because the average passenger weight has increased over the years, 
causing more boats to get stuck on the bottom).

(Hint: Use two intermediate variables to keep track of weight, one of the actual value and another being the shifted weight. 
Binary shift right by one is the same as dividing by two and binary shift left by one is the same as multiplying by two.)
*/

int main(void) {
//inputs	
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0x00; PORTC = 0xFF;
//output
    DDRD = 0xFF; PORTD = 0x00;
//variables
	unsigned char weightA = 0x00;
	unsigned char weightB = 0x00;
	unsigned char weightC = 0x00;
	unsigned short totalWeight = 0x0;
	unsigned char weightExceeded = 0x00;
	unsigned char unbalanced = 0x00;
//constants
	const unsigned char maxWeight = 0x8C;
	const unsigned char maxDifference = 0x50;
    while (1) {
		weightA = PINA & 0xFF;
		weightB = PINB & 0xFF;
		weightC = PINC & 0xFF;
		totalWeight = weightA + weightB + weightC;
		weightExceeded = 0x00;
		unbalanced = 0x00;
		//check if total > 140
		if (totalWeight > maxWeight) {
			weightExceeded = 0x01;
		}
		//check if unbalanced
		if (weightA > weightC) {
			if ((weightA - weightC) > maxDifference) {
				unbalanced = 0x02;
			}
		}
		else {
			if ((weightC - weightA) > maxDifference) {
				unbalanced = 0x02;
			}
		}
		PORTD = weightExceeded | unbalanced;
    }
}

