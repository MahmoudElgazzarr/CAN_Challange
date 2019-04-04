



#include <FreeRTOS.h>
#include <stdint.h>
#include "task.h"
#include "LEDS.h"
#include "led_task.h"

void LED_Task()
{
    while(1)
    {
        led2_on();
        vTaskDelay(50);
    }
}
void LED2_Task()
{
    while(1)
    {
        led2_off();
        vTaskDelay(100);
    }
}
void LEDS_Task_init()
{
    while(1)
    {
    LEDS_PORT_init();
    led1_init();
    led2_init();
    led3_init();
    vTaskSuspend(NULL);
    }
}
