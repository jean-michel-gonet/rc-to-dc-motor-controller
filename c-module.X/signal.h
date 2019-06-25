#ifndef SIGNAL__H
#define SIGNAL__H

typedef enum SIGNAL_TYPE {
    SIGNAL_WAITING_FOR_NEUTRAL,
    SIGNAL_NEUTRAL,
    SIGNAL_MOVING_POSITIVE,
    SIGNAL_MOVING_NEGATIVE
} SignalType;

void signal();

#endif