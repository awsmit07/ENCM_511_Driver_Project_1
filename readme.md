# ENCM 511 Driver Project 1
## Andreas Smit, 30065542
Driver project 1 for ENCM 511 Embedded System Interfacing.

## Project Details
Create a program that takes a 4 bit binary number, n, as input and
outputs the nth digit of your UCID on 9 LEDs
Input | Output
------|-------
 0000 | 0000
 0001 | First digit of UCID
 0010 | Second digit of UCID
 0011 | Third digit of UCID
 0100 | Fourth digit of UCID
 0101 | Fifth digit of UCID
 0110 | Sixth digit of UCID
 0111 | Seventh digit of UCID
 1000 | eighth digit of UCID
 1001 | 0000
 1010 | 0000
 1011 | 0000
 1100 | 0000
 1101 | 0000
 1110 | 0000
 1111 | 0000

## Design
We can easily make a truth table of inputs to output with the digits of
my UCID number (30065542)
Input | UCID Digit
------|----------
 0000 | 0000
 0001 | 0011 (3)
 0010 | 0000 (0)
 0011 | 0000 (0)
 0100 | 0110 (6)
 0101 | 0101 (5)
 0110 | 0101 (5)
 0111 | 0100 (4)
 1000 | 0010 (2)
 1001 | 0000
 1010 | 0000
 1011 | 0000
 1100 | 0000
 1101 | 0000
 1110 | 0000
 1111 | 0000

From the truth table we can make a combinational logic functions to
compute each bit of the UCID digit.
\[y_0 = \bar{x_3}\bar{x_1}x_0 + \bar{x_3}x_2x_1\bar{x_0}\]
\[y_1 = x_3\bar{x_2}\bar{x_1}\bar{x_0} + \bar{x_3}x_2\bar{x_1}\bar{x_0} + \bar{x_3}\bar{x_2}\bar{x_1}x_0\]
\[y_2 = x_2\bar{x_3}\]
\[y_3 = 0\]

We can then use a bit shift to expand this to turn on all the bits we
need for all the LEDs
```c
#define SET_N_BITS(n) ((1 << (n)) - 1)
```
We can use multiple bit fields to make use and computation of the UCID
digit from the input easy.
