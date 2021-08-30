/*
 * File:   main.c
 * Author: Pablo
 * Ejemplo de uso de I2C Master
 * Created on 17 de febrero de 2020, 10:32 AM
 */
//*****************************************************************************
// Palabra de configuración
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
// Definición e importación de librerías
//*****************************************************************************
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 4000000

//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
void cfg_clk(void);
unsigned char d0;
unsigned char d1;
unsigned char pd;
unsigned char val = 0;
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    cfg_clk();
    while(1){
        I2C_Master_Start();
        I2C_Master_Write(0x50);
        I2C_Master_Write(val);
        I2C_Master_Stop();
        __delay_ms(200);
       
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        pd = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        d0 = (0x0F & pd);
        d1 = (0x0F & (pd>>4));
        
        PORTDbits.RD0 = d0;
        PORTDbits.RD1 = d1;
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}
void cfg_clk(void){
    OSCCONbits.IRCF = 0b110; //IRCF = 111 (8MHz) 
    OSCCONbits.SCS = 1;   //Reloj interno habilitado
}