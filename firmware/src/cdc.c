/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    cdc.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "cdc.h"
#include "app.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the CDC_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

CDC_DATA cdcData;
extern APP_DATA appData;
uint8_t receiveDataBuffer[512] CACHE_ALIGN;
uint32_t numBytesRead; 

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

/* This a sample Application Device Layer Event Handler
 * Note how the CDC Function Driver event handler APP_USBDeviceCDCEventHandler()
 * is registered in the USB_DEVICE_EVENT_CONFIGURED event. The appData
 * object that is passed in the USB_DEVICE_CDC_EventHandlerSet()
 * function will be returned as the userData parameter in the
 * when the APP_USBDeviceCDCEventHandler() function is invoked */

// This code example shows all CDC Function Driver possible events
// and a possible scheme for handling these events. In this case
// event responses are not deferred.

uint16_t breakDuration;
USB_DEVICE_HANDLE    usbDeviceHandle;
USB_CDC_LINE_CODING  lineCoding = {9600, 0 , 0, 8};
USB_CDC_CONTROL_LINE_STATE * controlLineStateData;

USB_DEVICE_CDC_EVENT_RESPONSE USBDeviceCDCEventHandler
(
    USB_DEVICE_CDC_INDEX instanceIndex,
    USB_DEVICE_CDC_EVENT event,
    void * pData,
    uintptr_t userData
)
{
    USB_DEVICE_CDC_EVENT_DATA_READ_COMPLETE * eventDataRead;
    
    switch(event)
    {
        case USB_DEVICE_CDC_EVENT_SET_LINE_CODING:

            // In this case, the application should read the line coding
            // data that is sent by the host.

            USB_DEVICE_ControlReceive(appData.usbDevHandle, &lineCoding,
                                sizeof(USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_GET_LINE_CODING:

            // In this case, the application should send the line coding
            // data to the host.

            USB_DEVICE_ControlSend(appData.usbDevHandle, &lineCoding,
                                sizeof(USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_CONTROL_LINE_STATE:

            // In this case, pData should be interpreted as a
            // USB_CDC_CONTROL_LINE_STATE pointer type.  The application
            // acknowledges the parameters by calling the
            // USB_DEVICE_ControlStatus() function with the
            // USB_DEVICE_CONTROL_STATUS_OK option.

            controlLineStateData = (USB_CDC_CONTROL_LINE_STATE *)pData;
            USB_DEVICE_ControlStatus(appData.usbDevHandle, USB_DEVICE_CONTROL_STATUS_OK);

            break;

        case USB_DEVICE_CDC_EVENT_SEND_BREAK:

            // In this case, pData should be interpreted as a
            // USB_DEVICE_CDC_EVENT_DATA_SEND_BREAK pointer type
            // to the break duration. The application acknowledges
            // the parameters by calling the USB_DEVICE_ControlStatus()
            // function with the USB_DEVICE_CONTROL_STATUS_OK option.

            breakDuration = ((USB_DEVICE_CDC_EVENT_DATA_SEND_BREAK *) pData)->breakDuration;
            USB_DEVICE_ControlStatus(appData.usbDevHandle, USB_DEVICE_CONTROL_STATUS_OK);

            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_SENT:

            // This event indicates the data send request associated with
            // the latest USB_DEVICE_ControlSend() function was
            // completed.  The application could use this event to track
            // the completion of the USB_DEVICE_CDC_EVENT_GET_LINE_CODING
            // request.

            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:

            // This means that the data stage is complete. The data in
            // setLineCodingData is valid or data in getLineCodingData was
            // sent to the host.  The application can now decide whether it
            // supports this data. It is not mandatory to do this in the
            // event handler.

            USB_DEVICE_ControlStatus(appData.usbDevHandle, USB_DEVICE_CONTROL_STATUS_OK);

            break;

        case USB_DEVICE_CDC_EVENT_WRITE_COMPLETE:

            // This means USB_DEVICE_CDC_Write() operation completed.
            // The pData member will point to a
            // USB_DEVICE_CDC_EVENT_DATA_WRITE_COMPLETE type of data.
            cdcData.cdcWriteCompleted = true;
            break;

        case USB_DEVICE_CDC_EVENT_READ_COMPLETE:

            // This means USB_DEVICE_CDC_Read() operation completed.
            // The pData member will point to a
            // USB_DEVICE_CDC_EVENT_DATA_READ_COMPLETE type of data.
            eventDataRead = (USB_DEVICE_CDC_EVENT_DATA_READ_COMPLETE *)pData;
            cdcData.cdcReadCompleted = true;
            cdcData.cdcReadNumBytes = eventDataRead->length;
            break;

        case USB_DEVICE_CDC_EVENT_SERIAL_STATE_NOTIFICATION_COMPLETE:

            // This means USB_DEVICE_CDC_SerialStateNotification() operation
            // completed. The pData member will point to a
            // USB_DEVICE_CDC_EVENT_DATA_SERIAL_STATE_NOTIFICATION_COMPLETE type of data.
            cdcData.cdcSerialStateNotificationCompleted = true;
            break;

         default:
            break;
     }

    return USB_DEVICE_CDC_EVENT_RESPONSE_NONE;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void CDC_Initialize ( void )

  Remarks:
    See prototype in cdc.h.
 */

void CDC_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    cdcData.state = CDC_STATE_INIT;

    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    
}


/******************************************************************************
  Function:
    void CDC_Tasks ( void )

  Remarks:
    See prototype in cdc.h.
 */

void CDC_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( cdcData.state )
    {
        /* Application's initial state. */
        case CDC_STATE_INIT:
        {
            if (appData.deviceIsConfigured)
            {
                cdcData.cdcReadCompleted = false;
                USB_DEVICE_CDC_Read(USB_DEVICE_CDC_INDEX_0, &cdcData.rdTransferHandle, receiveDataBuffer, 512);
                cdcData.state = CDC_STATE_SERVICE_TASKS;
            }
            break;
        }

        case CDC_STATE_SERVICE_TASKS:
        {
            if (!appData.deviceIsConfigured)
            {
                cdcData.state = CDC_STATE_INIT;
                break;
            }
            if (cdcData.cdcReadCompleted)
            {
                SYS_DEBUG_PRINT(SYS_ERROR_INFO, "Recv %d bytes [%s]\r\n",cdcData.cdcReadNumBytes, receiveDataBuffer);
                cdcData.cdcReadCompleted = false;
                USB_DEVICE_CDC_Read(USB_DEVICE_CDC_INDEX_0, &cdcData.rdTransferHandle, receiveDataBuffer, 512);
            }
            break;
        }

        /* TODO: implement your application state machine.*/


        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
