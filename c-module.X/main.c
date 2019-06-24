#include "test.h"
#include "capture.h"
#include "queue.h"
#include "events.h"
#include "dashboard.h"
#include "power.h"

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
    
    // Configure timer 2 for a PWM frequency of 64*10^6 / (4 * 255) = 64kHz
    T2CONbits.TMR2ON = 1;     // Enable timer 2.
    T2CONbits.T2CKPS = 0;     // No clock pre-scaling (so 1:1).
    PR2 = 255;                // Count up to 255.
    
    // Configure CCP1 as full-bridge PWM generator
    TRISCbits.RC2 = 0;        // RC2/P1A as output.
    TRISBbits.RB2 = 0;        // RB2/P1B as output.
    TRISBbits.RB1 = 0;        // RB1/P1C as output.
    TRISBbits.RB4 = 0;        // RB4/P1D as output.
    CCP1CONbits.CCP1M = 12;   // P1A, P1B, P1C, P1D with positive logic.
    CCPTMRS0bits.C1TSEL = 0;  // CCP1 uses timer 2.
    CCPR1L = 0;               // Start by duty cycle of 0%
    CCP1CONbits.P1M = POSITION_POSITIVE;
    
    // Enable interruptions:
    RCONbits.IPEN = 1;     // Let's have high / low priority interruptions.
    INTCONbits.GIEH = 1;   // Enable interrupts.
    INTCONbits.GIEL = 1;   // Enable low priority interrupts.
}

void __interrupt(low_priority) lowPriorityInterrupts(void) {
    
    // Just captured an edge:
    if (PIR4bits.CCP5IF) {
        PIR4bits.CCP5IF = 0;
        switch (CCP5CONbits.CCP5M) {
            case CAPTURE_RAISING_EDGE:
                CCP5CONbits.CCP5M = CAPTURE_FALLING_EDGE;
                captureRaisingEdge(CCPR5);
                break;
            case CAPTURE_FALLING_EDGE:
                CCP5CONbits.CCP5M = CAPTURE_RAISING_EDGE;
                captureFallingEdge(CCPR5, &(dashboard.capturedPosition));
                enqueueEvent(RC_POSITION_CAPTURED, 0);
                break;
        }
    }
}

void main(void) {
    Event* event;
    
    initializeHardware();
    resetCapture();
    
    while(1) {
        event = dequeueEvent();
        if (event != 0) {
            do {
                power(event);
                leds(event);
                event = dequeueSubsequentEvent();
            } while (event != 0);
        }
    }
}
#endif

#ifdef TEST
void main() {
    startTests();

    test_capture();
    test_queue();
    test_events();
    test_power();

    finishTests();

    while(1);
}
#endif
