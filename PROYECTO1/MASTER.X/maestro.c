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
void cfg_usart(void);
unsigned char lock;
unsigned char door;
unsigned char light;
float v0;
float v1;
float v2;
unsigned char pd;
unsigned char val = 0;
void send_crct(char st[]);
void send_char(char dato);
float conv(unsigned char aa);
char f1[10];
/*------------------------------------------------------------------------------
 INTERRUPCIÓN
 -----------------------------------------------------------------------------*/

void __interrupt() isr(void){

    if(PIR1bits.RCIF){           //Interrupción USART
        if(RCREG == '1'){
            light = 1;           // Envío a PORTD            
        }
        if(RCREG == '0'){
            light = 0;           // Envío a PORTD     
        }

        //__delay_ms(4000);
    }
}    
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    cfg_clk();
    cfg_usart();
    while(1){
        /*I2C_Master_Start();
        I2C_Master_Write(0x50);
        I2C_Master_Write(val);
        I2C_Master_Stop();
        __delay_ms(200);*/

        I2C_Master_Start();
        I2C_Master_Write(0x51);
        pd = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        lock = (0x0F & pd);
        door = (0x0F & (pd>>4));
                
        v0 = conv(lock); //Conversión de Binario a float
        v1 = conv(door); //Conversión de Binario a float
        v2 = conv(light);

        sprintf(f1, ",%0.0f,%0.0f,%0.0f,",v0,v1,v2);
         
        send_crct(f1);          //Función para enviar valor a TXREG 
        PORTDbits.RD0 = lock;
        PORTDbits.RD1 = door;
        
        if(light == 1){
            PORTAbits.RA0 = 1;
        }
        if(light == 0){
            PORTAbits.RA0 = 0;
        }
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISD = 0;
    TRISA = 0;
    PORTD = 0;
    PORTA = 0;
    INTCONbits.GIE = 1;  //Enable Interrupciones globales
    INTCONbits.PEIE = 1; //Enable interrupciones perifericas
    PIE1bits.RCIE = 1;   //Enable interrupcion del UART  
    
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}
void cfg_clk(void){
    OSCCONbits.IRCF = 0b110; //IRCF = 111 (8MHz) 
    OSCCONbits.SCS = 1;   //Reloj interno habilitado
}
void cfg_usart(void){
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 103;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    
    TXSTAbits.TXEN = 1;    
}
//*****************************************************************************
// Función de conversión
//*****************************************************************************
float conv (unsigned char aa){ //Función para convertir a float
    float result;
    result = aa*1;
    return result;
}
void send_crct(char st[]){
    int i = 0;              //Declaración variable que recorrera la cadena
    while (st[i] != 0){     //Mientras st diferente de 0
        send_char(st[i]);  //Se envía caracter por caracter
        i++;                //Se incrementa el caracter
    }
}
void send_char(char dato){
    while(!TXIF);           //Mientras la bandera de transmisión sea 0
    TXREG = dato;           //Se envía el caracter
}