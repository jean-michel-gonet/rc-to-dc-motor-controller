#ifndef CAPTURE__H
#define	CAPTURE__H

/**
 * Signs the position according to values in CCP1CON.P1M
 */
typedef enum {
    POSITION_POSITIVE = 0b01,
    POSITION_NEGATIVE = 0b11
} PositionSign;

/**
 * A structure to describe the captured position
 * in terms compatible with PWM configuration.
 */
typedef struct {
    PositionSign sign;
    unsigned char position;
} CapturedPosition;

/**
 * Resets the capture module.
 */
void resetCapture(void);

/**
 * Call this method to indicate the capture of a raising edge.
 * Use the CCP modules to capture edges.
 * @param instant The instant of detection of the rasing edge.
 */
void captureRaisingEdge(unsigned int instant);

/**
 * Call this method to indicate the capture of a falling edge.
 * Use the CCP modules to capture edges.
 * @param instant The instant of detection of the falling edge.
 * @param capturedPosition Method writes the captured position here.
 */
void captureFallingEdge(
        unsigned int instant, 
        CapturedPosition *capturedPosition);

#ifdef TEST
void test_capture();
#endif

#endif