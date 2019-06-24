#include "capture.h"
#include "test.h"

/** Last captured raising edge. */
unsigned int lastRaisingEdge;

void resetCapture(void) {
    lastRaisingEdge = 0;
}

void captureRaisingEdge(unsigned int instant) {
    lastRaisingEdge = instant;
}

void captureFallingEdge(
        unsigned int instant, 
        CapturedPosition *capturedPosition) {

    // Expected values of delta range from:
    // Most negative: 1 ms ==> 2000
    // Most positive: 2 ms ==> 4000
    unsigned int delta = instant - lastRaisingEdge;
    
    // Values of magnitude have to range:
    // Closest to central point: 0
    // Furthest from central point: 1000
    unsigned int magnitude;
    
    if (delta > 4000) {
        delta = 4000;
    }
    if (delta < 2000) {
        delta = 2000;
    }
    delta -= 2000;
    if (delta <= 1000) {
        magnitude = 1000 - delta;
        capturedPosition->sign = POSITION_NEGATIVE;
        capturedPosition->position = magnitude >> 2;
    } else {
        magnitude = delta - 1000;
        capturedPosition->sign = POSITION_POSITIVE;
        capturedPosition->position = magnitude >> 2;
    }
}

#ifdef TEST
void can_detect_a_pulse_over_a_channel() {
    CapturedPosition capturedPosition;
    resetCapture();

    captureRaisingEdge(10000);
    captureFallingEdge(11000, &capturedPosition);
    assertEquals("CP1C01a", capturedPosition.sign, POSITION_NEGATIVE);
    assertEquals("CP1C01b", capturedPosition.position, 250);

    captureRaisingEdge(10000);
    captureFallingEdge(12000, &capturedPosition);
    assertEquals("CP1C02a", capturedPosition.sign, POSITION_NEGATIVE);
    assertEquals("CP1C02b", capturedPosition.position, 250);

    captureRaisingEdge(10000);
    captureFallingEdge(12250, &capturedPosition);
    assertEquals("CP1C03a", capturedPosition.sign, POSITION_NEGATIVE);
    assertEquals("CP1C03b", capturedPosition.position, 187);

    captureRaisingEdge(10000);
    captureFallingEdge(12500, &capturedPosition);
    assertEquals("CP1C04a", capturedPosition.sign, POSITION_NEGATIVE);
    assertEquals("CP1C04b", capturedPosition.position, 125);

    captureRaisingEdge(10000);
    captureFallingEdge(12750, &capturedPosition);
    assertEquals("CP1C05a", capturedPosition.sign, POSITION_NEGATIVE);
    assertEquals("CP1C05b", capturedPosition.position, 62);

    captureRaisingEdge(10000);
    captureFallingEdge(13000, &capturedPosition);
    assertEquals("CP1C06a", capturedPosition.sign, POSITION_NEGATIVE);
    assertEquals("CP1C06b", capturedPosition.position,  0);

    captureRaisingEdge(10000);
    captureFallingEdge(13250, &capturedPosition);
    assertEquals("CP1C07a", capturedPosition.sign, POSITION_POSITIVE);
    assertEquals("CP1C07b", capturedPosition.position,  62);
    
    captureRaisingEdge(10000);
    captureFallingEdge(13500, &capturedPosition);
    assertEquals("CP1C08a", capturedPosition.sign, POSITION_POSITIVE);
    assertEquals("CP1C08b", capturedPosition.position, 125);

    captureRaisingEdge(10000);
    captureFallingEdge(13750, &capturedPosition);
    assertEquals("CP1C09a", capturedPosition.sign, POSITION_POSITIVE);
    assertEquals("CP1C09b", capturedPosition.position, 187);
    
    captureRaisingEdge(10000);
    captureFallingEdge(14000, &capturedPosition);
    assertEquals("CP1C10a", capturedPosition.sign, POSITION_POSITIVE);
    assertEquals("CP1C10b", capturedPosition.position, 250);
    
    captureRaisingEdge(10000);
    captureFallingEdge(14500, &capturedPosition);
    assertEquals("CP1C11a", capturedPosition.sign, POSITION_POSITIVE);
    assertEquals("CP1C11b", capturedPosition.position, 250);
}

void can_detect_a_pulse_with_carry() {
    CapturedPosition capturedPosition;
    resetCapture();

    captureRaisingEdge(65500);
    captureFallingEdge(2215, &capturedPosition);
    assertEquals("CPDE00", capturedPosition.position,   187);
}

void test_capture() {
    can_detect_a_pulse_over_a_channel();
    can_detect_a_pulse_with_carry();
}
#endif