//////////////////////////
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "drivers/buttons.h"
#include "drivers/pinout.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"
#include "io.h"
#include "sensorlib/hw_sht21.h"
#include "sensorlib/i2cm_drv.h"
#include "sensorlib/sht21.h"
#include "simplelink.h"
#include "sl_common.h"
#include "utils/uartstdio.h"
#include "protocol.h"
////
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/PWM.c>
/* Example/Board Header files */
#include "board2.h"

unsigned long PWMEnable = 1;


#define TASKSTACKSIZE   2048

Task_Struct tsk0Struct;
UInt8 tsk0Stack[TASKSTACKSIZE];
Task_Handle task;
void I2C8Init();
void IoSetTimer(unsigned long ui32SpeedPercent);
unsigned int Decimal2String(char* pcBuf, int number);
unsigned int String2Decimal(char *pcBuf);
void IOEnablePWM(unsigned long enable);

//*****************************************************************************
//
// Define SHT21 I2C Address.
//
//*****************************************************************************
#define SHT21_I2C_ADDRESS  0x40

//*****************************************************************************
//
// Global instance structure for the I2C master driver.
//
//*****************************************************************************
tI2CMInstance g_sI2CInst;

//*****************************************************************************
//
// Global instance structure for the SHT21 sensor driver.
//
//*****************************************************************************
tSHT21 g_sSHT21Inst;

//*****************************************************************************
//
// Global variable to hold the actual system clock speed.
//
//*****************************************************************************
uint32_t g_ui32SysClock;

//*****************************************************************************
//
// Global new data flag to alert main that TMP006 data is ready.
//
//*****************************************************************************
volatile uint_fast8_t g_vui8DataFlag;

//*****************************************************************************
//
// Global new error flag to store the error condition if encountered.
//
//*****************************************************************************
volatile uint_fast8_t g_vui8ErrorFlag;

//*****************************************************************************
//
// Application function to capture ASSERT failures and other debug conditions.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//*****************************************************************************
//
// SHT21 Sensor callback function.  Called at the end of SHT21 sensor driver
// transactions. This is called from I2C interrupt context. Therefore, we just
// set a flag and let main do the bulk of the computations and display.
//
//*****************************************************************************
void
SHT21AppCallback(void *pvCallbackData, uint_fast8_t ui8Status)
{

    if(ui8Status == I2CM_STATUS_SUCCESS)
    {
        g_vui8DataFlag = 1;
    }

    //
    // Store the most recent status in case it was an error condition.
    //
    g_vui8ErrorFlag = ui8Status;
}

//*****************************************************************************
//
// SHT21 Application error handler.
//
//*****************************************************************************
void
SHT21AppErrorHandler(char *pcFilename, uint_fast32_t ui32Line)
{
    uint32_t ui32LEDState;


    ui32LEDState &= ~CLP_D2;

    //
    // Go to sleep wait for interventions.  A more robust application could
    // attempt corrective actions here.
    //
    while(1)
    {
        //
        // Toggle LED D1 to indicate the error.
        //
        ui32LEDState ^= CLP_D1;

        ROM_SysCtlDelay(g_ui32SysClock / (10 * 3));

    }
}

//*****************************************************************************
//
// Called by the NVIC as a result of I2C8 Interrupt. I2C8 is the I2C connection
// to the SHT21.
//
//
//*****************************************************************************
void
SHT21I2CIntHandler(void)
{
    //
    // Pass through to the I2CM interrupt handler provided by sensor library.
    // This is required to be at application level so that I2CMIntHandler can
    // receive the instance structure pointer as an argument.
    //
    I2CMIntHandler(&g_sI2CInst);
}

//*****************************************************************************
//
// Function to wait for the SHT21 transactions to complete.
//
//*****************************************************************************
void
SHT21AppI2CWait(char *pcFilename, uint_fast32_t ui32Line)
{
    //
    // Put the processor to sleep while we wait for the I2C driver to
    // indicate that the transaction is complete.
    //
    while((g_vui8DataFlag == 0) && (g_vui8ErrorFlag == 0))
    {
        ROM_SysCtlSleep();
    }

    //
    // If an error occurred call the error handler immediately.
    //
    if(g_vui8ErrorFlag)
    {
        SHT21AppErrorHandler(pcFilename, ui32Line);
    }

    //
    // clear the data flag for next use.
    //
    g_vui8DataFlag = 0;
}

//*****************************************************************************
//
// If buffered mode is defined, set aside RX and TX buffers and read/write
// pointers to control them.
//
//*****************************************************************************
#ifdef UART_BUFFERED

//*****************************************************************************
//
// This global controls whether or not we are echoing characters back to the
// transmitter.  By default, echo is enabled but if using this module as a
// convenient method of implementing a buffered serial interface over which
// you will be running an application protocol, you are likely to want to
// disable echo by calling UARTEchoSet(false).
//
//*****************************************************************************
static bool g_bDisableEcho;

//*****************************************************************************
//
// Output ring buffer.  Buffer is full if g_ui32UARTTxReadIndex is one ahead of
// g_ui32UARTTxWriteIndex.  Buffer is empty if the two indices are the same.
//
//*****************************************************************************
static unsigned char g_pcUARTTxBuffer[UART_TX_BUFFER_SIZE];
static volatile uint32_t g_ui32UARTTxWriteIndex = 0;
static volatile uint32_t g_ui32UARTTxReadIndex = 0;

//*****************************************************************************
//
// Input ring buffer.  Buffer is full if g_ui32UARTTxReadIndex is one ahead of
// g_ui32UARTTxWriteIndex.  Buffer is empty if the two indices are the same.
//
//*****************************************************************************
static unsigned char g_pcUARTRxBuffer[UART_RX_BUFFER_SIZE];
static volatile uint32_t g_ui32UARTRxWriteIndex = 0;
static volatile uint32_t g_ui32UARTRxReadIndex = 0;

//*****************************************************************************
//
// Macros to determine number of free and used bytes in the transmit buffer.
//
//*****************************************************************************
#define TX_BUFFER_USED          (GetBufferCount(&g_ui32UARTTxReadIndex,  \
                                                &g_ui32UARTTxWriteIndex, \
                                                UART_TX_BUFFER_SIZE))
#define TX_BUFFER_FREE          (UART_TX_BUFFER_SIZE - TX_BUFFER_USED)
#define TX_BUFFER_EMPTY         (IsBufferEmpty(&g_ui32UARTTxReadIndex,   \
                                               &g_ui32UARTTxWriteIndex))
#define TX_BUFFER_FULL          (IsBufferFull(&g_ui32UARTTxReadIndex,  \
                                              &g_ui32UARTTxWriteIndex, \
                                              UART_TX_BUFFER_SIZE))
#define ADVANCE_TX_BUFFER_INDEX(Index) \
                                (Index) = ((Index) + 1) % UART_TX_BUFFER_SIZE

