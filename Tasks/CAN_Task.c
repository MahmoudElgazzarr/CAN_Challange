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

/*Structure To Hold Source And Destionation IDS*/
/*Previous Source Destionation*/
/*8 2 3*/
/*2 3 4*/
/*3 4 2*/
Node_T Node = { 3 , 4 , 5};

/*Defines Source Node ID*/
#define First_Node 2
/*Todo Change it . Defines Last Node */
#define Last_Node 4
/*Ack*/
#define Ack 0x99

/*Define Max Number Of Nodes*/
#define Max_Nodes 3
/*define Max Number OF Nodes*/
#define Max_Number_Of_Nodes 8

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
            /*Turn On Init Led*/
            led1_on();
        }
        /*If Switch 0 Pressed On First Node Send Token To The Next Node And Intalize Other Nodes*/
        if (Switch0_Read() == 1)
        {
            /*Send First Token To The First Node To Inailize System*/
            Can_Send(Node.This_Node, Node.Destionation_Node, 0x00);
            /*Change State To Normal State*/
            State = NORMAL_STATE;
            /*Turn Off Init Led*/
            led1_off();
        }
        /*Check Source Node If it Previous Or Next*/
        /*Todo*/
        if ((pui8MsgData_Recived[0] == Node.Previous_Source)
                && (pui8MsgData_Recived[1] == Node.This_Node))
        {
            /*Turn Off Led Of init State*/
            led1_off();
            /*Turn On Led That We Have Recived Token*/
            led2_on();
            /*Change State To Normal State*/
            State = NORMAL_STATE;
            /*Send Ack*/
            Can_Send(0x00, 0x00 , Ack);
            /*If Second Button Pressed Change Direction*/
            if (Switch1_Read() == 1)
            {
                /*Send token To The Previous Node*/
                led2_off();
                Can_Send(Node.This_Node, Node.Previous_Source,0x00);
            }
            /*Send To The Normal Direction*/
            else
            {
                /*Send To The Next*/
                Can_Send(Node.This_Node, Node.Destionation_Node,0x00);
                if (pui8MsgData_Recived[2] != Ack)
                {
                    if (Node.Destionation_Node == Max_Number_Of_Nodes)
                    {
                        /*Send To The Least Address Node*/
                        /*Destionation Number Two*/
                        Node.Destionation_Node = SOURCE_NUM_TWO;
                    }
                    else
                    {
                        Node.Destionation_Node = Node.Destionation_Node + 1;
                    }
                    Can_Send(Node.This_Node, Node.Destionation_Node,0x00);
                }
                led2_off();
            }
        }
        /*If Direction Is Changed*/
        else if ((pui8MsgData_Recived[0] == Node.Destionation_Node)
                && ((pui8MsgData_Recived[1]) == Node.This_Node))
        {
            /*Turn On Led That We Have Recived Token*/
            /*Change State To Normal State*/
            State = NORMAL_STATE;
            led2_on();
            /*If Second Button Pressed Change Direction*/
            if (Switch1_Read() == 1)
            {
                /*Send token To The Next Node*/
                Can_Send(Node.This_Node, Node.Destionation_Node , 0x00);
                led2_off();
            }
            else
            {
                /*Send token To The Previous Node*/
                led2_off();
                Can_Send(Node.This_Node, Node.Previous_Source,0x00);
            }
        }
        /*Todo If Ack Doens't Come From Next Node Send To The Next OF Next and So On Until Ack Recived*/
        vTaskDelay(50);
    }

}

