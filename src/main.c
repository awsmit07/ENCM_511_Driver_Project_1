/*
 * File:   main.c
 * Author: Andreas Smit
 * UCID: 30065542
 *
 * Created on September 11, 2022, 7:12 PM
 */


#include "xc.h"
#include "bits.h"

// Disable the primary oscillator to use RA3 as an input
#pragma config POSCMOD = NONE
#pragma config OSCIOFNC = ON

// Set the PGx3 port as the programmer
#pragma config ICS = PGx3 

// Combinational logic to compute UCID digits. Truth Table:
// x3 x2 x1 x0 | Y3 Y2 Y1 Y0
//-------------|-------------
// 0  0  0  0  | 0  0  0  0
// 0  0  0  1  | 0  0  1  1
// 0  0  1  0  | 0  0  0  0
// 0  0  1  1  | 0  0  0  0
// 0  1  0  0  | 0  1  1  0
// 0  1  0  1  | 0  1  0  1
// 0  1  1  0  | 0  1  0  1
// 0  1  1  1  | 0  1  0  0
// 1  0  0  0  | 0  0  1  0
// 1  0  0  1  | 0  0  0  0
// 1  0  1  0  | 0  0  0  0
// 1  0  1  1  | 0  0  0  0
// 1  1  0  0  | 0  0  0  0
// 1  1  0  1  | 0  0  0  0
// 1  1  1  0  | 0  0  0  0
// 1  1  1  1  | 0  0  0  0
#define Y3(x3, x2, x1, x0) 0
#define Y2(x3, x2, x1, x0) ((x2) && !(x3))
#define Y1(x3, x2, x1, x0) (((x3) && !(x2) && !(x1) && !(x0)) || (!(x3) && (x2) && !(x1) && !(x0)) || (!(x3) && !(x2) && !(x1) && (x0)))
#define Y0(x3, x2, x1, x0) ((!(x3) && !(x1) && (x0)) || (!(x3) && (x2) && (x1) && !(x0)))

// Shifter to write sequential bits of the B latch. 
#define ENCODE_LATB_BITS(data) (((data) & 0x7) | (((data) & 0x8)<<1) | (((data) & 0x70)<<3) | (((data) & 0x780)<<5))

int main(void) 
{
    // Set PORT A<3:0> as inputs and PORT B as output
    TRISA = 0x000f;
    AD1PCFG = 0xDC3F; // Configure all AN pins as digital inputs for use on hardware
    TRISB = 0x0000;
    
    // Verify LATB starts as all 0s
    LATB = 0;
    
    // Create a bitfield for storing the input data. 
    union X 
    {
        uint8_t data;
        struct
        {
            uint8_t x0:1;
            uint8_t x1:1;
            uint8_t x2:1;
            uint8_t x3:1;
        };
    } x = {0};
    
    // Create a bitfield for storing the output data.
    union Y
    {
        uint8_t data;
        struct
        {
            uint8_t y0:1;
            uint8_t y1:1;
            uint8_t y2:1;
            uint8_t y3:1;
        };
    } y = {0};
    
    while(1)
    {
        // Optional heartbeat bit for debugging
        // LATBbits.LATB15 = 0;
        
        // Read inputs from PORTA
        x.data = PORTA;
        
        // Compute output data
        y.y0 = Y0(x.x3, x.x2, x.x1, x.x0);
        y.y1 = Y1(x.x3, x.x2, x.x1, x.x0);
        y.y2 = Y2(x.x3, x.x2, x.x1, x.x0);
        y.y3 = Y3(x.x3, x.x2, x.x1, x.x0);
        
        // Set data out on PORTB
        uint16_t num_bits = SET_N_BITS(y.data);
        LATB = ENCODE_LATB_BITS(num_bits);
        
        // Heartbeat 
        // LATBbits.LATB15 = 1;
    }
    
    return 0;
}
