//*****************************************************************************
/*
 * File:   main5a.c
 * Author: Gerardo Fuentes
 * Ejemplo de implementaci�n de la comunicaci�n SPI 
 * C�digo Maestro
 * Created on 13 de agosto de 2021, 01:58 PM
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
void cfg_io(void);
void cfg_clk(void);
void cfg_inte(void);
void cfg_usart(void);
void send_crct(char st[]);
void send_char(char dato);
float conv(int aa);
int V;
int D = 0;
unsigned char i;
float v;
char f1[15];

/*------------------------------------------------------------------------------
 INTERRUPCIÓN
 -----------------------------------------------------------------------------*/

void __interrupt() isr(void){


}
//*****************************************************************************
// C�digo Principal
//*****************************************************************************
void main(void) {
    cfg_io();
    //cfg_inte();
    cfg_usart();
    cfg_clk();
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        /*TMR1H = 0;
        TMR1L = 0;
        
        PORTAbits.RA0 = 1;
        __delay_us(10);
        PORTAbits.RA0 = 0;

        
        while(!RA1);              //Waiting for Echo
        TMR1ON = 1;               //Timer Starts
        while(RA1);               //Waiting for Echo goes LOW
        TMR1ON = 0;               //Timer Stops
        
        V = (TMR1L | (TMR1H<<8));
        D = V/58;
        v = conv(D);
        
        sprintf(f1, "%0.0f cm\r", v);
        send_crct(f1);

        if(PORTAbits.RA2 == 1){
            PORTDbits.RD0 = 1;
        }
        if(PORTAbits.RA2 == 0){
            PORTDbits.RD0 = 0;
        }*/
        
        I2C_Master_Start();       //Repeticiones para iniciar comunicación
        I2C_Master_Write(0x72);   //con los esclavo y sensor (este es sensor)
        I2C_Master_Write(0x80);
        I2C_Master_Write(0x03);

        I2C_Master_Write(0x81);
        I2C_Master_Write(0x12);
        
   
        I2C_Master_Stop();        // Dirección del SHT21 es 0x80        
        __delay_ms(200);
       
        I2C_Master_Start();       
        I2C_Master_Write(0x73);
        I2C_Master_Write(0x9C);
        D = I2C_Master_Read(0); //Guardar temperatura en variable
        I2C_Master_Stop();
        __delay_ms(200);
        
        v = conv(D);
        
        sprintf(f1, "%0.0f", v);
        send_crct(f1);
        
        __delay_ms(500);
    }
    return;
}
//*****************************************************************************
// Funci�n de Inicializaci�n
//*****************************************************************************
void cfg_io(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    TRISB = 0;
    TRISD = 0;
    TRISB = 0;
    
    PORTB = 0;
    PORTD = 0;
    PORTA = 0;

}
void cfg_clk(){
    OSCCONbits.IRCF = 0b110; //IRCF = 111 (8MHz) 
    OSCCONbits.SCS = 1;   //Reloj interno habilitado
}

void cfg_inte(){
    INTCONbits.GIE = 1;  //Enable Interrupciones globales
    INTCONbits.PEIE = 1; //Enable interrupciones perifericas
    PIE1bits.RCIE = 1;   //Enable interrupcion del UART 
    
}
void cfg_usart(){        //Configuración usart

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

/*------------------------------------------------------------------------------
 FUNCIONES
 -----------------------------------------------------------------------------*/

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
float conv(int aa){ //Función para convertir binario en doble preci.
    float result;
    result = aa;
    return result;
}

