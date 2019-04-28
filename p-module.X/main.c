#include "test.h"
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

#define CONVERSION_K 9646;

typedef struct {
    unsigned char lowerTwoBits : 2;
    unsigned char higherSixBits : 6;
} PR;

typedef union {
    PR pr;
    unsigned char value;
} DutyCycle;

DutyCycle predictDutyCycle(unsigned char ad) {
    DutyCycle dutyCycle;
    unsigned int value = CONVERSION_K;
    value /= ad;
    
    dutyCycle.value = value;
    return dutyCycle;
}

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

    // Configure timer 2 for a PWM frequency of 64*10^6 / (4 * 128) = 256kHz
    T2CONbits.TMR2ON = 1;     // Enable timer 2.
    T2CONbits.T2CKPS = 0;     // No clock pre-scaling (so 1:1).
    PR2 = 64;                 // Count up to 64.
    
    // Configure CCP2 as half-bridge PWM generator
    TRISBbits.RB3 = 0;        // RC3/P2A as output.
    TRISBbits.RB5 = 0;        // RB2/P2B as output.
    CCP2CONbits.P2M = 2;      // Half-bridge controller.
    CCP2CONbits.CCP2M = 12;   // P2A & P2B use positive logic.
    CCPTMRS0bits.C2TSEL = 0;  // CCP2 uses timer 2.
    CCPR2L = 0;               // Start by duty cycle of 0%

    // Enable interruptions:
    RCONbits.IPEN = 1;     // Let's have high / low priority interruptions.
    INTCONbits.GIEH = 1;   // Enable interrupts.
    INTCONbits.GIEL = 1;   // Enable low priority interrupts.
}

void __interrupt(low_priority) lowPriorityInterrupts(void) {
    DutyCycle dutyCycle;
    
    // Just captured an AD conversion:
    if (PIR1bits.ADIF) {
        PIR1bits.ADIF = 0;
        dutyCycle = predictDutyCycle(ADRESH);
        CCPR2L = dutyCycle.pr.higherSixBits;
        CCP2CONbits.DC2B = dutyCycle.pr.lowerTwoBits;
        ADCON0bits.GO = 1;
    }
}

void main(void) {
    initializeHardware();
    ADCON0bits.GO = 1;
    while(1);
}

#endif

#ifdef TEST

void can_calculate_duty_cycle() {
    DutyCycle dutyCycle;
    
    dutyCycle = predictDutyCycle(91);
    assertEquals("PDC001a", dutyCycle.value, 106);
    assertEquals("PDC001b", dutyCycle.pr.higherSixBits, 26);
    assertEquals("PDC001c", dutyCycle.pr.lowerTwoBits, 2);

    dutyCycle = predictDutyCycle(53);
    assertEquals("PDC002a", dutyCycle.value, 182);
    assertEquals("PDC002b", dutyCycle.pr.higherSixBits, 45);
    assertEquals("PDC002c", dutyCycle.pr.lowerTwoBits, 2);

    dutyCycle = predictDutyCycle(113);
    assertEquals("PDC003a", dutyCycle.value, 85);
    assertEquals("PDC003b", dutyCycle.pr.higherSixBits, 21);
    assertEquals("PDC003c", dutyCycle.pr.lowerTwoBits, 1);
}

void main(void) {
    startTests();
    can_calculate_duty_cycle();
    finishTests();
    while(1);
}
#endif