//*****************************************************************************
//
// Macros to determine number of free and used bytes in the receive buffer.
//
//*****************************************************************************
#define RX_BUFFER_USED          (GetBufferCount(&g_ui32UARTRxReadIndex,  \
                                                &g_ui32UARTRxWriteIndex, \
                                                UART_RX_BUFFER_SIZE))
#define RX_BUFFER_FREE          (UART_RX_BUFFER_SIZE - RX_BUFFER_USED)
#define RX_BUFFER_EMPTY         (IsBufferEmpty(&g_ui32UARTRxReadIndex,   \
                                               &g_ui32UARTRxWriteIndex))
#define RX_BUFFER_FULL          (IsBufferFull(&g_ui32UARTRxReadIndex,  \
                                              &g_ui32UARTRxWriteIndex, \
                                              UART_RX_BUFFER_SIZE))
#define ADVANCE_RX_BUFFER_INDEX(Index) \
                                (Index) = ((Index) + 1) % UART_RX_BUFFER_SIZE
#endif



#ifdef UART_BUFFERED
//*****************************************************************************
//
// The list of possible interrupts for the console UART.
//
//*****************************************************************************
static const uint32_t g_ui32UARTInt[3] =
{
    INT_UART0, INT_UART1, INT_UART2
};

//*****************************************************************************
//
// The port number in use.
//
//*****************************************************************************
static uint32_t g_ui32PortNum;
#endif


#define APPLICATION_VERSION "1.1.0"

#define SL_STOP_TIMEOUT        0xFF

/* Application specific status/error codes */
typedef enum{
    DEVICE_NOT_IN_STATION_MODE = -0x7D0,        /* Choosing this number to avoid overlap w/ host-driver's error codes */

    STATUS_CODE_MAX = -0xBB8
}e_AppStatusCodes;

/* The tokens cna be declared as const to be placed in Flash. Declared as variables in RAM for faster access */
/* definition of http put events    */
_u8 POST_token[] = "__SL_P_L.D";
_u8 GET_token[]  = "__SL_G_LED";
_u8 POST_token0[] = "__SL_P_U00"; /* turn on LED */
_u8 POST_token1[] = "__SL_P_U01"; /* turn off LED */
_u8 POST_token2[] = "__SL_P_U02"; /* turn on LED toggle */
_u8 POST_token3[] = "__SL_P_U03"; /* turn off LED toggle */
_u8 POST_token4[] = "__SL_P_U04"; /* set LED toggling speed */
_u8 POST_token5[] = "__SL_P_U05"; /* clear button 1 count   */
_u8 POST_token6[] = "__SL_P_U06"; /* start PWM   */
_u8 POST_token7[] = "__SL_P_U07"; /* update temperature reading */
_u8 POST_token8[] = "__SL_P_U08"; /* set temperature unit in C  */
_u8 POST_token9[] = "__SL_P_U09"; /* set temperature unit in F  */
_u8 POST_token10[] = "__SL_P_U10"; /* PAUSE PWM  */

/* definition of http get events    */
_u8 GET_token0[]  = "__SL_G_U00"; /* return LED on */
_u8 GET_token1[]  = "__SL_G_U01"; /* return LED off */
_u8 GET_token2[]  = "__SL_G_U02"; /* return LED toggle on */
_u8 GET_token3[]  = "__SL_G_U03"; /* return LED toggle off */
_u8 GET_token4[]  = "__SL_G_U04"; /* return LED toogle speed */
_u8 GET_token5[]  = "__SL_G_U05"; /* return button 1 count  */
_u8 GET_token6[]  = "__SL_G_U06"; /* reutrn START PWM  */
_u8 GET_token7[]  = "__SL_G_U07"; /* return temperature */
_u8 GET_token8[]  = "__SL_G_U08"; /* return temperature unit in C */
_u8 GET_token9[]  = "__SL_G_U09"; /* return temperature unit in F */
_u8 GET_token10[]  = "__SL_P_U10"; /* return PAUSE PWM */

_u32  g_Status = 0;
_u8 g_auth_name[MAX_AUTH_NAME_LEN+1];
_u8 g_auth_password[MAX_AUTH_PASSWORD_LEN+1];
_u8 g_auth_realm[MAX_AUTH_REALM_LEN+1];

_u8 g_domain_name[MAX_DOMAIN_NAME_LEN];
_u8 g_device_urn[MAX_DEVICE_URN_LEN];


/*
 * STATIC FUNCTION DEFINITIONS -- Start
 */
static _i32 configureSimpleLinkToDefaultState();
static _i32 initializeAppVariables();


static _i32 set_authentication_check (_u8 enable);
static _i32 get_auth_name (_u8 *auth_name);
static _i32 get_auth_password (_u8 *auth_password);
static _i32 get_auth_realm (_u8 *auth_realm);
static _i32 get_device_urn (_u8 *device_urn);
static _i32 get_domain_name (_u8 *domain_name);

_i32 set_port_number(_u16 num);

void HttpServer();

extern Semaphore_Handle Semaphore_CC3100_Req;
extern int delayTime ;
float fTemperature, fHumidity;
int32_t i32IntegerPart;
int32_t i32FractionPart;
uint32_t ui32LEDState;
uint32_t ui32SysClock;
char ptrGlobalTemp[];
char ptrGlobalHumid[];
int number;
void getTempHumid(int*,int*);
void PinoutSetDevice(bool bEthernet, bool bUSB);
//////

/*
 * ASYNCHRONOUS EVENT HANDLERS -- Start
 */

/*!
    \brief This function handles WLAN events

    \param[in]      pWlanEvents is the event passed to the handler

    \return         none

    \note

    \warning
*/
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent)
{
    if(pWlanEvent == NULL)
        CLI_Write(" [WLAN EVENT] NULL Pointer Error \n\r");

    switch(pWlanEvent->Event)
    {
        case SL_WLAN_CONNECT_EVENT:
        {
            SET_STATUS_BIT(g_Status, STATUS_BIT_CONNECTION);

            /*
             * Information about the connected AP (like name, MAC etc) will be
             * available in 'slWlanConnectAsyncResponse_t' - Applications
             * can use it if required
             *
             * slWlanConnectAsyncResponse_t *pEventData = NULL;
             * pEventData = &pWlanEvent->EventData.STAandP2PModeWlanConnected;
             *
             */
        }
        break;

        case SL_WLAN_DISCONNECT_EVENT:
        {
            slWlanConnectAsyncResponse_t*  pEventData = NULL;

            CLR_STATUS_BIT(g_Status, STATUS_BIT_CONNECTION);
            CLR_STATUS_BIT(g_Status, STATUS_BIT_IP_ACQUIRED);

            pEventData = &pWlanEvent->EventData.STAandP2PModeDisconnected;

            /* If the user has initiated 'Disconnect' request, 'reason_code' is SL_USER_INITIATED_DISCONNECTION */
            if(SL_USER_INITIATED_DISCONNECTION == pEventData->reason_code)
            {
                CLI_Write((_u8 *)" Device disconnected from the AP on application's request \n\r");
            }
            else
            {
                CLI_Write((_u8 *)" Device disconnected from the AP on an ERROR..!! \n\r");
            }
        }
        break;

        case SL_WLAN_STA_CONNECTED_EVENT:
        {
            SET_STATUS_BIT(g_Status, STATUS_BIT_STA_CONNECTED);
        }
        break;

        case SL_WLAN_STA_DISCONNECTED_EVENT:
        {
            CLR_STATUS_BIT(g_Status, STATUS_BIT_STA_CONNECTED);
            CLR_STATUS_BIT(g_Status, STATUS_BIT_IP_LEASED);
        }
        break;

        default:
        {
            CLI_Write((_u8 *)" [WLAN EVENT] Unexpected event \n\r");
        }
        break;
    }
}

