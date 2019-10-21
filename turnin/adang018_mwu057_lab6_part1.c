/*	Author: adang018
 *      Partner(s) Name: Min-Hua Wu
 *	Lab Section: 022
 *	Assignment: Lab #6  Exercise #1
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

void TimerOn() {
    TCCR1B = 0x0B;
    OCR1A = 125;
    TIMSK1 = 0x02;
    TCNT1 = 0;
    _avr_timer_cntcurr = _avr_timer_M;
    SREG |= 0x80;
}

void TimerOff() {
    TCCR1B = 0x00;
}

void TimerISR() {
    TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
    _avr_timer_cntcurr--;
    if(_avr_timer_cntcurr == 0) {
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M) {
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}

void Tick();

enum States {Init, NextLED} state;
unsigned char out;

void main() {
    DDRB = 0xFF;
    PORTB = 0x00;
        
    state = Init;
    out = 0x01;
    PORTB = out;

    TimerSet(1000);
    TimerOn();
    
    while(1) {
        Tick();
        while (!TimerFlag);
        TimerFlag = 0;    
    } 
}

void Tick() {
    switch(state) {
        case Init:
            state = NextLED;
            break;
        case NextLED:
            state = NextLED;
            break;
        default:
            state = Init;
            break;
    }
    
    switch(state) {
        case Init:
            out = 0x01;
            break;
        case NextLED:
            out = (out == 0x04) ? 0x01 : out << 1;
            break;
        default: 
            out = 0x01;
            break;    
    }
    PORTB = out;
}
