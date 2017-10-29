/* 
 * File:   CKTimerPool
 * Author: Paulo Pedreiras
 *
 * Created on Oct 25, 2017
 * MPLAB X IDE v3.50 + XC32 v1.42
 *
 * Target: Digilent chipKIT MAx32 board 
 * Configurations:
 *      HSPLL; CPUCLK=80MHz, PBCLCK=40MHz; Watchdog timer disabled
 *      
 * Overview:
 *     Blinks led connected on port RA3 with a pace measured via timer 2
 *     Port D5 is set as open drain and changes state at the same rate (3.3V-5V interface)
 *     Illustrates how stdout can be redirected to UART  
 *
 *  Revisions:
 *      2017-10-15: Initial release
 */

#include "../CKCommon/ConfigBits/config_bits.h" // NOTE!! Must precede project file includes

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#define _SUPPRESS_PLIB_WARNING 1
#include <plib.h>
#include "../CKCommon/UART/uart.h" 

#define SYSCLK  80000000L // System clock frequency, in Hz
#define PBCLOCK 40000000L // Peripheral Bus Clock frequency, in Hz

/*
 * 
 */
int main(int argc, char** argv) {
    // Performance optimization (flash access time, enable instruct and data cache,... and PBClock setup
    SYSTEMConfigPerformance(SYSCLK);
    mOSCSetPBDIV(OSC_PB_DIV_2); // This is necessary since SYSTEMConfigPerformance defaults FPBDIV to DIV_1

    // Set RA3 and D5 ports
    TRISAbits.TRISA3 = 0;  // A3 as output
    TRISDbits.TRISD5 = 0; // D5 a output
    ODCDbits.ODCD5 = 1; // D5 as open-drain

    // Init UART and redirect tdin/stdot/stderr to UART
    if(UartInit(PBCLOCK, 115200) != UART_SUCCESS) {
        PORTAbits.RA3 = 1;
        while(1);
    }
    __XC_UART = 1; /* Redirect stdin/stdout/stderr to UART1*/
    
      // Set timer
    T2CONbits.ON = 0; // Stop timer
    T2CONbits.TCS = 0; // Internal clock
    T2CONbits.TCKPS = 0b111; //Select 256 pre-scaler: fclk = 40000000/256= 156250Hz
    T2CONbits.T32 = 0; // 16 bit timer operation
    PR2= 65000;   // Timer event around 0.4s (1/156250*65535)
    TMR2=0;
    T2CONbits.TON=1; // Start the timer
    
    // Loop
    while (1) {
        //Busywait for timer
        while(IFS0bits.T2IF==0);  

        // Reset int flag
        IFS0bits.T2IF=0;
        
        // Toggle ports and send char
        LATAbits.LATA3 = !LATAbits.LATA3;
        PORTDbits.RD5 = PORTDbits.RD5;
        printf("%s\n\r","Bang!");

    }
     
    return (EXIT_SUCCESS);
}