/*!
    \brief This function handles callback for the HTTP server events

    \param[in]      pEvent - Contains the relevant event information
    \param[in]      pResponse - Should be filled by the user with the
                    relevant response information

    \return         None
*/
void HttpServerCallback(SlHttpServerEvent_t *pEvent,
                                  SlHttpServerResponse_t *pResponse)
{
    unsigned int ulSpeed, n;
    int humid = 0, temp = 0;
    if(pEvent == NULL || pResponse == NULL)
        CLI_Write(" [HTTP EVENT] NULL Pointer Error \n\r");

    switch (pEvent->Event)
    {
        case SL_NETAPP_HTTPGETTOKENVALUE_EVENT:
        {
            _u8 status = 0;
            _u8 *ptr = 0;

            ptr = pResponse->ResponseData.token_value.data;
            pResponse->ResponseData.token_value.len = 0;

  /* Check if the token is for the original CC3100 demo    */
            if(pal_Memcmp(pEvent->EventData.httpTokenName.data, GET_token,
                                         pal_Strlen(GET_token)) == 0)
            {
                status = GetLEDStatus();
                pal_Memcpy(ptr, "LED1_", pal_Strlen("LED1_"));
                ptr += 5;
                pResponse->ResponseData.token_value.len += 5;
                if(status & 0x01)
                {
                    pal_Memcpy(ptr, "ON", pal_Strlen("ON"));
                    ptr += 2;
                    pResponse->ResponseData.token_value.len += 2;
                }
                else
                {
                    pal_Memcpy(ptr, "OFF", pal_Strlen("OFF"));
                    ptr += 3;
                    pResponse->ResponseData.token_value.len += 3;
                }
                pal_Memcpy(ptr,",LED2_", pal_Strlen(",LED2_"));
                ptr += 6;
                pResponse->ResponseData.token_value.len += 6;
                if(status & 0x02)
                {
                    pal_Memcpy(ptr, "ON", pal_Strlen("ON"));
                    ptr += 2;
                    pResponse->ResponseData.token_value.len += 2;
                }
                else
                {
                    pal_Memcpy(ptr, "OFF", pal_Strlen("OFF"));
                    ptr += 3;
                    pResponse->ResponseData.token_value.len += 3;
                }

                *ptr = '\0';
            }

/* check if the get LED on event, return the LED on status  */
           if(pal_Memcmp(pEvent->EventData.httpTokenName.data, GET_token0,
                                                    pal_Strlen(GET_token0)) == 0)
           {
        	   CLI_Write((_u8 *)"D2 on\n");
        	   if(g_ui32LED_Status)
        	   {
        		   pal_Memcpy(ptr, "checked", pal_Strlen("checked")); /* return the on status to update the "LED ON" button */
        		   ptr += 7;
        		   pResponse->ResponseData.token_value.len += 7;
        	   }
        	   else
        	   {
        		   pal_Memcpy(ptr, " ", pal_Strlen(" "));            /* do not update the "LED ON" button */
        	 	   ptr += 1;
        	       pResponse->ResponseData.token_value.len += 1;
        	   }
 			   *ptr = '\0';
           }

 /* check if the get LED off event, return the LED off status */
           else if(pal_Memcmp(pEvent->EventData.httpTokenName.data, GET_token1,
							   pal_Strlen(GET_token1)) == 0)
		  {
        	   CLI_Write((_u8 *)"D2 off\n");
			   if(!g_ui32LED_Status)
			   {
				   pal_Memcpy(ptr, "checked", pal_Strlen("checked")); /* return the off status to update the "LED OFF" button */
				   ptr += 7;
				   pResponse->ResponseData.token_value.len += 7;
			   }
			   else
			   {
				   pal_Memcpy(ptr, " ", pal_Strlen(" "));            /* do not update the "LED OFF" button */
				   ptr += 1;
				   pResponse->ResponseData.token_value.len += 1;
			   }
		  	   *ptr = '\0';
		  }

/* check if the get LED toggle on event */
           else if(pal_Memcmp(pEvent->EventData.httpTokenName.data, GET_token2,
		  		 							   pal_Strlen(GET_token2)) == 0)
		  {
        	   CLI_Write((_u8 *)"D2 toggle\n");
			   if(g_ui32LED_Toggle_Status)
			   {
				   pal_Memcpy(ptr, "checked", pal_Strlen("checked"));    /* return the on status to update the "LED toggle on" button */
				   ptr += 7;
				   pResponse->ResponseData.token_value.len += 7;
			   }
			   else
			   {
				   pal_Memcpy(ptr, " ", pal_Strlen(" "));               /* do not update the "LED toggle on" button */
				   ptr += 1;
				   pResponse->ResponseData.token_value.len += 1;
			   }
			   *ptr = '\0';
		  }

/* check if the get LED toggle off event */
           else if(pal_Memcmp(pEvent->EventData.httpTokenName.data, GET_token3,
		 							   pal_Strlen(GET_token3)) == 0)
		  {
        	   CLI_Write((_u8 *)"D2 toggle off\n");
			   if(!g_ui32LED_Toggle_Status)
			   {
				   pal_Memcpy(ptr, "checked", pal_Strlen("checked"));   /* return the off status to update the "LED toggle off" button */
				   ptr += 7;
				   pResponse->ResponseData.token_value.len += 7;
			   }
			   else
			   {
				   pal_Memcpy(ptr, " ", pal_Strlen(" "));                /* do not update the "LED toggle off" button */
				   ptr += 1;
				   pResponse->ResponseData.token_value.len += 1;
			   }
			   *ptr = '\0';
		  }

/* check if the get speed event  */
           else if(pal_Memcmp(pEvent->EventData.httpTokenName.data, GET_token4,
										   pal_Strlen(GET_token4)) == 0)
		  {
        	   CLI_Write((_u8 *)"speed is set\n");
			   n = Decimal2String(ptr, g_ui32AnimSpeed); /* write speed to the buffer and update the string length */
			   ptr += n;
			   pResponse->ResponseData.token_value.len += n;
			   *ptr = '\0';
		  }

/* check if the get button count event */
           else if(pal_Memcmp(pEvent->EventData.httpTokenName.data, GET_token5,
											   pal_Strlen(GET_token5)) == 0)
		  {
        	   CLI_Write((_u8 *)"botton press checked\n");
			   n = Decimal2String(ptr, g_ui32SW1Presses);  /* write number of button press to the buffer and update the string length */
			   ptr += n;
			   pResponse->ResponseData.token_value.len += n;
			   *ptr = '\0';
		  }

/* check if the PWM start event */
           else if(pal_Memcmp(pEvent->EventData.httpTokenName.data, GET_token6,
		  		  							   pal_Strlen(GET_token6)) == 0)
		  {
        	   CLI_Write((_u8 *)"PWM start\n");
			   if(!PWMEnable)
			   {
				   pal_Memcpy(ptr, "checked", pal_Strlen("checked"));   /* return the off status to update the "LED toggle off" button */
				   ptr += 7;
				   pResponse->ResponseData.token_value.len += 7;
			   }
			   else
			   {
				   pal_Memcpy(ptr, " ", pal_Strlen(" "));                /* do not update the "LED toggle off" button */
				   ptr += 1;
				   pResponse->ResponseData.token_value.len += 1;
			   }
			   *ptr = '\0';
  		  }


/* check if the humid or temperature event  */
           else if(pal_Memcmp(pEvent->EventData.httpTokenName.data, GET_token7,
		  							   pal_Strlen(GET_token7)) == 0)
		  {
        	   getTempHumid(&humid,&temp);
			   if(!g_ui32Temperature_U)
			   {
				   n = Decimal2String(ptr, humid); /* write temperature in C to the buffer and update the string length */
			   }
			   else
			   {
				   n = Decimal2String(ptr, temp); /* write temperature in F to the buffer and update the string length */
			   }
			   ptr += n;
			   pResponse->ResponseData.token_value.len += n;
			   *ptr = '\0';
		  }

/* check if the humid event */
           else if(pal_Memcmp(pEvent->EventData.httpTokenName.data, GET_token8,
										  pal_Strlen(GET_token8)) == 0)
		 {
        	   getTempHumid(&humid,&temp);
			   if(!g_ui32Temperature_U)
			   {
				   n = Decimal2String(ptr, humid); /* write temperature in C to the buffer and update the string length */
			   }
			   else
			   {
				   n = Decimal2String(ptr, temp); /* write temperature in F to the buffer and update the string length */
			   }
			   ptr += n;
			   pResponse->ResponseData.token_value.len += n;
			   *ptr = '\0';
		 }

/* check if the temperature event */
           else if(pal_Memcmp(pEvent->EventData.httpTokenName.data, GET_token9,
														  pal_Strlen(GET_token9)) == 0)
		 {

        	   getTempHumid(&humid,&temp);
			   if(!g_ui32Temperature_U)
			   {
				   n = Decimal2String(ptr, humid); /* write temperature in C to the buffer and update the string length */
			   }
			   else
			   {
				   n = Decimal2String(ptr, temp); /* write temperature in F to the buffer and update the string length */
			   }
			   ptr += n;
			   pResponse->ResponseData.token_value.len += n;
			   *ptr = '\0';
		 }
/* check if the PWM pause */
           else if(pal_Memcmp(pEvent->EventData.httpTokenName.data, GET_token10,
		  		  							   pal_Strlen(GET_token10)) == 0)
		  {
        	   CLI_Write((_u8 *)"PWM pause\n");
			   if(!PWMEnable)
			   {
				   pal_Memcpy(ptr, "checked", pal_Strlen("checked"));   /* return the off status to update the "LED toggle off" button */
				   ptr += 7;
				   pResponse->ResponseData.token_value.len += 7;
			   }
			   else
			   {
				   pal_Memcpy(ptr, " ", pal_Strlen(" "));                /* do not update the "LED toggle off" button */
				   ptr += 1;
				   pResponse->ResponseData.token_value.len += 1;
			   }
			   *ptr = '\0';
  		  }


        }
        break;

        case SL_NETAPP_HTTPPOSTTOKENVALUE_EVENT:
        {
            _u8 led = 0;
            _u8 *ptr = pEvent->EventData.httpPostData.token_name.data;

/* Check if the post command is for original CC310 demo  */
            if(pal_Memcmp(ptr, POST_token, pal_Strlen(POST_token)) == 0)
            {
                ptr = pEvent->EventData.httpPostData.token_value.data;
                if(pal_Memcmp(ptr, "LED", 3) != 0)
                    break;

                ptr += 3;
                led = *ptr;
                ptr += 2;
                if(led == '1')
                {
                    if(pal_Memcmp(ptr, "ON", 2) == 0)
                    {
                        turnLedOn(LED1);
                    }
                    else
                    {
                        turnLedOff(LED1);
                    }
                }
                else if(led == '2')
                {
                    if(pal_Memcmp(ptr, "ON", 2) == 0)
                    {
                        turnLedOn(LED2);
                    }
                    else
                    {
                        turnLedOff(LED2);
                    }
                }
            }

/* Check if the post command is for add MCU demo: LED on  */
			if(pal_Memcmp(ptr, POST_token0, pal_Strlen(POST_token0)) == 0)
			{
				g_ui32LED_Status =1;                           /* turn the LED on */
				turnLedOn(LED1);
				if(g_ui32LED_Toggle_Status)            /* reenable LED toggling */
				{
					IoSetTimer(g_ui32AnimSpeed);

				}
			}

/* Check if the post command is for add MCU demo: LED off  */
			else if(pal_Memcmp(ptr, POST_token1, pal_Strlen(POST_token1)) == 0)
			{
				g_ui32LED_Status =0;
				turnLedOff(LED1);
				PWMEnable = 0;

				if(g_ui32LED_Toggle_Status)
				{
					IoSetTimer(0);         /*reenable LED toggling */
				}
			}

/* Check if the post command is for add MCU demo: LED toggle on  */
			else if(pal_Memcmp(ptr, POST_token2, pal_Strlen(POST_token2)) == 0)
			{
				g_ui32LED_Toggle_Status = 1;
				if(g_ui32LED_Status)
				{
					IoSetTimer(g_ui32AnimSpeed);  /* enable LED toggling  */
				}
			}

/* Check if the post command is for add MCU demo: LED toggle off  */
			else if(pal_Memcmp(ptr, POST_token3, pal_Strlen(POST_token3)) == 0)
			{
				g_ui32LED_Toggle_Status = 0;
				IoSetTimer(0);               /* disable toggling */
				if(g_ui32LED_Status)
				{
					turnLedOn(LED1);
				}
				else
				{
					turnLedOff(LED1);
				}
			}

/* Check if the post command is for add MCU demo: set LED toggling speed  */
			else if(pal_Memcmp(ptr, POST_token4, pal_Strlen(POST_token4)) == 0)
			{
				ptr = pEvent->EventData.httpPostData.token_value.data;
				ulSpeed = String2Decimal(ptr);  /* get speed value from string */
				g_ui32AnimSpeed = ulSpeed;
				if (g_ui32LED_Toggle_Status && g_ui32LED_Status)
				{
					IoSetTimer(ulSpeed);

				}
			}

/* Check if the post command is for add MCU demo: Clear button 1 count  */
			else if(pal_Memcmp(ptr, POST_token5, pal_Strlen(POST_token5)) == 0)
			{
				g_ui32SW1Presses = 0;
			}

/* Check if the post command is for add MCU demo:  start PWM  */
			else if(pal_Memcmp(ptr, POST_token6, pal_Strlen(POST_token6)) == 0)
			{
//				g_ui32SW2Presses = 0;
				PWMEnable = 1;
				if(PWMEnable)
				{
					IOEnablePWM(1);  /* enable LED toggling  */
//					delayTime =
				}
			}

/* Check if the post command is for add MCU demo: temperature in C */
			else if(pal_Memcmp(ptr, POST_token8, pal_Strlen(POST_token8)) == 0)
			{
				g_ui32Temperature_U = 0;
			}

/* Check if the post command is for add MCU demo: temperature in F */
			else if(pal_Memcmp(ptr, POST_token9, pal_Strlen(POST_token9)) == 0)
			{
				g_ui32Temperature_U = 1;
			}
			/* Check if the post command is for add MCU demo: pause PWM */
			else if(pal_Memcmp(ptr, POST_token10, pal_Strlen(POST_token10)) == 0)
			{
				PWMEnable = 0;
				IOEnablePWM(0);  /* enable LED toggling  */
			}

        }
        break;

        default:
        break;
    }
}

