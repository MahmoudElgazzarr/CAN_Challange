/*
 * Switch_Driver.c
 *
 *  Created on: Mar 13, 2019
 *      Author: AVE-LAP-040
 */


#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"

void Switch_init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {}
    /*Enable Port D*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    /*Wait while Peripheral ready*/
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
        {}
    /*Make Pin 4 and 0 As Input */
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
    /*Set Pins as Pull Up*/
    GPIOPadConfigSet(GPIO_PORTF_BASE ,GPIO_PIN_4,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTF_BASE ,GPIO_PIN_0,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);
}

uint8_t Switch0_Read(void)
{
    uint8_t Button0_Read = GPIOPinRead(GPIO_PORTF_BASE , GPIO_PIN_4);
            if (Button0_Read == 0)
            {
                return 1;
            }
            else
            {
                return 0;
            }
}
void Switch1_Read(void)
{
    uint8_t Button1_Read = GPIOPinRead(GPIO_PORTF_BASE , GPIO_PIN_4);

            if (Button1_Read == 0)
            {
                return 1;
            }
            else
            {
                return 0;
            }
}
