/*	Author: amidd001
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
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

enum SM_States {SM_Init, SM_PA0, SM_PA1, SM_Both, SM_None} SM_State;

void TickFct_Press()
{
	unsigned char tmpA = ~PINA & 0x03;
	unsigned char increaseHeld = 0x00;
	unsigned char decreaseHeld = 0x00;
	switch(SM_State)
	{
		unsigned char tmpA = ~PINA & 0x03;
		case(SM_Init):
			if(tmpA == 0x00)
			{
				SM_State = SM_None;
			}
			else if(tmpA == 0x01)
			{
				SM_State = SM_PA0;
			}
			else if(tmpA == 0x02)
			{
				SM_State = SM_PA1;
			}
			else if(tmpA == 0x03)
			{
				SM_State = SM_Both;
			}
			break;
		case(SM_PA0):
			if(tmpA == 0x00)
			{
				SM_State = SM_None;
			}
			else if(tmpA == 0x01)
			{
				increaseHeld = 0x01;
			}
			else if(tmpA == 0x02)
			{
				SM_State = SM_PA1;
			}
			else if(tmpA == 0x03)
			{
				SM_State = SM_Both;
			}
			break;
		case(SM_PA1):
			if(tmpA == 0x00)
                        {
                                SM_State = SM_None;
                        }
                        else if(tmpA == 0x01)
                        {
                                SM_State = SM_PA0;
                        }
                        else if(tmpA == 0x02)
                        {
				decreaseHeld = 0x01;
                        }
                        else if(tmpA == 0x03)
                        {
                                SM_State = SM_Both;
                        }
			break;
		case(SM_Both):
			if(tmpA == 0x00)
                        {
                                SM_State = SM_None;
                        }
                        else if(tmpA == 0x01)
                        {
                                SM_State = SM_PA0;
                        }
                        else if(tmpA == 0x02)
                        {
                                SM_State = SM_PA1;
                        }
                        else if(tmpA == 0x03)
                        {
                                SM_State = SM_Both;
                        }
			break;
		case(SM_None):
			if(tmpA == 0x00)
                        {
                                SM_State = SM_None;
                        }
                        else if(tmpA == 0x01)
                        {
                                SM_State = SM_PA0;
                        }
                        else if(tmpA == 0x02)
                        {
                                SM_State = SM_PA1;
                        }
                        else if(tmpA == 0x03)
                        {
                                SM_State = SM_Both;
                        }
			break;

	}

	switch(SM_State)
	{
		case(SM_Init):
			PORTC = 0x07;
			break;
		case(SM_PA0):
			decreaseHeld = 0x00;
			if(increaseHeld == 0x00)
			{
				if(PORTC < 0x09)
				{
					PORTC = PORTC + 0x01;
				}
			}
			//while(tmpA == 0x01); //pauses the state machine
			break;
		case(SM_PA1):
			increaseHeld = 0x00;
			if(decreaseHeld == 0x00)
			{
				if(PORTC > 0x00)
				{
					PORTC = PORTC - 0x01;
				}
			}
			//while(tmpA == 0x02); //pauses the state machine
			break;
		case(SM_Both):
			PORTC = 0x00;
			break;
		case(SM_None):
			increaseHeld = 0x00;
			decreaseHeld = 0x00;
			break;

	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF; //sets PINA as input
    DDRB = 0xFF; PORTB = 0x00; //sets PORTB as output
    DDRC = 0xFF; PORTC = 0x00; //sets PORTC as output
    /* Insert your solution below */
    SM_State = SM_Init;
    PORTB = 0x07;
    TimerSet(100);
    TimerOn();
    unsigned char incrementPressed = 0x00;
    unsigned char decrementPressed = 0x00;
  //  unsigned char tmpA;
    while (1) {
//	tmpA = ~PINA & 0x03;
	TickFct_Press();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