/*!
    \brief This function handles events for IP address acquisition via DHCP
           indication

    \param[in]      pNetAppEvent is the event passed to the handler

    \return         None

    \note

    \warning
*/
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent)
{
    if(pNetAppEvent == NULL)
        CLI_Write(" [NETAPP EVENT] NULL Pointer Error \n\r");

    switch(pNetAppEvent->Event)
    {
        case SL_NETAPP_IPV4_IPACQUIRED_EVENT:
        {
            SET_STATUS_BIT(g_Status, STATUS_BIT_IP_ACQUIRED);

            /*
             * Information about the connected AP's IP, gateway, DNS etc
             * will be available in 'SlIpV4AcquiredAsync_t' - Applications
             * can use it if required
             *
             * SlIpV4AcquiredAsync_t *pEventData = NULL;
             * pEventData = &pNetAppEvent->EventData.ipAcquiredV4;
             * <gateway_ip> = pEventData->gateway;
             *
             */
        }
        break;

        case SL_NETAPP_IP_LEASED_EVENT:
        {
            SET_STATUS_BIT(g_Status, STATUS_BIT_IP_LEASED);
        }
        break;

        default:
        {
            CLI_Write(" [NETAPP EVENT] Unexpected event \n\r");
        }
        break;
    }
}

