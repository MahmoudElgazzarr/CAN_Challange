/*
 * CAN_Task.h
 *
 *  Created on: Apr 3, 2019
 *      Author: AVE-LAP-040
 */

#ifndef HEADERS_CAN_TASK_H_
#define HEADERS_CAN_TASK_H_

/*Definiations For Source Numbers*/
#define SOURCE_NUM_ZERO 0
#define SOURCE_NUM_ONE 1
#define SOURCE_NUM_TWO 2
#define SOURCE_NUM_THREE 3
#define SOURCE_NUM_FOUR 4

/*Definitions For Destionations Numbers*/
#define Destionation_NUM_ZERO 0
#define Destionation_NUM_ONE 1
#define Destionation_NUM_TWO 2
#define Destionation_NUM_THREE 3
#define Destionation_NUM_FOUR 4


#define PDU_MSG_ID 1

#define Token 0xFF

enum
{
    START_UP_STATE, NORMAL_STATE
};

/*Can Tasks*/
void Can_Send_Init(void);
void Can_Recive_Init(void);
void Can_Send(uint8_t Source_Node , uint8_t Destination_Node);
void Can_Task_Recive(void);

/*Task To Check For Token And Set State Of System*/
void Token_Task();

#endif /* HEADERS_CAN_TASK_H_ */
