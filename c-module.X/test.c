#ifdef TEST

#include <xc.h>
#include <stdio.h>

/**
 * To send characters to the EUSART. Only intended for testing, so it is
 * rather primitive. By implementing this method, we enable <code>printf</code>
 * to send data through RX / TX outputs.
 * @param data ASCII code to display.
 */
void putch(char data) {
    while( ! TX1IF);
    TXREG1 = data;
}

/**
 * Configures the EUSART as an asynchronous output.
 * As it is intended to be used within the MPLABX simulator,
 * frequency and baud rate considerations are not very important.
 */
void initialiseUART1() {
    // Inactivate A/D converters:
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;

    // Not critical here, but at Fosc=1 MHz, this is 1200 bauds:
    SPBRG1 = 12;
    SPBRGH1 = 0;

    // Configures RC6 and RC7 as digital input, so EUSART can take control:
    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;

    // Configures EUSART:
    RCSTA1bits.SPEN = 1;  // Enables EUSART.
    TXSTA1bits.SYNC = 0;  // Asynchronous mode.
    TXSTA1bits.TXEN = 1;  // Enables transmitter.
}

/** Number of failed tests. */
static int testsFailed = 0;

/** Number of succeeded tests. */
static int testsSucceeded = 0;

/** Startups the test bench.*/
void startTests() {
    initialiseUART1();
    testsFailed = 0;
    printf("\r\nLaunched tests...\r\n");
}

unsigned char assertEquals(const char *testId, int actualValue, int expectedValue) {
    if (actualValue != expectedValue) {
        printf("Test %s: actual [%d], expected [%d]\r\n",
                testId, actualValue, expectedValue);
        testsFailed ++;
        return 1;
    } else {
        testsSucceeded ++;
    }
    return 0;
}

unsigned char assertInterval(const char *testId, int actualValue, int min, int max) {
    if ( (actualValue < min) || (actualValue > max) ) {
        printf("Test %s: actual value [%d], expected between [%d] and [%d]\r\n",
                testId, actualValue, min, max);
        testsFailed ++;
        return 1;
    } else {
        testsSucceeded ++;
    }
    return 0;
}

unsigned char assertNotZero(const char *testId, char actualValue) {
    if (actualValue == 0) {
        printf("Test %s: actual [%d], expected [0]\r\n",
                testId, actualValue);
        testsFailed ++;
        return 1;
    } else {
        testsSucceeded ++;
    }
    return 0;
}

void finishTests() {
    printf("%d tests succeeded\r\n", testsSucceeded);
    printf("%d tests failed\r\n", testsFailed);
}

#endif