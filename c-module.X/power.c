#include "events.h"
#include "dashboard.h"
#include "power.h"
#include "test.h"

#include <xc.h>

#define COUNTING_NEUTRAL 100
#define NEUTRAL_POSITION 10

typedef enum POWER_STATE {
    WAITING_FOR_NEUTRAL,
    ACTIVE
} PowerState;

PowerState status = WAITING_FOR_NEUTRAL;
unsigned char countingNeutral = COUNTING_NEUTRAL;

void resetPower() {
    status = WAITING_FOR_NEUTRAL;
    countingNeutral = COUNTING_NEUTRAL;
}

void power(Event *event) {
    EventType eventType = event->event;
    unsigned char position = dashboard.capturedPosition.position;
    PositionSign sign = dashboard.capturedPosition.sign;

    switch(status) {
        case ACTIVE:
            switch(eventType) {
                case RC_POSITION_CAPTURED:
                    CCPR1L = position;
                    CCP1CONbits.P1M = sign;
                    if (position < NEUTRAL_POSITION) {
                        dashboard.signal = SIGNAL_NEUTRAL;                        
                    } else {
                        if (sign == POSITION_POSITIVE) {
                            dashboard.signal = SIGNAL_MOVING_NEGATIVE;
                        } else {
                            dashboard.signal = SIGNAL_MOVING_POSITIVE;
                        }
                    }
                    break;

                case RC_NO_POSITION:
                    CCPR1L = 0;
                    status = WAITING_FOR_NEUTRAL;
                    countingNeutral = COUNTING_NEUTRAL;
                    break;

                default:
                    break; // Do nothing.
            }
            break;

        case WAITING_FOR_NEUTRAL:
            CCPR1L = 0;
            dashboard.signal = SIGNAL_WAITING_FOR_NEUTRAL;
            switch (eventType) {
                case RC_POSITION_CAPTURED:
                    if (position < NEUTRAL_POSITION) {
                        countingNeutral--;
                        if (countingNeutral == 0) {
                            status = ACTIVE;
                        }
                    }
                    break;

                case RC_NO_POSITION:
                    countingNeutral = COUNTING_NEUTRAL;
                    break;
                    
                default:
                    break; // Do nothing.
            }
    }
}

#ifdef TEST
void test_power() {
    Event rcPositionCaptured;
    Event rcNoPosition;
    int n;
    
    rcPositionCaptured.event = RC_POSITION_CAPTURED;
    
    resetPower();
    
    dashboard.capturedPosition.sign = POSITION_POSITIVE;
    dashboard.capturedPosition.position = NEUTRAL_POSITION;
    for (n = 0; n < COUNTING_NEUTRAL * 2; n++) {
        power(&rcPositionCaptured);
    }
    assertEquals("POWN001", CCPR1L, 0);

    dashboard.capturedPosition.position = NEUTRAL_POSITION - 1;
    for (n = 0; n < COUNTING_NEUTRAL; n++) {
        power(&rcPositionCaptured);
    }
    assertEquals("POWN002a", dashboard.signal, SIGNAL_NOT_MOVING);
    assertEquals("POWN002b", CCPR1L, 0);

    dashboard.capturedPosition.position = 100;
    power(&rcPositionCaptured);
    assertEquals("POWN003a", dashboard.signal, MOVING_POSITIVE);
    assertEquals("POWN003b", CCPR1L, 100);    
    
    rcNoPosition.event = RC_NO_POSITION;
    power(&rcNoPosition);
    assertEquals("POWN004", CCPR1L, 0);
    
    dashboard.capturedPosition.position = 100;
    power(&rcPositionCaptured);
    assertEquals("POWN005a", dashboard.signal, NOT_MOVING);
    assertEquals("POWN005b", CCPR1L, 0);
}
#endif