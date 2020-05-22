/**
 * Programador: Israel Arévalo
 *
 * Material: Electrónica digital 2
 *
 * Nombre del archivo: main.c
 *
 * Referencia: Se utilizó como referencia el código de ejemplo que se encontraba en
 *             la guia del laboratorio 9
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
    while(1);
}


void Timer0IntHandler(void)
{
   TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);//Se limpia la interrupción del timer
   if (!ban)
   {
       if (valor == 0x52)
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0xff);
       else if (valor == 0x41)
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0xff);
       else if (valor == 0x56)
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xff);
       ban = true;
   }
   else
   {
       GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
       ban = false;
   }
}
