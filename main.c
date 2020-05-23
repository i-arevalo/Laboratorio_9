/**
 * Programador: Israel Arévalo
 *
 * Material: Electrónica digital 2
 *
 * Nombre del archivo: main.c
 *
 * Referencia: Se utilizó como referencia el código de ejemplo que se encontraba en
 *             la guia del laboratorio 9 y los ejemplos que se encuentran en canvas
 */


//librerías
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"//librería para hacer la comunicación serial

bool ban = false;
char valor = 'b';

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_XTAL_16MHZ);//Se configura el reloj oscilador
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//Se habilita el puerto F como salida
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));//Se espera a que se termine de habilitar el puerto F
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    //Configuración del timer 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); //Se habilita el reloj en el timer 0
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));//Se espera a que se complete la configuración del timer 0
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);//Se toma como base el timer0 y se define que es periódico
    TimerLoadSet(TIMER0_BASE, TIMER_A, 20000000-1);//Se define un ciclo de trabajo de 50%
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);//Se define una interrupción con el timeout del timer 0
    IntEnable(INT_TIMER0A);//Se habilita la interrupción del timer0
    TimerEnable(TIMER0_BASE, TIMER_A);//Se habilita el timer0

    //Configuración del UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); //Se difine que se utilizará el UART0
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));//Espera a que se termine de configurar el UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //Se habilita el puerto RX y TX
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));//Espera a que termine de configurarse los puertos de UART
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 |GPIO_PIN_1);//Se definen los pines del UART
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
    IntEnable(INT_UART0);//Se habilita la interrupción del UART
    UARTIntEnable(UART0_BASE, UART_INT_RX);
    UARTIntDisable(UART0_BASE, UART_INT_9BIT|UART_INT_TX |UART_INT_OE|UART_INT_BE|UART_INT_PE|UART_INT_FE|UART_INT_RT|UART_INT_DSR|UART_INT_DCD |UART_INT_CTS | UART_INT_RI);
    UARTFIFOLevelSet(UART0_BASE,UART_FIFO_TX1_8,UART_FIFO_RX1_8);//Se define el tamaño del FIFO
    UARTEnable(UART0_BASE);//Se habilita el UART0
    while(1);
}


void Timer0IntHandler(void)
{
   TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);//Se limpia la interrupción del timer
   if (!ban)
   {
       if (valor == 'r')//Sentencia para saber si la consola envio el caracter r
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0xff);
       else if (valor == 'b')//Sentencia para saber si la consola envio el caracter b
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0xff);
       else if (valor == 'g')//Sentencia para saber si la consola envio el caracter g
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xff);
       ban = true;
   }
   else
   {
       GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
       ban = false;
   }
}

void UARTIntHandler(void)
{
   valor = 0;
   UARTIntClear(UART0_BASE, UART_INT_RX);//Se limpia la interrupción del UART
   valor = UARTCharGet(UART0_BASE);//Se obtiene el valor de la comunicación
   UARTCharPut(UART0_BASE, valor);
   UARTRxErrorClear(UART0_BASE);
}