/*!
    \brief This function handles general error events indication

    \param[in]      pDevEvent is the event passed to the handler

    \return         None
*/
void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent)
{
    /*
     * Most of the general errors are not FATAL are are to be handled
     * appropriately by the application
     */
    CLI_Write(" [GENERAL EVENT] \n\r");
}

/*!
    \brief This function handles socket events indication

    \param[in]      pSock is the event passed to the handler

    \return         None
*/
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock)
{
    /*
     * This application doesn't work with socket - Hence these
     * events are not handled here
     */
    CLI_Write(" [SOCK EVENT] Unexpected event \n\r");
}
/*
 * ASYNCHRONOUS EVENT HANDLERS -- End
 */


/*
 * Application's entry point
 */


void HttpServer(){
	 _u8   SecType = 0;
	    _i32   retVal = -1;
	    _i32   mode = ROLE_STA;
	   /*
	     * Following function configures the device to default state by cleaning
	     * the persistent settings stored in NVMEM (viz. connection profiles &
	     * policies, power policy etc)
	     *
	     * Applications may choose to skip this step if the developer is sure
	     * that the device is in its default state at start of application
	     *
	     * Note that all profiles and persistent settings that were done on the
	     * device will be lost
	     */
	    retVal = configureSimpleLinkToDefaultState();
	    if(retVal < 0)
	    {
	        if (DEVICE_NOT_IN_STATION_MODE == retVal)
	            CLI_Write(" Failed to configure the device in its default state \n\r");

	        LOOP_FOREVER();
	    }

	    CLI_Write(" Device is configured in default state \n\r");

	    /*
	     * Assumption is that the device is configured in station mode already
	     * and it is in its default state
	     */
	    mode = sl_Start(0, 0, 0);
	    if(mode < 0)
	    {
	        LOOP_FOREVER();
	    }
	    else
	    {
	        if (ROLE_AP == mode)
	        {
	            /* If the device is in AP mode, we need to wait for this
	             * event before doing anything */
	            while(!IS_IP_ACQUIRED(g_Status)) { _SlNonOsMainLoopTask(); }
	        }
	        else
	        {
	            /* Configure CC3100 to start in AP mode */
	            retVal = sl_WlanSetMode(ROLE_AP);
	            if(retVal < 0)
	                LOOP_FOREVER();
	        }
	    }

	    /* Configure AP mode without security */
	    retVal = sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_SSID,
	               pal_Strlen(SSID_AP_MODE), (_u8 *)SSID_AP_MODE);
	    if(retVal < 0)
	        LOOP_FOREVER();

	    SecType = SEC_TYPE_AP_MODE;
	    /* Configure the Security parameter in the AP mode */
	    retVal = sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_SECURITY_TYPE, 1,
	            (_u8 *)&SecType);
	    if(retVal < 0)
	        LOOP_FOREVER();

	    retVal = sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_PASSWORD, pal_Strlen(PASSWORD_AP_MODE),
	            (_u8 *)PASSWORD_AP_MODE);
	    if(retVal < 0)
	        LOOP_FOREVER();

	    /* Restart the CC3100 */
	    retVal = sl_Stop(SL_STOP_TIMEOUT);
	    if(retVal < 0)
	        LOOP_FOREVER();

	    g_Status = 0;

	    mode = sl_Start(0, 0, 0);
	    if (ROLE_AP == mode)
	    {
	        /* If the device is in AP mode, we need to wait for this event before doing anything */
	        while(!IS_IP_ACQUIRED(g_Status)) { _SlNonOsMainLoopTask(); }
	    }
	    else
	    {
	        CLI_Write(" Device couldn't come in AP mode \n\r");
	        LOOP_FOREVER();
	    }

	    CLI_Write(" \r\n Device is configured in AP mode \n\r");

	    CLI_Write(" Waiting for client to connect\n\r");
	    /* wait for client to connect */
	    while((!IS_IP_LEASED(g_Status)) || (!IS_STA_CONNECTED(g_Status))) { _SlNonOsMainLoopTask(); }

	    CLI_Write(" Client connected\n\r");

	    /* Enable the HTTP Authentication */
	    retVal = set_authentication_check(TRUE);
	    if(retVal < 0)
	        LOOP_FOREVER();

	    /* Get authentication parameters */
	    retVal = get_auth_name(g_auth_name);
	    if(retVal < 0)
	        LOOP_FOREVER();

	    retVal = get_auth_password(g_auth_password);
	    if(retVal < 0)
	        LOOP_FOREVER();

	    retVal = get_auth_realm(g_auth_realm);
	    if(retVal < 0)
	        LOOP_FOREVER();

	    CLI_Write((_u8 *)"\r\n Authentication parameters: ");
	    CLI_Write((_u8 *)"\r\n Name = ");
	    CLI_Write(g_auth_name);
	    CLI_Write((_u8 *)"\r\n Password = ");
	    CLI_Write(g_auth_password);
	    CLI_Write((_u8 *)"\r\n Realm = ");
	    CLI_Write(g_auth_realm);

	    /* Get the domain name */
