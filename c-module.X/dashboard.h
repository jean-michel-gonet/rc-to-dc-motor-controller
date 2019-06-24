#ifndef DASHBOARD__H
#define DASHBOARD__H

#include "capture.h"
#include "signal.h"

typedef struct DASHBOARD {
    CapturedPosition capturedPosition;
    SignalType signal;
} Dashboard;

Dashboard dashboard;

#endif