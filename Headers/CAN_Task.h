/*
 * CAN_Task.h
 *
 *  Created on: Apr 3, 2019
 *      Author: AVE-LAP-040
 */

#ifndef HEADERS_CAN_TASK_H_
#define HEADERS_CAN_TASK_H_

/*Typedef For Node Struct*/
typedef struct Node
{
    uint8_t Previous_Node;
    uint8_t This_Node;
    uint8_t Destionation_Node;
    uint8_t Dir;
}Node_T;

/*Define Directions*/
enum Directions
{
    Forward = 21,
    Backward = 22,
};
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

enum
{
    START_UP_STATE, NORMAL_STATE,idle
};

/*Can Task*/
void Can_Task_Recive(void);

/*Task To Check For Token And Set State Of System*/
void Token_Task();

/**/
void Led_Task(void);
/*Button Task*/
void Button_Task(void);
#endif /* HEADERS_CAN_TASK_H_ */
