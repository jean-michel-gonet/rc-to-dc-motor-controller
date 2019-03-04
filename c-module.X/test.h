#ifndef TEST_H
#define	TEST_H

#ifdef TEST

/** 
 * Resets the test bench. 
 * To configure MPLABX to run unit tests:
 * - In project properties, click on Manage Configurations.
 * - Create a new configuration and rename it to 'test'.
 * - Select 'Simulator' as hardware tool. 
 * - Apply changes, but don't close the properties yet.
 * - In 'Simulator' options, select 'Option categories' as 'UART1 options'
 * - Enable the UART1.
 * - Apply changes, but don't close the properties yet.
 * - In XC8 compiler options, add a macro called 'TEST' (in capitals).
 * - Apply and close.
 */
void startTests(void);

/**
 * Verifies if <param>actualValue</param and <param>expectedValue</param>
 * are equal. If not, then notifies that the test has failed.
 * @param testId Test identifier. Use an arbitrary but unique identifier for
 * each test.
 * @param extualValue The actual value.
 * @param expectedValue The expected value.
 */
unsigned char assertEquals(const char *testId, int actualValue, int expectedValue);

/**
 * Verifies if <param>actualValue</param> is between the specified limits.
 * @param testId Test identifier. Use an arbitrary but unique identifier for
 * each test.
 * @param actualValue The actual value.
 * @param min The minimum expected value.
 * @param max The maximum expected value.
 */
unsigned char assertInterval(const char *testId, int actualValue, int min, int max);

/**
 * Verifies if <param>actualValue</param> is not zero.
 * @param testId Test identifier. Use an arbitrary but unique identifier for
 * each test.
 * @param actualValue The actual value.
 */
unsigned char assertNonZero(const char *testId, char valeurObtenue);

/** Finishes testing and displays the results.*/
void finishTests(void);

#endif

#endif