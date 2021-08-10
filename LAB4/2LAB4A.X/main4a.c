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
#include "LCD.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000

//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
void cfg_clk(void);

double conv1(unsigned char aa);
double conv2(unsigned char aa);
float conv3(unsigned char aa);
unsigned char V1;
unsigned char V2;
unsigned char V3;
char f1[10];
double v11;
double v22;
double v33;

//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    cfg_clk();
    setup();
    Lcd_Init();                //Inicializar LCD
    
    while(1){
        I2C_Master_Start();       //Repeticiones para iniciar comunicación
        I2C_Master_Write(0x80);   //con los esclavo y sensor (este es sensor)
        I2C_Master_Write(0xF3);   // Línea que envia comando de SHT21
        I2C_Master_Stop();        // Dirección del SHT21 es 0x80
        __delay_ms(200);
       
        I2C_Master_Start();       
        I2C_Master_Write(0x81);
        V1 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_Start();       //Rep esclavo 1 (potenciómetro)
        I2C_Master_Write(0x50);
        I2C_Master_Write(PORTB);
        I2C_Master_Stop();
        __delay_ms(200);
       
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        V2 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_Start();        //Rep esclavo 2 (contador)
        I2C_Master_Write(0x40);
        I2C_Master_Write(PORTB);
        I2C_Master_Stop();
        __delay_ms(200);
       
        I2C_Master_Start();
        I2C_Master_Write(0x41);
        V3 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);        

        Lcd_Clear();           //Display LCD usando librerías primera línea
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String(" S1:   S2:   S3:");
        v11 = conv2(V1);        //Conversión a doble precisión "0.00" temp
        v22 = conv1(V2);        //Conversión a doble presición voltaje
        v33 = conv3(V3);        //Conversión a float
      
        Lcd_Set_Cursor(2,1);    //Display LCD usando librerías segunda línea
        sprintf(f1, "%3.1fK %3.1fV %3.0f",v11, v22, v33); //Print en LCD
        Lcd_Write_String(f1);

        __delay_ms(100);
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISCbits.TRISC0 = 0;           //Salidas para E y RS de LCD
    TRISCbits.TRISC1 = 0;
    TRISB = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}
void cfg_clk(){
    OSCCONbits.IRCF = 0b111; //IRCF = 111 (8MHz) 
    OSCCONbits.SCS = 1;   //Reloj interno habilitado
}
/*------------------------------------------------------------------------------
FUNCIONES
------------------------------------------------------------------------------*/

double conv1(unsigned char aa){ //Función para convertir binario en doble preci.
    double result;
    result = aa*0.0196;
    return result;
}
double conv2(unsigned char aa){ //Función para convertir a doble preci
    double result;              // En un intervalo de 0 a 160, para temp
    result = (aa*0.62745)+233.15;
    return result;
}
float conv3 (unsigned char aa){ //Función para convertir a float
    float result;
    result = aa*1;
    return result;
}