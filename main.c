/*
 * File:   main.c
 * Author: Utku
 *
 * Created on June 14, 2023, 10:21 AM
 */

#include <xc.h>

#define TMR1_RESET 65286 // Interrupt each 1ms with a prescaler of 8
#define BLINK_INTERVAL 1000 // in milliseconds

uint16_t timerCounter = 0;

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    if (IFS0bits.T1IF) {
        IFS0bits.T1IF = 0;
        TMR1 = TMR1_RESET;
        timerCounter++;
    }
}

void main(void) {
    // Clock Config
    CLKDIVbits.FRCDIV = 0b000; // INTOSC Postscaler 1:1

    // Pins Config
    TRISA = 0;

    // Timer Config
    T1CONbits.TCS = 0; // Clock Source Internal Clock
    T1CONbits.TCKPS = 0b01; // Prescaler 1:8
    T1CONbits.TGATE = 0; // Gated Time Accumulation bit, couldn't understand how it works exactly...
    T1CONbits.TSIDL = 0; // Do not stop in idle mode
    T1CONbits.TON = 1; // Timer 1 ON
    TMR1 = TMR1_RESET; // Reset Timer 1

    // Interrupt Config
    IEC0bits.T1IE = 1; // Enable
    IFS0bits.T1IF = 0; // Flag bit

    while (1) {
        if (timerCounter == BLINK_INTERVAL) {
            PORTAbits.RA0 = 1;
        } else if (timerCounter == BLINK_INTERVAL * 2) {
            timerCounter = 0;
            PORTAbits.RA0 = 0;
        }
    }

    return;
}
