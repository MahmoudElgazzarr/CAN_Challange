/*
 * CAN_Driver.h
 *
 *  Created on: Apr 3, 2019
 *      Author: AVE-LAP-040
 */

#ifndef HEADERS_CAN_DRIVER_H_
#define HEADERS_CAN_DRIVER_H_


#include <stdbool.h>
#include "driverlib/can.h"
/* A counter that keeps track of the number of times the TX interrupt has */
/* occurred, which should match the number of TX messages that were sent */
extern volatile uint32_t g_ui32MsgCount;

/* A counter that keeps track of the number of times the RX interrupt has */
/* occurred, which should match the number of TX messages that were sent */
extern volatile uint32_t g_ui32MsgCountRx;

/*A flag to indicate that some error occurred*/
extern volatile bool g_bErrFlag;

// A flag for the interrupt handler to indicate that a message was received.
extern volatile bool g_bRXFlag2;

/*Variables To Hold Data of Can To Send*/
extern tCANMsgObject sCANMessage;
extern uint32_t ui32MsgData;
extern uint8_t *pui8MsgData;

/*Variables To Hold Data of Can Recived*/
extern tCANMsgObject sCANMessage_Recived;
extern uint32_t ui32MsgData_Recived;
extern uint8_t *pui8MsgData_Recived;

void Can_Send_Init(void);
void Can_Recive_Init(void);
void Can_Send(uint8_t Source_Node , uint8_t Destination_Node);
void Can_Recive(void);


#endif /* HEADERS_CAN_DRIVER_H_ */
