//*****************************************************************************
/*
 * File:   pruebamaster.c
 * Author: Gerardo Fuentes
 * Ejemplo de implementaci�n de la comunicaci�n i2c 
 * C�digo Maestro
 * Created on 29 de agosto de 2021, 01:04 AM
 */
//*****************************************************************************
//*****************************************************************************
// Palabra de configuraci�n
//*****************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definici�n e importaci�n de librer�as
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <pic16f887.h>
#include "I2C.h"
//*****************************************************************************
// Definici�n de variables
//*****************************************************************************
#define _XTAL_FREQ 4000000
//*****************************************************************************
// Definici�n de funciones para que se puedan colocar despu�s del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************