/*
 * CAN_Task.h
 *
 *  Created on: Apr 3, 2019
 *      Author: AVE-LAP-040
 */

#ifndef HEADERS_CAN_TASK_H_
#define HEADERS_CAN_TASK_H_


#define SOURCE_NUM_ZERO 0
#define SOURCE_NUM_ONE 1
#define PDU_MSG_ID 1

enum
{
    START_UP_STATE, NORMAL_STATE
};

/*Can Tasks*/
void Can_Send_Init(void);
void Can_Recive_Init(void);
void Can_Task_Send(void);
void Can_Task_Recive(void);

/*Task To Check For Token And Set State Of System*/
void Token_Task();

#endif /* HEADERS_CAN_TASK_H_ */
