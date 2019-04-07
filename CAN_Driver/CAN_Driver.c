/*
 * CAN_Driver.c
 *
 *  Created on: Apr 3, 2019
 *      Author: AVE-LAP-040
 */
/*Includes Related To Tivaware*/
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/can.h"
#include "CAN_Driver.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

/* A counter that keeps track of the number of times the TX interrupt has */
/* occurred, which should match the number of TX messages that were sent */
volatile uint32_t g_ui32MsgCount = 0;

/* A counter that keeps track of the number of times the RX interrupt has */
/* occurred, which should match the number of TX messages that were sent */
volatile uint32_t g_ui32MsgCountRx = 0;

/* A counter that keeps track of the number of times the RX interrupt has */
/* occurred, which should match the number of TX messages that were sent */
volatile uint32_t g_ui32AckCountRx = 0;

/*A flag to indicate that some error occurred*/
volatile bool g_bErrFlag = 0;

// A flag for the interrupt handler to indicate that a message was received.
volatile bool g_bRXFlag2 = 0;

/*Variables To Hold Data of Can To Send*/
tCANMsgObject sCANMessage;
uint32_t ui32MsgData;
uint8_t *pui8MsgData;

/*Variables To Hold Data of Can Recived*/
tCANMsgObject sCANMessage_Recived;
uint32_t ui32MsgData_Recived;
uint8_t *pui8MsgData_Recived;

/*Can Recive Init*/
void Can_Recive_Init()
{
    pui8MsgData_Recived = (uint8_t *) &ui32MsgData_Recived;
    //
    // Initialize a message object to receive CAN messages with ID 0x1001.
    // The expected ID must be set along with the mask to indicate that all
    // bits in the ID must match.
    //
    sCANMessage_Recived.ui32MsgID = 0x55;
    sCANMessage_Recived.ui32MsgIDMask = 0xfffff;
    sCANMessage_Recived.ui32Flags = (MSG_OBJ_RX_INT_ENABLE
            | MSG_OBJ_USE_ID_FILTER);
    sCANMessage_Recived.ui32MsgLen = 8;

    //
    // Change the ID to 0x2001, and load into message object 2 which will be
    // used for receiving any CAN messages with this ID.  Since only the CAN
    // ID field changes, we don't need to reload all the other fields.
    //
    CANMessageSet(CAN0_BASE, 2, &sCANMessage_Recived, MSG_OBJ_TYPE_RX);
}

/*Can Recive Func*/

void Can_Recive(void)
{
    //
    // Reuse the same message object that was used earlier to configure
    // the CAN for receiving messages.  A buffer for storing the
    // received data must also be provided, so set the buffer pointer
    // within the message object.  This same buffer is used for all
    // messages in this example, but your application could set a
    // different buffer each time a message is read in order to store
    // different messages in different buffers.
    //
    sCANMessage_Recived.pui8MsgData = pui8MsgData_Recived;
    //
    // Read the message from the CAN.  Message object number 2 is used
    // (which is not the same thing as CAN ID).  The interrupt clearing
    // flag is not set because this interrupt was already cleared in
    // the interrupt handler.
    //
    CANMessageGet(CAN0_BASE, 2, &sCANMessage_Recived, 0);

    //
    // Clear the pending message flag so that the interrupt handler can
    // set it again when the next message arrives.
    //
    g_bRXFlag2 = 0;
    //
    // Print information about the message just received.
    //
    UARTprintf("Reciving msg: 0x%02X %02X %02X %02X", pui8MsgData_Recived[0],
               pui8MsgData_Recived[1], pui8MsgData_Recived[2],
               pui8MsgData_Recived[3]);
    UARTprintf(" total count = %u\n", g_ui32MsgCountRx);
}
/*Can Send Init*/
void Can_Send_Init(void)
{
    pui8MsgData = (uint8_t *) &ui32MsgData;
    //
    // For this example CAN0 is used with RX and TX pins on port B4 and B5.
    // The actual port and pins used may be different on your part, consult
    // the data sheet for more information.
    // GPIO port B needs to be enabled so these pins can be used.
    // TODO: change this to whichever GPIO port you are using
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //
    // Configure the GPIO pin muxing to select CAN0 functions for these pins.
    // This step selects which alternate function is available for these pins.
    // This is necessary if your part supports GPIO pin function muxing.
    // Consult the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using
    //
    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);

    //
    // Enable the alternate function on the GPIO pins.  The above step selects
    // which alternate function is available.  This step actually enables the
    // alternate function instead of GPIO for these pins.
    // TODO: change this to match the port/pin you are using
    //
    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    //
    // The GPIO port and pins have been set up for CAN.  The CAN peripheral
    // must be enabled.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

    //
    // Initialize the CAN controller
    //
    CANInit(CAN0_BASE);
    /*Set Bit Rate*/
    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);
    //
    // Enable interrupts on the CAN peripheral.  This example uses static
    // allocation of interrupt handlers which means the name of the handler
    // is in the vector table of startup code.  If you want to use dynamic
    // allocation of the vector table, then you must also call CANIntRegister()
    // here.
    //
    // CANIntRegister(CAN0_BASE, CANIntHandler); // if using dynamic vectors
    //
    CANIntEnable(CAN0_BASE,
    CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);

    //
    // Enable the CAN interrupt on the processor (NVIC).
    //
    IntEnable(INT_CAN0);

    //
    // Enable the CAN for operation.
    //
    CANEnable(CAN0_BASE);
    /*Set Mail Box That Will Send PDU Message*/
    ui32MsgData = 0;
    sCANMessage.ui32MsgID = 0x55;
    sCANMessage.ui32MsgIDMask = 0;
    sCANMessage.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    sCANMessage.ui32MsgLen = 8;
    sCANMessage.pui8MsgData = pui8MsgData;

}

