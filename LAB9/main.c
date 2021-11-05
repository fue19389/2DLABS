/**
 * Sem�foro
 * IE-3027
 * Editado por GERARDO FUENTES
 */

/******************************LIBRER�AS***************/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

/*****************************VARIABLES*****************/
uint8_t F = 0; //Bandera inicia en 0
uint8_t PB1;   //Definici�n de variable para el PUSH1
/********************CONFIG*****************************/
int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //Iniciar el clk interno a 16MHz
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //Iniciar el Puerto F
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); //Definic�n de pines como outputs
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);// Definici�n de pin como input (PUSH1)
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); //Configuraaci�n del PUSH1 como weak pull-up
    while (1){ //Loop principal

        PB1 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4); //Guardar la lectura del PUSH1 en una variable

        if(PB1 == 0){ // Cuando se acciona el PUSH1...
            if(F == 0){//Si la bandera est� en 0, secuencia de sem�foro
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08); //PINLED VERDE
                SysCtlDelay(15000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00); //LEDS OFF
                SysCtlDelay(5000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08); //PINLED VERDE
                SysCtlDelay(5000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00); //LEDS OFF
                SysCtlDelay(5000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08); //PINLED VERDE
                SysCtlDelay(5000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00); //LEDS OFF
                SysCtlDelay(5000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x0A); //PINLED AMARILLO
                SysCtlDelay(20000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x02); //PINLED ROJO
                SysCtlDelay(20000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00); //LEDS OFF
            }
            F = 1; //Al terminar secuencia, encender la bandera (sistema antirrebote)
        }else{
            F = 0;//Cuando se suelta o si no se presiona el bot�n, clear de la bandera
        }
    }
}
