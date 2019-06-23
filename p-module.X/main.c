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

#define CONVERSION_K 10400
#define DEAD_TIME 8
#define HISTERESIS 15

typedef struct {
    unsigned char lowerTwoBits : 2;
    unsigned char higherSixBits : 6;
} PR;

typedef union {
    PR pr;
    unsigned char value;
} DutyCycle;

unsigned char maxDuty

DutyCycle predictDutyCycle(unsigned char ad) {
    DutyCycle dutyCycle;
    unsigned int value = CONVERSION_K;
    value /= ad;

    if (value > 60) {
        value = 57;
    }
    
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
    
    // Configure CCP1 as half-bridge PWM generator
    TRISBbits.RB2 = 0;            // RB2/P1B as output.
    TRISCbits.RC2 = 0;            // RC2/P2B as output.
    CCP1CONbits.P1M = 2;          // Half-bridge controller.
    CCP1CONbits.CCP1M = 12;       // P1A & P1B use positive logic.
    CCPTMRS0bits.C1TSEL = 0;      // CCP2 uses timer 2.
    PWM1CONbits.P1DC = DEAD_TIME; // Have a small dead time.
    CCPR1L = 0;                   // Start by duty cycle of 0%

    // Enable timer 3 to have 2kHz interruptions
    T1CONbits.T1CKPS = 0;
    T1CONbits.TMR1ON = 1;
    PIE1bits.TMR1IE = 1;
    IPR1bits.TMR1IP = 0;
    
    // Enable interruptions:
    RCONbits.IPEN = 1;     // Let's have high / low priority interruptions.
    INTCONbits.GIEH = 1;   // Enable interrupts.
    INTCONbits.GIEL = 1;   // Enable low priority interrupts.
}

typedef enum {
    MEASURE_VIN,
    MEASURE_VOUT
} MeasureInputEnum;

void __interrupt(low_priority) lowPriorityInterrupts(void) {
    DutyCycle dutyCycle;
    static unsigned char lastVo;
    static MeasureInputEnum  measureInput = MEASURE_VIN;

    // Time to launch an AD conversion:
    if (PIR1bits.TMR1IF) {
        TMR1H = 0xE0;
        TMR1L = 0xBF;
        PIR1bits.TMR1IF = 0;
        
        switch (measureInput) {
            case MEASURE_VIN:
                ADCON0bits.CHS = 17;
                break;

            case MEASURE_VOUT:
            default:
                ADCON0bits.CHS = 16;
                break;
        }

        ADCON0bits.GO = 1;
    }

    // Just captured an AD conversion:
    if (PIR1bits.ADIF) {
        PIR1bits.ADIF = 0;
        switch (measureInput) {
            case MEASURE_VIN:
                dutyCycle = predictDutyCycle(ADRESH);
                CCPR1L = DEAD_TIME + dutyCycle.pr.higherSixBits;
                CCP1CONbits.DC1B = dutyCycle.pr.lowerTwoBits;
                measureInput = MEASURE_VOUT;
                break;

            case MEASURE_VOUT:
            default:
                lastVo = ADRESH;
                measureInput = MEASURE_VIN;
                break;
        }
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
    assertEquals("PDC001a", dutyCycle.value, 114);
    assertEquals("PDC001b", dutyCycle.pr.higherSixBits, 28);
    assertEquals("PDC001c", dutyCycle.pr.lowerTwoBits, 2);

    dutyCycle = predictDutyCycle(53);
    assertEquals("PDC002a", dutyCycle.value, 196);
    assertEquals("PDC002b", dutyCycle.pr.higherSixBits, 49);
    assertEquals("PDC002c", dutyCycle.pr.lowerTwoBits, 0);

    dutyCycle = predictDutyCycle(113);
    assertEquals("PDC003a", dutyCycle.value, 92);
    assertEquals("PDC003b", dutyCycle.pr.higherSixBits, 23);
    assertEquals("PDC003c", dutyCycle.pr.lowerTwoBits, 0);
}

void main(void) {
    startTests();
    can_calculate_duty_cycle();
    finishTests();
    while(1);
}
#endif