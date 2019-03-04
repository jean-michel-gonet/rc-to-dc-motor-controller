#include "test.h"
#include "capture.h"

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

#define CAPTURE_FALLING_EDGE 4
#define CAPTURE_RAISING_EDGE 5

void initializeHardware() {
    // Inactivate A/D converters:
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    
    // Configure Fosc at 64Mhz
    OSCCONbits.IRCF = 7;
    OSCTUNEbits.PLLEN = 1;

    // At 64Mhz, Timer 5 counts up to 4000 in 2 ms
    T5CONbits.TMR5CS = 0;                     // Choose Fosc/4 as source.
    T5CONbits.T5CKPS = 3;                     // Frequency divisor at 1/8
    T5CONbits.TMR5ON = 1;                     // Enable Timer 5

    // Configure CCP5 as signal capture:
    TRISAbits.RA4 = 1;
    CCP5CONbits.CCP5M = CAPTURE_RAISING_EDGE; // Capture raising edge.
    CCPTMRS1bits.C5TSEL = 2;                  // CCP5 uses timer 5
    PIE4bits.CCP5IE = 1;                      // Enable interruptions.
    IPR4bits.CCP5IP = 0;                      // Low priority.
    
    // Enable interruptions:
    RCONbits.IPEN = 1;     // Let's have high / low priority interruptions.
    INTCONbits.GIEH = 1;   // Enable interrupts.
    INTCONbits.GIEL = 1;   // Enable low priority interrupts.
}

void __interrupt(low_priority) lowPriorityInterrupts(void) {
    CapturedPosition capturedPosition;
    
    // Just captured an edge:
    if (PIR4bits.CCP5IF) {
        PIR4bits.CCP5IF = 0;
        switch (CCP5CONbits.CCP5M) {
            case CAPTURE_RAISING_EDGE:
                CCP5CONbits.CCP5M = CAPTURE_FALLING_EDGE;
                captureRaisingEdge(5, CCPR5);
                break;
            case CAPTURE_FALLING_EDGE:
                CCP5CONbits.CCP5M = CAPTURE_RAISING_EDGE;
                captureFallingEdge(5, CCPR5, &capturedPosition);
                break;
        }
    }
}

void main(void) {
    initializeHardware();
    resetCapture();
    while(1);
}
#endif

#ifdef TEST
void main() {
    startTests();
    test_capture();
    finishTests();
    while(1);
}
#endif