//	    pal_Memset(g_domain_name, "simpleServer.net", sizeof(g_auth_name));
	    retVal = get_domain_name(g_domain_name);
	    if(retVal < 0)
	        LOOP_FOREVER();

	    CLI_Write((_u8 *)"\r\n\r\n Domain name = ");
	    CLI_Write(g_domain_name);

	    /* Get URN */
	    retVal = get_device_urn(g_device_urn);
	    if(retVal < 0)
	        LOOP_FOREVER();

	    CLI_Write((_u8 *)"\r\n Device URN = ");
	    CLI_Write(g_device_urn);
	    CLI_Write((_u8 *)"\r\n");

	    /* Process the async events from the NWP */
	    while(1)
	    {
	        Semaphore_pend(Semaphore_CC3100_Req, BIOS_WAIT_FOREVER);
	        _SlNonOsMainLoopTask();
	    }

}

/*!
    \brief Set the HTTP port

    This function can be used to change the default port (80) for HTTP request

    \param[in]      num- contains the port number to be set

    \return         None

    \note

    \warning
*/
_i32 set_port_number(_u16 num)
{
    _NetAppHttpServerGetSet_port_num_t port_num;
    _i32 status = -1;

    port_num.port_number = num;

    /*Need to restart the server in order for the new port number configuration
     *to take place */
    status = sl_NetAppStop(SL_NET_APP_HTTP_SERVER_ID);
    ASSERT_ON_ERROR(status);

    status  = sl_NetAppSet (SL_NET_APP_HTTP_SERVER_ID, NETAPP_SET_GET_HTTP_OPT_PORT_NUMBER,
                  sizeof(_NetAppHttpServerGetSet_port_num_t), (_u8 *)&port_num);
    ASSERT_ON_ERROR(status);

    status = sl_NetAppStart(SL_NET_APP_HTTP_SERVER_ID);
    ASSERT_ON_ERROR(status);

    return SUCCESS;
}

/*!
    \brief Enable/Disable the authentication check for http server,
           By default authentication is disabled.

    \param[in]      enable - false to disable and true to enable the authentication

    \return         None

    \note

    \warning
*/
static _i32 set_authentication_check (_u8 enable)
{
    _NetAppHttpServerGetSet_auth_enable_t auth_enable;
    _i32 status = -1;

    auth_enable.auth_enable = enable;
    status = sl_NetAppSet(SL_NET_APP_HTTP_SERVER_ID, NETAPP_SET_GET_HTTP_OPT_AUTH_CHECK,
                 sizeof(_NetAppHttpServerGetSet_auth_enable_t), (_u8 *)&auth_enable);
    ASSERT_ON_ERROR(status);

    return SUCCESS;
}

/*!
    \brief Get the authentication user name

    \param[in]      auth_name - Pointer to the string to store authentication
                    name

    \return         None

    \note

    \warning
*/
static _i32 get_auth_name (_u8 *auth_name)
{
    _u8 len = MAX_AUTH_NAME_LEN;
    _i32 status = -1;

    status = sl_NetAppGet(SL_NET_APP_HTTP_SERVER_ID, NETAPP_SET_GET_HTTP_OPT_AUTH_NAME,
                 &len, (_u8 *) auth_name);
    ASSERT_ON_ERROR(status);

    auth_name[len] = '\0';

    return SUCCESS;
}

/*!
    \brief Get the authentication password

    \param[in]      auth_password - Pointer to the string to store
                    authentication password

    \return         None

    \note

    \warning
*/
static _i32 get_auth_password (_u8 *auth_password)
{
    _u8 len = MAX_AUTH_PASSWORD_LEN;
    _i32 status = -1;

    status = sl_NetAppGet(SL_NET_APP_HTTP_SERVER_ID, NETAPP_SET_GET_HTTP_OPT_AUTH_PASSWORD,
                                                &len, (_u8 *) auth_password);
    ASSERT_ON_ERROR(status);

    auth_password[len] = '\0';

    return SUCCESS;
}

/*!
    \brief Get the authentication realm

    \param[in]      auth_realm - Pointer to the string to store authentication
                    realm

    \return         None

    \note

    \warning
*/
static _i32 get_auth_realm (_u8 *auth_realm)
{
    _u8 len = MAX_AUTH_REALM_LEN;
    _i32 status = -1;

    status = sl_NetAppGet(SL_NET_APP_HTTP_SERVER_ID, NETAPP_SET_GET_HTTP_OPT_AUTH_REALM,
                 &len, (_u8 *) auth_realm);
    ASSERT_ON_ERROR(status);

    auth_realm[len] = '\0';

    return SUCCESS;
}

/*!
    \brief Get the device URN

    \param[in]      device_urn - Pointer to the string to store device urn

    \return         None

    \note

    \warning
*/
static _i32 get_device_urn (_u8 *device_urn)
{
    _u8 len = MAX_DEVICE_URN_LEN;
    _i32 status = -1;

    status = sl_NetAppGet(SL_NET_APP_DEVICE_CONFIG_ID, NETAPP_SET_GET_DEV_CONF_OPT_DEVICE_URN,
                 &len, (_u8 *) device_urn);
    ASSERT_ON_ERROR(status);

    device_urn[len] = '\0';

    return SUCCESS;
}

/*!
    \brief Get the domain Name

    \param[in]      domain_name - Pointer to the string to store domain name

    \return         None

    \note

    \warning        Domain name is used only in AP mode.
*/
static _i32 get_domain_name (_u8 *domain_name)
{
    _u8 len = MAX_DOMAIN_NAME_LEN;
    _i32 status = -1;

    status = sl_NetAppGet(SL_NET_APP_DEVICE_CONFIG_ID, NETAPP_SET_GET_DEV_CONF_OPT_DOMAIN_NAME,
                 &len, (_u8 *)domain_name);
    ASSERT_ON_ERROR(status);

    domain_name[len] = '\0';

    return SUCCESS;
}

