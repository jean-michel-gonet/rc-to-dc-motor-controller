#ifndef SIGNAL__H
#define SIGNAL__H

typedef enum SIGNAL_TYPE {
    NOT_MOVING,
    MOVING_POSITIVE,
    MOVING_NEGATIVE
} SignalType;

void signal(Event *event);

#endif