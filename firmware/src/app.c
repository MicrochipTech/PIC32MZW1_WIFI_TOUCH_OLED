/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

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

#include "app.h"
#include "oled_w.h"
#include "definitions.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
// Touch
extern volatile uint8_t measurement_done_touch ;
uint8_t button1_value ;
uint8_t button2_value ;
uint8_t slider_value ;
uint16_t scroller_position ;
// Display
volatile bool refreshDisplay = false ;
// Net service
SYS_MODULE_OBJ g_tcpServHandle = SYS_MODULE_OBJ_INVALID ;
SYS_NET_Config g_tcpServCfg ;
#define RECV_BUFFER_LEN 1400
uint8_t recv_buffer[RECV_BUFFER_LEN + 1] ;

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/
void DisplayCallback(uintptr_t context)
{
    refreshDisplay = true ;
}

void TcpServCallback(uint32_t event, void *data, void *cookie)
{
    char message[25] ;
    switch (event)
    {
        case SYS_NET_EVNT_CONNECTED:
        {
            SYS_CONSOLE_PRINT("TcpServCallback(): Status UP\r\n") ;
            sprintf(message, "{\"data\":\"Hello\"}") ;
            while(SYS_NET_SendMsg(g_tcpServHandle, (uint8_t*)message , strlen(message)) <= 0) ;
            break ;
        }
        case SYS_NET_EVNT_DISCONNECTED:
        {
            SYS_CONSOLE_PRINT("TcpServCallback(): Status DOWN\r\n") ;
            break;
        }
        case SYS_NET_EVNT_RCVD_DATA:
        {
			int32_t cumm_len = 0 ;
            int32_t len = RECV_BUFFER_LEN ;
            while(len == RECV_BUFFER_LEN)
            {
                len = SYS_NET_RecvMsg(g_tcpServHandle, recv_buffer, RECV_BUFFER_LEN) ;
                if(len > 0)
                {
                    if(cumm_len == 0)
                    {
                        uint8_t buffer[33] ;
                        int32_t tmp_len = (len > 32)? 32 : len ;
                        memcpy(buffer, recv_buffer, tmp_len) ;
                        buffer[tmp_len] = '\0' ;
                        SYS_CONSOLE_PRINT("TcpServCallback(): Data Received = %s\r\n", buffer) ;
                    }
                    cumm_len += len ;
                    SYS_NET_SendMsg(g_tcpServHandle, recv_buffer, len) ;
                }
            }
            SYS_CONSOLE_PRINT("TcpServCallback(): Total Data Received = %d Bytes\r\n", cumm_len) ;
            break;
        }        
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/

void DelayMs(uint32_t delay)
{
    SYS_TIME_HANDLE timer = SYS_TIME_HANDLE_INVALID;
    if (SYS_TIME_DelayMS(delay, &timer) != SYS_TIME_SUCCESS)
    {
        // Handle error
    } 
    else if (SYS_TIME_DelayIsComplete(timer) != true) 
    {
        // Wait till the delay has not expired
        while (SYS_TIME_DelayIsComplete(timer) == false) ;
    }    
}

static void touch_status_display(void)
{
    uint8_t button1_temp, button2_temp ;
    char message[25] ;
    /* Button 1 */
    button1_temp = get_sensor_state(0) & 0x80 ;
    if (button1_temp != button1_value)
    {
        button1_value = button1_temp ;
        memset(message, 0, 20) ;
        sprintf(message, "{\"Button1\":%d}", button1_value) ;
        if (SYS_NET_STATUS_CONNECTED == SYS_NET_GetStatus(g_tcpServHandle))
        {
            while(SYS_NET_SendMsg(g_tcpServHandle, (uint8_t*)message, strlen(message)) <= 0) ;
        }
    }
    if (button1_value)
    {
        OLED_DrawFilledCircle(6, 0) ;
    }
    else
    {
        OLED_DrawCircle(6, 0) ;     
    }
    /* Button 2 */
    button2_temp = get_sensor_state(1) & 0x80 ;
    if (button2_temp != button2_value)
    {
        button2_value = button2_temp ;
        memset(message, 0, 20) ;
        sprintf(message, "{\"Button2\":%d}", button2_value) ;
        if (SYS_NET_STATUS_CONNECTED == SYS_NET_GetStatus(g_tcpServHandle))
        {
            while(SYS_NET_SendMsg(g_tcpServHandle, (uint8_t*)message, strlen(message)) <= 0) ;
        }
    }
    if (button2_value)
    {
        OLED_DrawFilledCircle(6, 3) ;
    }
    else
    {
        OLED_DrawCircle(6, 3) ;
    }    
    /* Scroller */
    if (get_scroller_state(0) != 0)
    {
        scroller_position = get_scroller_position(0) ;
        if (slider_value != scroller_position)
        {
            slider_value = scroller_position ;
            memset(message, 0, 20) ;
            sprintf(message, "{\"Slider\":%d}", slider_value) ;
            if (SYS_NET_STATUS_CONNECTED == SYS_NET_GetStatus(g_tcpServHandle))
            {
                while(SYS_NET_SendMsg(g_tcpServHandle, (uint8_t*)message, strlen(message)) <= 0) ;
            }
        }
        OLED_PutInt(8, 2, slider_value) ;
        OLED_DrawRectangle(RECTANGLE_POS_X, 3) ;
        OLED_DrawCursor(CURSOR_MIN_POS + (slider_value/4), 3) ;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    SYS_TIME_CallbackRegisterMS(DisplayCallback, (uintptr_t)NULL, 200, SYS_TIME_PERIODIC) ;
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            OLED_Init() ;
            OLED_SplashScreen() ;
            DelayMs(2000) ;
            OLED_MainScreen() ;
            
            g_tcpServCfg.mode = SYS_NET_INDEX0_MODE ;
            g_tcpServCfg.port = SYS_NET_INDEX0_PORT ;
            g_tcpServCfg.enable_tls = SYS_NET_INDEX0_ENABLE_TLS ;
            g_tcpServCfg.ip_prot = SYS_NET_INDEX0_IPPROT ;
            g_tcpServCfg.enable_reconnect = SYS_NET_INDEX0_RECONNECT ;
            g_tcpServHandle = SYS_NET_Open(&g_tcpServCfg, TcpServCallback, 0) ;
            if(g_tcpServHandle != SYS_MODULE_OBJ_INVALID)
                SYS_CONSOLE_PRINT("TCP Service Initialized Successfully\r\n");
            
            appData.state = APP_STATE_SERVICE_TASKS;
            break;
        }
        case APP_STATE_SERVICE_TASKS:
        {
            SYS_NET_Task(g_tcpServHandle) ;
            touch_process() ;
            if (measurement_done_touch == 1)
            {
                touch_status_display() ;
                measurement_done_touch = 0 ;
            }
            if (refreshDisplay)
            {
                refreshDisplay = false ;
                LED_GREEN_Toggle() ;
            }
            break;
        }
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
