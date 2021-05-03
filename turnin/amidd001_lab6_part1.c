/*	Author: amidd001
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *	Demo Video: https://youtu.be/wlS8gkBw744
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn()
{
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff()
{
	TCCR1B = 0x00;
}

void TimerISR()
{
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect)
{
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0)
	{
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M)
{
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


enum BL_States {BL_Init, BL_One, BL_Two, BL_Three} BL_STATE;

void TickFct_Blink()
{
	switch(BL_STATE)
	{
		case(BL_Init):
			BL_STATE = BL_One;
			break;
		case(BL_One):
			BL_STATE = BL_Two;
			break;
		case(BL_Two):
			BL_STATE = BL_Three;
			break;
		case(BL_Three):
			BL_STATE = BL_One;
			break;
		default:
			break;

	}

	switch(BL_STATE)
	{
		case(BL_Init):
			PORTB = 0x01; //PB0
			break;
		case(BL_One):
			PORTB = 0x01; //PB0
			break;
		case(BL_Two):
			PORTB = 0x02; //PB1
			break;
		case(BL_Three):
			PORTB = 0x04; //PB2
		     	break;
		default:
			break;
	}
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
	unsigned char tmpB = 0x00;
	PORTB = 0x00;
	BL_STATE = BL_Init;
    /* Insert your solution below */
    while (1)
    {
	   TickFct_Blink();
	   while(!TimerFlag);
	   TimerFlag = 0;
    }
    return 1;
}
