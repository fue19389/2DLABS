//*****************************************************************************
/*
 * File:   main.c
 * Author: Pablo
 * Ejemplo de implementaci�n de la comunicaci�n SPI 
 * C�digo Maestro
 * Created on 10 de febrero de 2020, 03:32 PM
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
#include "SPI.h"
//*****************************************************************************
// Definici�n de variables
//*****************************************************************************
#define _XTAL_FREQ 4000000
//*****************************************************************************
// Definici�n de funciones para que se puedan colocar despu�s del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
void cfg_clk(void);
void cfg_usart(void);
void cfg_inte(void);
void send_crct(char st[]);
void send_char(char dato);
double conv(unsigned char aa);
unsigned char V3;
unsigned char chng1 = 0;
unsigned char chng2;
double v11;
double v22;
double v33;

char f1[10];




void __interrupt() isr(void){

if(PIR1bits.RCIF){            //Interrupción USART

    if (RCREG == 43){         //Acciones con caracter "+"
        TXREG = 43;           //Display de "+"
        V3++;                 //Incremento de variable
    }
    if (RCREG == 45){         //Accinoes con caracter "-"
        TXREG = 45;           //Display de "-"
        V3--;                 //Decremento de variable
    }
}
}
//*****************************************************************************
// C�digo Principal
//*****************************************************************************
void main(void) {
    setup();
    cfg_inte();
    cfg_usart();
    cfg_clk();
    PORTAbits.RA0 = 1;
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
       PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(chng1);
       
       if(PORTAbits.RA0 == 1){
           chng1 = spiRead();
           PORTAbits.RA0 = 0;
       }
       else if(PORTAbits.RA0 == 0){
           chng2 = spiRead(); 
           PORTAbits.RA0 = 1;
       }
   
       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave Deselect 
       
       
      v11 = conv(chng1);        //Conversión de Binario a doble precisión "0.00"
      v22 = conv(chng2);
      v33 = conv(V3);
      
      sprintf(f1, "%3.1fV %3.2fV %3.2f",v11, v22, v33);
      
      TXREG = 12;           //Clear del display 
      send_crct(f1);          //Función para enviar valor a TXREG 
      
       __delay_us(100);
       
       PORTB = V3;
  
    }
    return;
}
//*****************************************************************************
// Funci�n de Inicializaci�n
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISCbits.TRISC2 = 0;
    TRISA0 = 0;
    TRISB = 0;
    TRISD = 0;
    
    PORTB = 0;
    PORTD = 0;
    PORTCbits.RC2 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}
void cfg_clk(){
    OSCCONbits.IRCF = 0b100; //IRCF = 100 (1MHz) 
    OSCCONbits.SCS = 1;   //Reloj interno habilitado
}
void cfg_usart(){        //Configuración usart

    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 25;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    
    TXSTAbits.TXEN = 1;
            
}
void cfg_inte(){
    INTCONbits.GIE = 1;  //Enable Interrupciones globales
    INTCONbits.PEIE = 1; //Enable interrupciones perifericas
    PIE1bits.RCIE = 1;   //Enable interrupcion del UART    
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
double conv(unsigned char aa){ //Función para convertir binario en doble preci.
    double result;
    result = aa*0.0196;
    return result;
}