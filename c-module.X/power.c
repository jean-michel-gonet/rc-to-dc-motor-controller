#include "events.h"
#include "dashboard.h"
#include "power.h"
#include "test.h"

#include <xc.h>

#define COUNTING_NEUTRAL 100
#define NEUTRAL_POSITION 5

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

    switch(status) {
        case ACTIVE:
            switch(eventType) {
                case RC_POSITION_CAPTURED:
                    CCPR1L = position;
                    CCP1CONbits.P1M = dashboard.capturedPosition.sign;
                    break;

                default:
                case RC_NO_POSITION:
                    CCPR1L = 0;
                    status = WAITING_FOR_NEUTRAL;
                    countingNeutral = COUNTING_NEUTRAL;
                    break;
            }
            break;

        case WAITING_FOR_NEUTRAL:
            CCPR1L = 0;
            switch (eventType) {
                case RC_POSITION_CAPTURED:
                    if (position < NEUTRAL_POSITION) {
                        countingNeutral--;
                        if (countingNeutral == 0) {
                            status = ACTIVE;
                        }
                    }
                    break;

                default:
                case RC_NO_POSITION:
                    countingNeutral = COUNTING_NEUTRAL;
                    break;
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
    
    dashboard.capturedPosition.position = NEUTRAL_POSITION;
    for (n = 0; n < COUNTING_NEUTRAL * 2; n++) {
        power(&rcPositionCaptured);
    }
    assertEquals("POWN001", CCPR1L, 0);

    dashboard.capturedPosition.position = NEUTRAL_POSITION - 1;
    for (n = 0; n < COUNTING_NEUTRAL; n++) {
        power(&rcPositionCaptured);
    }
    assertEquals("POWN002", CCPR1L, 0);

    dashboard.capturedPosition.position = 100;
    power(&rcPositionCaptured);
    assertEquals("POWN003", CCPR1L, 100);    
    
    rcNoPosition.event = RC_NO_POSITION;
    power(&rcNoPosition);
    assertEquals("POWN004", CCPR1L, 0);
    
    dashboard.capturedPosition.position = 100;
    power(&rcPositionCaptured);
    assertEquals("POWN005", CCPR1L, 0);
}
#endif