/*!
    \brief This function configure the SimpleLink device in its default state. It:
           - Sets the mode to STATION
           - Configures connection policy to Auto and AutoSmartConfig
           - Deletes all the stored profiles
           - Enables DHCP
           - Disables Scan policy
           - Sets Tx power to maximum
           - Sets power policy to normal
           - Unregisters mDNS services
           - Remove all filters

    \param[in]      none

    \return         On success, zero is returned. On error, negative is returned
*/
static _i32 configureSimpleLinkToDefaultState()
{
    SlVersionFull   ver = {0};
    _WlanRxFilterOperationCommandBuff_t  RxFilterIdMask = {0};

    _u8           val = 1;
    _u8           configOpt = 0;
    _u8           configLen = 0;
    _u8           power = 0;

    _i32          retVal = -1;
    _i32          mode = -1;

    mode = sl_Start(0, 0, 0);
    ASSERT_ON_ERROR(mode);

    /* If the device is not in station-mode, try configuring it in station-mode */
    if (ROLE_STA != mode)
    {
        if (ROLE_AP == mode)
        {
            /* If the device is in AP mode, we need to wait for this event before doing anything */
            while(!IS_IP_ACQUIRED(g_Status)) { _SlNonOsMainLoopTask(); }
        }

        /* Switch to STA role and restart */
        retVal = sl_WlanSetMode(ROLE_STA);
        ASSERT_ON_ERROR(retVal);

        retVal = sl_Stop(SL_STOP_TIMEOUT);
        ASSERT_ON_ERROR(retVal);

        retVal = sl_Start(0, 0, 0);
        ASSERT_ON_ERROR(retVal);

        /* Check if the device is in station again */
        if (ROLE_STA != retVal)
        {
            /* We don't want to proceed if the device is not coming up in station-mode */
            ASSERT_ON_ERROR(DEVICE_NOT_IN_STATION_MODE);
        }
    }

    /* Get the device's version-information */
    configOpt = SL_DEVICE_GENERAL_VERSION;
    configLen = sizeof(ver);
    retVal = sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION, &configOpt, &configLen, (_u8 *)(&ver));
    ASSERT_ON_ERROR(retVal);

    /* Set connection policy to Auto + SmartConfig (Device's default connection policy) */
    retVal = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);
    ASSERT_ON_ERROR(retVal);

    /* Remove all profiles */
    retVal = sl_WlanProfileDel(0xFF);
    ASSERT_ON_ERROR(retVal);

    /*
     * Device in station-mode. Disconnect previous connection if any
     * The function returns 0 if 'Disconnected done', negative number if already disconnected
     * Wait for 'disconnection' event if 0 is returned, Ignore other return-codes
     */
    retVal = sl_WlanDisconnect();
    if(0 == retVal)
    {
        /* Wait */
        while(IS_CONNECTED(g_Status)) { _SlNonOsMainLoopTask(); }
    }

    /* Enable DHCP client*/
    retVal = sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE,1,1,&val);
    ASSERT_ON_ERROR(retVal);

    /* Disable scan */
    configOpt = SL_SCAN_POLICY(0);
    retVal = sl_WlanPolicySet(SL_POLICY_SCAN , configOpt, NULL, 0);
    ASSERT_ON_ERROR(retVal);

    /* Set Tx power level for station mode
       Number between 0-15, as dB offset from max power - 0 will set maximum power */
    power = 0;
    retVal = sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID, WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, 1, (_u8 *)&power);
    ASSERT_ON_ERROR(retVal);

    /* Set PM policy to normal */
    retVal = sl_WlanPolicySet(SL_POLICY_PM , SL_NORMAL_POLICY, NULL, 0);
    ASSERT_ON_ERROR(retVal);

    /* Unregister mDNS services */
    retVal = sl_NetAppMDNSUnRegisterService(0, 0);
    ASSERT_ON_ERROR(retVal);

    /* Remove  all 64 filters (8*8) */
    pal_Memset(RxFilterIdMask.FilterIdMask, 0xFF, 8);
    retVal = sl_WlanRxFilterSet(SL_REMOVE_RX_FILTER, (_u8 *)&RxFilterIdMask,
                       sizeof(_WlanRxFilterOperationCommandBuff_t));
    ASSERT_ON_ERROR(retVal);

    retVal = sl_Stop(SL_STOP_TIMEOUT);
    ASSERT_ON_ERROR(retVal);

    retVal = initializeAppVariables();
    ASSERT_ON_ERROR(retVal);

    return retVal; /* Success */
}

/*!
    \brief This function initializes the application variables

    \param[in]  None

    \return     0 on success, negative error-code on error
*/
static _i32 initializeAppVariables()
{
    g_Status = 0;
    pal_Memset(g_auth_name, 0, sizeof(g_auth_name));
    pal_Memset(g_auth_password, 0, sizeof(g_auth_name));
    pal_Memset(g_auth_realm, 0, sizeof(g_auth_name));
    pal_Memset(g_domain_name, 0, sizeof(g_auth_name));
    pal_Memset(g_device_urn, 0, sizeof(g_auth_name));

    return SUCCESS;
}



void CC3100_Semaphore_Post()
{
    Semaphore_post(Semaphore_CC3100_Req);
}


