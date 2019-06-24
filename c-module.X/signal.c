#include "events.h"
#include "dashboard.h"
#include "signal.h"

#include <xc.h>

void signal(Event *event) {
    switch (dashboard.signal) {
        case NOT_MOVING:
            PORTCbits.RC5 = 1;
            PORTCbits.RC6 = 1;
            break;
        case MOVING_POSITIVE:
            PORTCbits.RC5 = 0;
            PORTCbits.RC6 = 1;
            break;
        case MOVING_NEGATIVE:
            PORTCbits.RC5 = 1;
            PORTCbits.RC6 = 0;
            break;  
    }
}
