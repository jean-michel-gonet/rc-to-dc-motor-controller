/*
 * File:   main.c
 * Author: jmgonet
 *
 * Created on March 3, 2019, 8:14 AM
 */

#include <xc.h>

/**
 * Configuration bits
 */
#pragma config FOSC = INTIO67  // Use internal oscillator. A6 and A7 are IO.
#pragma config IESO = OFF      // No oscillator at startup.
#pragma config FCMEN = OFF     // Don't monitor oscillator.
 
// Those are mandatory to use ICSP / ICD:
#pragma config MCLRE = EXTMCLR // RE3 is active as master reset.
#pragma config WDTEN = OFF     // Watchdog disabled.
#pragma config LVP = OFF       // Single Supply Enable bits off.


void main(void) {
    int n;
    for (n = 0; n < 100; n++) {
        n++;
    }
}
