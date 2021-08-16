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
void cfg_usart(void);
void cfg_inte(void);
void cfg_iocb(void);
void send_crct(char st[]);
void send_char(char dato);
float conv(unsigned char aa);
unsigned char V;
unsigned char CONT;
unsigned char op;
float v;
char f1[10];
char f2[10];


/*------------------------------------------------------------------------------
 INTERRUPCIÓN
 -----------------------------------------------------------------------------*/

void __interrupt() isr(void){

    if(PIR1bits.RCIF){           //Interrupción USART
        PORTD = RCREG;           // Envío a PORTD
        //__delay_ms(4000);
    }

    if(INTCONbits.RBIF){
        if (PORTBbits.RB0 == 0){
            V++;
        }
        if (PORTBbits.RB1 == 0){//Decremento de contador en puerto A
            V--;
        }
        INTCONbits.RBIF = 0;  
    }

}
//*****************************************************************************
// C�digo Principal
//*****************************************************************************
void main(void) {
    cfg_io();
    cfg_inte();
    cfg_iocb();
    cfg_usart();
    cfg_clk();
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
      v = conv(V); //Conversión de Binario a float

      sprintf(f1, "%0.0f,%0.0f,%0.0f,",v,v,v);
      
      //TXREG = 0x0A;             //Clear del display 
      //TXREG = 10;      
      send_crct(f1);          //Función para enviar valor a TXREG 

      __delay_us(500); 
    }
    return;
}
//*****************************************************************************
// Funci�n de Inicializaci�n
//*****************************************************************************
void cfg_io(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    TRISB = 0;
    TRISD = 0;
    TRISBbits.TRISB0 = 1;  //Botones en B0 y B1
    TRISBbits.TRISB1 = 1;
    OPTION_REGbits.nRBPU =  0 ; // se habilita el pull up interno en PORTB
    WPUB = 0x03;  //  Pull ups para los pines RB0 y RB1
    
    PORTB = 0;
    PORTD = 0;
    PORTA = 0;

}
void cfg_clk(){
    OSCCONbits.IRCF = 0b111; //IRCF = 111 (8MHz) 
    OSCCONbits.SCS = 1;   //Reloj interno habilitado
}
void cfg_usart(){        //Configuración usart

    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 207;
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
    INTCONbits.RBIE = 1; // Habilitar interrupción de B
    INTCONbits.RBIF = 0; // Clear en bandera de B
}
void cfg_iocb(){
    IOCB = 0X03 ;        // Habilitar PORTB 0 y 1 para interrupción
    INTCONbits.RBIF = 0; // Clear de la bandera B   
    
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
float conv(unsigned char aa){ //Función para convertir binario en doble preci.
    float result;
    result = aa*1;
    return result;
}
