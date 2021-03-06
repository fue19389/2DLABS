/*
 * Proyecto 3
 * Gerardo Fuentes y Stefano Papadopolo
 * Utilizaci?n de CCS y TIVAWARE para sensores y env?o de datos
 */

//Bibliotecas
#include<stdint.h>
#include<stdbool.h>
#include"inc/hw_memmap.h"
#include"inc/tm4c123gh6pm.h"
#include"driverlib/sysctl.h"
#include"driverlib/gpio.h"
#include"driverlib/timer.h"
#include"driverlib/interrupt.h"
#include"driverlib/adc.h"
#include"driverlib/uart.h"
#include"driverlib/pin_map.h"


// Constante de reloj
#define XTAL 16000000

//Variables
uint32_t a;
uint32_t b;
uint32_t c;
uint32_t d;
uint8_t V;
uint8_t Vp = 7;
uint8_t v1;
uint8_t v2;
uint8_t v3;
uint8_t v4;

//Funciones
void ObtenerMuestra(void);
void InitUART(void);


//Main
int main(void) {
    //Establecer reloj del microcontrolador
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    //Iniciar UART
    InitUART();
    //Habilitar Puertos Perif?ricos
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    //Habilitar interrupci?n del UART
    IntMasterEnable();
    //Habilitar ADC
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PIOSC | ADC_CLOCK_RATE_FOURTH , 5);


    //Habilitar pin TIPO ADC
    GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_5);
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

    GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_5);
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

    //Configurar secuencia de muestreo
    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_ALWAYS, 1);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH11);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH10);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH9);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_CH8|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 1);
    //Habilitar interrupci?n por parte del ADC
    ADCIntEnable(ADC0_BASE, 1);
    //Registrar ISR
    ADCIntRegister(ADC0_BASE, 1, ObtenerMuestra);
    //Habilitar interrupci?n por parte del NVIC
    IntEnable(INT_ADC0SS1);


    //Loop prinicipal
    while(1){
        if (a < 3500){  //Compara los valores del ADC para deterniar si enciende un pin u otro
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5,32);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6,0);
            v1 = 1;     //Encendido de bandera (Disponible)
        }
        if(a >= 3500){
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5,0);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6,64);
            v1 = 0;
        }

        if (b < 3500){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,4);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,0);
            v2 = 1;
        }
        if(b >= 3500){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,0);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,8);
            v2 = 0;
        }

        if (c < 3500){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3,8);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4,0);
            v3 = 1;
        }
        if(c >= 3500){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3,0);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4,16);
            v3 = 0;
        }

        if (d < 3500){
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7,128);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4,0);
            v4 = 1;
        }
        if(d >= 3500){
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7,0);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4,16);
            v4 = 0;
        }


        /***********************Env?o de datos****************/

        V = (v1+v2+v3+v4);      //Suma de bandera (disponible)

        if (Vp != V){           //Antirrebote para enviar solo cuando el valor cambia
            UARTCharPut(UART7_BASE, V);   //Enviar la suma de banderas
            SysCtlDelay(XTAL/3);          //Delay para no interrumpir
            Vp = V;                       //Valor de antirrebote
        }
    }
}

void ObtenerMuestra(){           //Funci?n para guardar valores del adc
    ADCIntClear(ADC0_BASE, 1);   //Clear en la bandera de interrupci?n del ADC
     a = ADC0_SSFIFO1_R;         //Obtenci?n de datos usando FIFO
     b = ADC0_SSFIFO1_R;
     c = ADC0_SSFIFO1_R;
     d = ADC0_SSFIFO1_R;
}

void InitUART(void)
{
    /*Habilitar puerto E*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);  //

    /*Habilitar m?dulo 7 de UART*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);

    /*Configuraci?n para utilizar los pines del UART7*/

    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PE0_U7RX);
    GPIOPinConfigure(GPIO_PE1_U7TX);

    /* Configuraci?n general del UART*/
    UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

}
