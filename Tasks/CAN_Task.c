/*Includes Related To FreeRTOS*/
#include <FreeRTOS.h>
#include "task.h"
/*Includes Related To The Task*/
#include "LEDS.h"
#include "CAN_Task.h"
/*Include Can Driver*/
#include "CAN_Driver.h"
/*Include Buttons Driver*/
#include "Switch_Driver.h"

/*Variable To Handle State*/
uint8_t State = START_UP_STATE;

/*Variables To Hold Source And Destionation OF Node*/
/*Least Source Is Two*/
uint8_t Source = 4;
uint8_t Destionation = 5;

/*Variables To Hold Previous Node*/
#define Previous_Source (Source - 1)

/*Variable To Hold Next Source*/
#define Next_Source (Source + 1)

/*Define Max Number Of Nodes*/
#define Max_Nodes 3

void Can_Task_Recive(void)
{
    while (1)
    {
        if (g_bRXFlag2 == 1)
        {

            Can_Recive();
        }

        vTaskDelay(10);
    }
}

void Token_Task()
{
    while (1)
    {
        if (State == START_UP_STATE)
        {
            led1_on();
        }
        /*If Switch 0 Pressed On First Node Send Token To The Next Node And Intalize Other Nodes*/
        if (Switch0_Read() == 1)
        {
            /*Send First Token To The First Node To Inailize System*/
            Can_Send(Source, Destionation);

            /*Change State To Normal State*/
            State = NORMAL_STATE;
        }
        /*Check If Source Node iS Two*/
        /*This Is Node Number 1*/
        if ((pui8MsgData_Recived[0] == Previous_Source)
                && (pui8MsgData_Recived[1] == Source))
        {
            /*Turn Off Led Of init State*/
            led1_off();
            /*Turn On Led That We Have Recived Token*/
            /*Change State To Normal State*/
            State = NORMAL_STATE;
            led2_on();
            vTaskDelay(100);

            /*If Second Button Pressed Change Direction*/
            if (Switch1_Read() == 1)
            {
                /*Send token To The Previous Node*/
                Can_Send(Source, Source - 1);
                led2_off();
            }
            /*Send To The Normal Direction*/
            else
            {
                /*Send token To The Next Node*/
                /*+2 As I Started From Source 2*/
                if (Destionation < (Max_Nodes + 2))
                {
                    Can_Send(Source, Destionation);
                    led2_off();
                }
                else
                {
                    /*Send To The Least Address Node*/
                    /*Destionation Number Two*/
                    Can_Send(Source, SOURCE_NUM_TWO);
                    led2_off();
                }
            }
        }
        /*If Direction Is Changed*/
        else if ((pui8MsgData_Recived[0] == Next_Source)
                && ((pui8MsgData_Recived[1]) == Source))
        {
            /*Turn On Led That We Have Recived Token*/
            /*Change State To Normal State*/
            State = NORMAL_STATE;
            led2_on();
            vTaskDelay(100);
            /*If Second Button Pressed Change Direction*/
            if (Switch1_Read() == 1)
            {
                /*Send token To The Next Node*/
                Can_Send(Source, Source + 1);
                led2_off();
            }
            else
            {
                /*Send token To The Previous Node*/
                Can_Send(Source, Source - 1);
                led2_off();
            }
        }
        /*Todo If Ack Doens't Come From Next Node Send To The Next OF Next and So On Until Ack Recived*/
        vTaskDelay(100);
    }

}