void Can_Send(uint8_t Source_Node, uint8_t Destination_Node , uint8_t Ack)
{
    //
    // Print a message to the console showing the message count and the
    // contents of the message being sent.
    //
    pui8MsgData = (uint8_t *) &ui32MsgData;

    /*Set PDU Message Data && this is one byte*/

    /*Set Destination On First Mail Box*/
    pui8MsgData[0] = Source_Node;
    pui8MsgData[1] = Destination_Node;
    pui8MsgData[2] = Ack;
    //
    // Send the CAN message using object number 1 (not the same thing as
    // CAN ID, which is also 1 in this example).  This function will cause
    // the message to be transmitted right away.
    //
    CANMessageSet(CAN0_BASE, 1, &sCANMessage, MSG_OBJ_TYPE_TX);
    if (g_bErrFlag)
    {
        UARTprintf(" error - cable connected?\n");
    }
    else
    {
        UARTprintf("Sending msg: 0x%02X %02X %02X %02X", pui8MsgData[0],
                   pui8MsgData[1], pui8MsgData[2], pui8MsgData[3]);
        //
        // If no errors then print the count of message sent
        //
        UARTprintf(" total count = %u\n", g_ui32MsgCount);
    }

    //
    // Increment the value in the message data.
    //
    ui32MsgData++;
}

/*Can Interrupt Handler*/
void CANIntHandler(void)
{
    uint32_t ui32Status;

    //
    // Read the CAN interrupt status to find the cause of the interrupt
    //
    ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

    //
    // If the cause is a controller status interrupt, then get the status
    //
    if (ui32Status == CAN_INT_INTID_STATUS)
    {
        //
        // Read the controller status.  This will return a field of status
        // error bits that can indicate various errors.  Error processing
        // is not done in this example for simplicity.  Refer to the
        // API documentation for details about the error status bits.
        // The act of reading this status will clear the interrupt.  If the
        // CAN peripheral is not connected to a CAN bus with other CAN devices
        // present, then errors will occur and will be indicated in the
        // controller status.
        //
        ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);

        //
        // Set a flag to indicate some errors may have occurred.
        //
        g_bErrFlag = 1;
    }

    //
    // Check if the cause is message object 1, which what we are using for
    // sending messages.
    //
    else if (ui32Status == 1)
    {
        //
        // Getting to this point means that the TX interrupt occurred on
        // message object 1, and the message TX is complete.  Clear the
        // message object interrupt.
        //
        CANIntClear(CAN0_BASE, 1);

        //
        // Increment a counter to keep track of how many messages have been
        // sent.  In a real application this could be used to set flags to
        // indicate when a message is sent.
        //
        //
        // Set flag to indicate received message is pending.
        //

        g_ui32MsgCount++;

        //
        // Since the message was sent, clear any error flags.
        //
        g_bErrFlag = 0;

        /*Todo To Be Changed*/
        //CANIntClear(CAN0_BASE, 2);
        //g_ui32MsgCountRx++;
    }
    else if (ui32Status == 2)
    {

        CANIntClear(CAN0_BASE, 2);

        // Set flag to indicate received message is pending.
        g_ui32MsgCountRx++;
        g_bRXFlag2 = 1;
        g_bErrFlag = 0;

    }

    //
    // Otherwise, something unexpected caused the interrupt.  This should
    // never happen.
    //
    else
    {
        //
        // Spurious interrupt handling can go here.
        //
    }
}

//*****************************************************************************
//
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//
//*****************************************************************************

void InitConsole(void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Enable UART0 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Select the alternate (UART) function for these pins.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

