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

#ifndef TEST

void initializeHardware() {
    // Configure Fosc at 64Mhz
    OSCCONbits.IRCF = 7;
    OSCTUNEbits.PLLEN = 1;

    // Activate A/D converters AN16 and AN17:
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    ANSELCbits.ANSC5 = 1; // RC5 (AN17)
    ANSELCbits.ANSC4 = 1; // RC4 (AN16)
    
    ADCON2bits.ADFM = 0;   // Conversion result is left justified on ADRESH
    ADCON2bits.ACQT = 0;   // No reason to wait after GO/DONE is set.
    ADCON2bits.ADCS = 6;   // TAD of 1µS when Fosc is at 64MHz

    ADCON0bits.CHS = 17;   // Let's acquire first the VCC value.
    ADCON0bits.ADON = 1;   // Turn it on.
    
    IPR1bits.ADIP = 0;     // Low priority interrupt.
    PIE1bits.ADIE = 1;     // Enable interrupts.
    
    // Enable interruptions:
    RCONbits.IPEN = 1;     // Let's have high / low priority interruptions.
    INTCONbits.GIEH = 1;   // Enable interrupts.
    INTCONbits.GIEL = 1;   // Enable low priority interrupts.
}

void __interrupt(low_priority) lowPriorityInterrupts(void) {

    // Just captured an AD conversion:
    if (PIR1bits.ADIF) {
        PIR1bits.ADIF = 0;
        CCPR1L = ADRESH;
        ADCON0bits.GO = 1;
    }
}


void main(void) {
    initializeHardware();
    ADCON0bits.GO = 1;
    while(1);
}

#endif