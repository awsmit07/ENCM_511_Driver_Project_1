/*
 * File:   main.c
 * Author: Andreas Smit
 * UCID: 30065542
 *
 * Created on September 11, 2022, 7:12 PM
 */


#include "xc.h"

// Disable the primary oscillator to use RA3 as an input
#pragma config POSCMOD = NONE
#pragma config OSCIOFNC = ON

// Set the PGx3 port as the programmer
#pragma config ICS = PGx3 

int main(void) 
{
    // Set PORT A<3:0> as inputs and PORT B as output
    TRISA = 0x000f;
    AD1PCFG = 0xDC3F; // Configure all AN pins as digital inputs for use on hardware
    TRISB = 0x0000;
    
    return 0;
}
