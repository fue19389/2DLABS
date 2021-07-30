//*****************************************************************************
/*
 * File:   main.c
 * Author: Pablo
 * Ejemplo de implementaci�n de la comunicaci�n SPI 
 * C�digo Esclavo
 * Created on 10 de febrero de 2020, 03:32 PM
 */
//*****************************************************************************
//*****************************************************************************
// Palabra de configuraci�n
//*****************************************************************************
// CONFIG1
#pragma config FOSC = EXTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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
#include "SPI.h"
//*****************************************************************************
// Definici�n de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000

uint8_t temporal = 0;
//*****************************************************************************
// Definici�n de funciones para que se puedan colocar despu�s del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
void cfg_adc(void);
void cfg_clk(void);

unsigned char V1;
unsigned char V2;
//*****************************************************************************
// C�digo de Interrupci�n 
//*****************************************************************************
void __interrupt() isr(void){
   if(SSPIF == 1){
        PORTD = spiRead();
        if(PORTAbits.RA0 == 1){
            spiWrite(V1);
        }
        if(PORTAbits.RA0 == 0){
            spiWrite(V2);
        }
        SSPIF = 0;
    }
    if(PIR1bits.ADIF){               //Interrupción ADC
        if(ADCON0bits.CHS == 5){  //Primer ciclo de ADC (ADRESH A PUERTO)
            V1 = ADRESH;
        }   
        else{                     //Segundo ciclo de ADC (ADRESH A PUERTO)
            V2 = ADRESH;
        }   
        PIR1bits.ADIF = 0;        //Clear de bandera ADC    
    }   
}
//*****************************************************************************
// C�digo Principal
//*****************************************************************************
void main(void) {

    cfg_adc();
    cfg_clk();
    setup();  
    ADCON0bits.GO = 1;
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
       //PORTB--;
       __delay_ms(250);
      if(ADCON0bits.GO == 0){  //Proceso al acabar conversión
        if (ADCON0bits.CHS == 5){ //Cambio de canal
            ADCON0bits.CHS = 6;
        }
        else{
            ADCON0bits.CHS = 5;   //Cambio de canal
          }
        __delay_us(50);     //Delay para no traslapar conversiones
        ADCON0bits.GO = 1;  //Reinicio de conversión
      }        
    }
    return;
}
//*****************************************************************************
// Funci�n de Inicializaci�n
//*****************************************************************************
void setup(void){
    ANSEL = 0X20;
    ANSELH = 0;
    
    TRISB = 0;
    TRISD = 0;
    TRISE = 0X03;
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA3 = 0;
    
    PORTB = 0;
    PORTD = 0;
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIE1bits.ADIE = 1;   //Enable interrupción del ADC
    PIR1bits.ADIF = 0;
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupci�n MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupci�n MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
   
}
void cfg_adc() {            //Configuración ADC
    ADCON1bits.ADFM = 0;    //Justificar a la izquierda
    ADCON1bits.VCFG0 = 0;   //Voltaje de referencia Vss y Vdd
    ADCON1bits.VCFG1 = 0;   
    
    ADCON0bits.ADCS0 = 0;   //ADC clock Fosc/2 para 1Mhz
    ADCON0bits.ADCS1 = 0;   
    ADCON0bits.CHS = 5;     //Canal 5 selecionado para inicar
    __delay_us(100);        //Delay más largo para tiempo necesario de conver.
    ADCON0bits.ADON = 1;    //Encender módulo ADC
    
}

void cfg_clk(){
    OSCCONbits.IRCF = 0b100; //IRCF = 100 (1MHz) 
    OSCCONbits.SCS = 1;   //Reloj interno habilitado
}