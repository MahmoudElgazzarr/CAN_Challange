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

uint8_t Button0_flag = 0;
uint8_t Token = 0;
uint8_t Checked = 0;

/*Variable To Handle State*/
uint8_t State = START_UP_STATE;

/*Structure To Hold Source And Destionation IDS*/
/*Previous Source Destionation*/
/*4 2 3*/
/*2 3 4*/
/*3 4 2*/
Node_T Node = { 2 , 3 , 2 , Forward };

/*Defines Source Node ID*/
#define First_Node 2
/*Todo Change it . Defines Last Node */
#define Last_Node 4

void Can_Task_Recive(void)
{
    while (1)
    {
        if (g_bRXFlag2 == 1)
        {
            Can_Recive();
            Checked = 1;
        }
        vTaskDelay(5);
    }
}

void Token_Task()
{
    while (1)
    {
        /*If Switch 0 Pressed On First Node Send Token To The Next Node And Intalize Other Nodes*/
        if (Button0_flag == 1)
        {
            /*Send First Token To The First Node To Inailize System*/
            Can_Send(Forward, Node.Destionation_Node, Node.This_Node);
            /*Normal State*/
            State = NORMAL_STATE;
            /*Change State To idle*/
            Button0_flag = 0;
            vTaskDelay(50);
        }
        /*Check Source Node If it Previous Or Next*/
        /*Todo*/
        if ((pui8MsgData_Recived[0] == Forward) && (pui8MsgData_Recived[1] == Node.This_Node) && (Checked == 1))
        {
            /*Change State To Normal State*/
            State = NORMAL_STATE;
            /*Recived Token*/
            Token = 1;
            vTaskDelay(1000);
            Token = 0;
            /*Send Token To The Next Node*/
            Can_Send(Node.Dir, Node.Destionation_Node, Node.This_Node);
            Checked = 0;
        }
        /*If Direction Is Backward*/
        else if ((pui8MsgData_Recived[0] == Backward) && ((pui8MsgData_Recived[1]) == Node.This_Node) && (Checked == 1))
        {
            /*I have Recived Token*/
            Token = 1;
            /*Change State To Normal State*/
            State = NORMAL_STATE;
            vTaskDelay(1000);
            Token = 0;
            /*Try To Send Until Getting Ack*/
            Can_Send(Node.Dir, Node.Previous_Node, Node.This_Node);
            Checked = 0;
        }
        vTaskDelay(10);
    }
}
void Led_Task(void)
{
    while (1)
    {
        if (State == START_UP_STATE)
        {
            /*Turn On Init Led*/
            led1_on();
        }
        else
        {
            led1_off();
        }
        if (Token == 1)
        {
            led2_on();
        }
        else
        {
            led2_off();
        }
        vTaskDelay(10);
    }
}
void Button_Task(void)
{
    while (1)
    {
        /*If Switch 0 Pressed On First Node Inalalize it and Other Nodes*/
        if (Switch0_Read() == 1)
        {
            /*Change State To Normal State*/
            State = NORMAL_STATE;
            /*Set Flag For Button 0*/
            Button0_flag = 1;
        }
        /*If Second Button Pressed Change Direction*/
        if (Switch1_Read() == 1)
        {
            if (Node.Dir == Backward)
            {
                Node.Dir = Forward;
            }
            else if (Node.Dir == Forward)
            {
                Node.Dir = Backward;
            }
        }
        vTaskDelay(50);
    }
}
