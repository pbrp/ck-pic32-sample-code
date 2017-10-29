/* 
 * File:   uart.h
 * Author: Paulo Pedreiras
 *
 * Created on Oct 25, 2017
 * MPLAB X IDE v3.50 + XC32 v1.42
 *
 * Target: Digilent chipKIT MAx32 board 
 * 
 * Overview:
 *          Set of functions to handle the UART       
 
 * Notes: Partially based on the bootloader code from Microchip
 * 
 * Revisions:
 *     2017-10-25: initial release
 */

#ifndef __UART_H__
#define __UART_H__

// Define return codes
#define UART_SUCCESS 1
#define UART_BR_NOT_SUP -1
#define UART_PBCLOCK_NOT_SUP -2

// Define prototypes (public interface)
int UartInit(UINT64 pbclock, UINT32 br);
void UartClose(void);
int GetChar(UINT8 *byte);
void PutChar(UINT8 txChar);


#endif
