#include "dashboard.h"
#include "signal.h"

#include <xc.h>

void signal() {
    SignalType signal = dashboard.signal;

    switch (signal) {
        case SIGNAL_WAITING_FOR_NEUTRAL:
            LATCbits.LATC5 = 1;
            LATCbits.LATC6 = 1;
            break;

        case SIGNAL_MOVING_POSITIVE:
            LATCbits.LATC5 = 0;
            LATCbits.LATC6 = 1;
            break;

        case SIGNAL_MOVING_NEGATIVE:
            LATCbits.LATC5 = 1;
            LATCbits.LATC6 = 0;
            break;

        default:
        case SIGNAL_NEUTRAL:
            LATCbits.LATC5 = 0;
            LATCbits.LATC6 = 0;
            break;
    }
}
