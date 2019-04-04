/*
 * LEDS.c
 *
 *  Created on: Mar 13, 2019
 *      Author: AVE-LAP-040
 */

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "LEDS.h"

void LEDS_PORT_init()
{
    //
      // Enable the GPIO port that is used for the on-board LED.
      //
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

      //
      // Check if the peripheral access is enabled.
      //
      while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
      {}
      /*Enable Port D*/
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
      /*Wait while Peripheral ready*/
      while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
          {}
}
void led1_init()
{
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
}
void led2_init()
{
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
}
void led3_init()
{
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
}
void led1_on()
{
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
}

void led1_off()
{
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);
}
void led2_on()
{
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
}
void led2_off()
{
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x0);
}

void led3_on()
{
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
}
void led3_off()
{
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);
}