void getTempHumid(int* humid,int* temp){
    _u8 ptr[64] = {0};
    int len;
    //
    // Write the command to start a humidity measurement.
    //
    SHT21Write(&g_sSHT21Inst, SHT21_CMD_MEAS_RH, g_sSHT21Inst.pui8Data, 0,
            SHT21AppCallback, &g_sSHT21Inst);

    //
    // Wait for the I2C transactions to complete before moving forward.
    //
    SHT21AppI2CWait(__FILE__, __LINE__);

    //
    // Wait 33 milliseconds before attempting to get the result. Datasheet
    // claims this can take as long as 29 milliseconds.
    //
    ROM_SysCtlDelay(g_ui32SysClock / (30 * 3));


    //
    // Get the raw data from the sensor over the I2C bus.
    //
    SHT21DataRead(&g_sSHT21Inst, SHT21AppCallback, &g_sSHT21Inst);

    //
    // Wait for the I2C transactions to complete before moving forward.
    //
    SHT21AppI2CWait(__FILE__, __LINE__);

    //
    // Get a copy of the most recent raw data in floating point format.
    //
    SHT21DataHumidityGetFloat(&g_sSHT21Inst, &fHumidity);



    //
    // Write the command to start a temperature measurement.
    //
    SHT21Write(&g_sSHT21Inst, SHT21_CMD_MEAS_T, g_sSHT21Inst.pui8Data, 0,
            SHT21AppCallback, &g_sSHT21Inst);

    //
    // Wait for the I2C transactions to complete before moving forward.
    //
    SHT21AppI2CWait(__FILE__, __LINE__);

    //
    // Wait 100 milliseconds before attempting to get the result. Datasheet
    // claims this can take as long as 85 milliseconds.
    //
    ROM_SysCtlDelay(g_ui32SysClock / (10 * 3));



    //
    // Read the conversion data from the sensor over I2C.
    //
    SHT21DataRead(&g_sSHT21Inst, SHT21AppCallback, &g_sSHT21Inst);

    //
    // Wait for the I2C transactions to complete before moving forward.
    //
    SHT21AppI2CWait(__FILE__, __LINE__);

    //
    // Get the most recent temperature result as a float in celcius.
    //
    SHT21DataTemperatureGetFloat(&g_sSHT21Inst, &fTemperature);

    //
    // Convert the floats to an integer part and fraction part for easy
    // print. Humidity is returned as 0.0 to 1.0 so multiply by 100 to get
    // percent humidity.
    //
    fHumidity *= 100.0f;
    i32IntegerPart = (int32_t) fHumidity;
    i32FractionPart = (int32_t) (fHumidity * 1000.0f);
    i32FractionPart = i32FractionPart - (i32IntegerPart * 1000);
    if(i32FractionPart < 0)
    {
        i32FractionPart *= -1;
    }

    //
    // Print the humidity value using the integers we just created.
    //
    *humid = i32IntegerPart;
    len = Decimal2String(ptr,(unsigned long)i32IntegerPart);
    ptr[len] = 0;
    CLI_Write((_u8 *)"humindiy ");
    CLI_Write((_u8 *)ptr);
    CLI_Write((_u8 *)"\n");
    int i;
    for(i = 0; i<len ;i++){
    	ptrGlobalHumid[i] = ptr[i];
    }
    //
    // Perform the conversion from float to a printable set of integers.
    //
    i32IntegerPart = (int32_t) fTemperature;
    i32FractionPart = (int32_t) (fTemperature * 1000.0f);
    i32FractionPart = i32FractionPart - (i32IntegerPart * 1000);
    if(i32FractionPart < 0)
    {
        i32FractionPart *= -1;
    }

    //
    // Print the temperature as integer and fraction parts.
    //
    *temp = i32IntegerPart;
    len = Decimal2String(ptr,(unsigned long)i32IntegerPart);
    ptr[len] = 0;
    CLI_Write((_u8 *)"temperature ");
    CLI_Write((_u8 *)ptr);
    CLI_Write((_u8 *)"\n");
    for(i = 0; i<len ;i++){
    	ptrGlobalTemp[i] = ptr[i];
    }
    //
    // Delay for one second. This is to keep sensor duty cycle
    // to about 10% as suggested in the datasheet, section 2.4.
    // This minimizes self heating effects and keeps reading more accurate.
    //
    ROM_SysCtlDelay(g_ui32SysClock / 3);


}
int main(int argc, char** argv)
{

    Task_Params tskParams;

    Task_Params_init(&tskParams);
    tskParams.stackSize = TASKSTACKSIZE;
    tskParams.stack = &tsk0Stack;
    tskParams.arg0 = 50;
    Task_construct(&tsk0Struct, (Task_FuncPtr)HttpServer, &tskParams, NULL);

    /* Obtain instance handle */
    task = Task_handle(&tsk0Struct);

    _i32   retVal = -1;

    retVal = initializeAppVariables();
    ASSERT_ON_ERROR(retVal);

    stopWDT();
    initClk();

    initLEDs();

    /* Configure command line interface */
    CLI_Configure();
    I2C8Init();
    CLI_Write(" HTTP Server ");

    Board_initPWM();
    IoInit();
    BIOS_start();

}

void I2C8Init(){


    //
    // Configure the system frequency.
    //
    g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                             SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                                             SYSCTL_CFG_VCO_480), 120000000);

    //
    // Configure the device pins for this board.
    //
    PinoutSetDevice(false, false);


    //
    // The I2C8 peripheral must be enabled before use.
    //
    //
    /* initial i2c8 and configure the pin muxing A2,A3 */
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C8);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    ROM_GPIOPinConfigure(GPIO_PA2_I2C8SCL);
    ROM_GPIOPinConfigure(GPIO_PA3_I2C8SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_2);
    ROM_GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_3);
    // Keep only some parts of the systems running while in sleep mode.
    ROM_SysCtlPeripheralClockGating(true);
    ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOE);
    ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_I2C8);

    ROM_IntMasterEnable();
    // Initialize I2C8 peripheral.Initialize the SHT21
    I2CMInit(&g_sI2CInst, I2C8_BASE, INT_I2C8, 0xff, 0xff, g_ui32SysClock);
    SHT21Init(&g_sSHT21Inst, &g_sI2CInst, SHT21_I2C_ADDRESS,
            SHT21AppCallback, &g_sSHT21Inst);

    //
    // Wait for the I2C transactions to complete before moving forward.
    //
//    SHT21AppI2CWait(__FILE__, __LINE__);

    //
    // Delay for 20 milliseconds for SHT21 reset to complete itself.
    // Datasheet says reset can take as long 15 milliseconds.
    //
    ROM_SysCtlDelay(g_ui32SysClock / (50 * 3));
}


void PinoutSetDevice(bool bEthernet, bool bUSB)
{
    //
    // Enable all the GPIO peripherals.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);

    //
    // PA0-1 are used for UART0.
    //
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // PB0-1/PD6/PL6-7 are used for USB.
    // PQ4 can be used as a power fault detect on this board but it is not
    // the hardware peripheral power fault input pin.
    //
    if(bUSB)
    {
        HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
        HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0xff;
        ROM_GPIOPinConfigure(GPIO_PD6_USB0EPEN);
        ROM_GPIOPinTypeUSBAnalog(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
        ROM_GPIOPinTypeUSBDigital(GPIO_PORTD_BASE, GPIO_PIN_6);
        ROM_GPIOPinTypeUSBAnalog(GPIO_PORTL_BASE, GPIO_PIN_6 | GPIO_PIN_7);
        ROM_GPIOPinTypeGPIOInput(GPIO_PORTQ_BASE, GPIO_PIN_4);
    }
    else
    {
        //
        // Keep the default config for most pins used by USB.
        // Add a pull down to PD6 to turn off the TPS2052 switch
        //
        ROM_GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_6);
        MAP_GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_STRENGTH_2MA,
                             GPIO_PIN_TYPE_STD_WPD);

    }

    //
    // PF0/PF4 are used for Ethernet LEDs.
    //
    if(bEthernet)
    {
        //
        // this app wants to configure for ethernet LED function.
        //
        ROM_GPIOPinConfigure(GPIO_PF0_EN0LED0);
        ROM_GPIOPinConfigure(GPIO_PF4_EN0LED1);

        GPIOPinTypeEthernetLED(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);

    }
    else
    {

        //
        // This app does not want Ethernet LED function so configure as
        // standard outputs for LED driving.
        //
        ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);

        //
        // Default the LEDs to OFF.
        //
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, 0);
        MAP_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4,
                             GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);


    }

    //
    // PJ0 and J1 are used for user buttons
    //
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    ROM_GPIOPinWrite(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0);

    //
    // PN0 and PN1 are used for USER LEDs.
    //
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    MAP_GPIOPadConfigSet(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1,
                             GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);

    //
    // Default the LEDs to OFF.
    //
    ROM_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0);
}
