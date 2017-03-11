

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
//#include <ti/drivers/PWM.c>
/* Example/Board Header files */
#include "board2.h"
//*****************************************************************************
//
// io.c - I/O routines

//
//*****************************************************************************
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_pwm.h"
#include "inc/hw_types.h"
#include "inc/hw_adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/adc.h"
#include "utils/ustdlib.h"
#include "io.h"
#include "board.h"
#include "buttons.h"

#define APP_TICKS_PER_SEC                   100
#define APP_INPUT_BUF_SIZE                  1024
#define NUM_STATS                           12
volatile int delayTime = 10;
//*****************************************************************************
//
// Hardware connection for the user LED.
//
//*****************************************************************************
#define LED_PORT_BASE GPIO_PORTN_BASE
#define LED_PIN GPIO_PIN_0

//*****************************************************************************
//
// Hardware connection for the animation LED.
//
//*****************************************************************************
#define LED_ANIM_PORT_BASE GPIO_PORTN_BASE
#define LED_ANIM_PIN GPIO_PIN_1

//*****************************************************************************
//
// The system clock speed.
//
//*****************************************************************************
extern uint32_t g_ui32SysClock;

volatile unsigned long g_ui32AnimSpeed = 10;
volatile unsigned long g_ui32timer2count = 0;
volatile unsigned long g_ui32SW1Presses = 0;
volatile unsigned long g_ui32InternalTempF = 0;
volatile unsigned long g_ui32InternalTempC = 0;
volatile unsigned long g_ui32TimerIntCount = 0;
volatile unsigned long g_ui32SecondsOnTime = 0;
volatile unsigned long g_ui32LED_Status = 0;
volatile unsigned long g_ui32LED_Toggle_Status = 0;
volatile unsigned long g_ui32Temperature_U = 0;


void UpdateButtons(void);
void UpdateInternalTemp(void);
void ConfigureADC0(void);
void ConfigureTimer0(void);
void delay(int time);
void IoSetTimer(unsigned long ui32SpeedPercent);
unsigned int Decimal2String(char* pcBuf, int number);
//*****************************************************************************
//
// Set the timer used to pace the animation.  We scale the timer timeout such
// that a speed of 100% causes the timer to tick once every 20 mS (50Hz).
//
//*****************************************************************************
void
IoSetTimer(unsigned long ui32SpeedPercent)
{
    unsigned long ui32Timeout;

    //
    // Turn the timer off while we are mucking with it.
    //
    TimerDisable(TIMER2_BASE, TIMER_A);
	delayTime = ui32SpeedPercent;
	if(ui32SpeedPercent) TimerLoadSet(TIMER3_BASE, TIMER_A, ui32SpeedPercent*1000000);
    //
    // If the speed is non-zero, we reset the timeout.  If it is zero, we
    // just leave the timer disabled.
    //
    if(ui32SpeedPercent)
    {
        //
        // Set Timeout
        //
        ui32Timeout = g_ui32SysClock / 50;
        ui32Timeout = (ui32Timeout * 100 ) / ui32SpeedPercent;

        TimerLoadSet(TIMER2_BASE, TIMER_A, ui32Timeout);
        TimerEnable(TIMER2_BASE, TIMER_A);

    }
}

void IOEnablePWM(unsigned long enable){
	if(enable == 0){
	    TimerDisable(TIMER3_BASE, TIMER_A);

	    return ;
	}
    TimerEnable(TIMER3_BASE, TIMER_A);
}

//*****************************************************************************
//
// Initialize the IO used in this demo
//
//*****************************************************************************
void
IoInit(void)
{

    // Enable the peripherals used by this example.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
    //
    // Configure the timer used to pace the animation.
    //
    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
    TimerConfigure(TIMER3_BASE, TIMER_CFG_PERIODIC);
    //
    // Setup the interrupts for the timer timeouts.
    //
    IntEnable(INT_TIMER2A);
    IntEnable(INT_TIMER3A);
    TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);

    //
   	    // Configure necessary peripherals.
   	    //
   	    ConfigureTimer0();
   	    ConfigureADC0();

   	    //
   	    // Take an initial reading of the internal temperature
   	    //
   	    UpdateInternalTemp();

   	    //
   	    // Initialize the buttons
   	    //
   	    ButtonsInit();


}



//*****************************************************************************
//
// Set the speed of the animation shown on the display.  In this version, the
// speed is described as a decimal number encoded as an ASCII string.
//
//*****************************************************************************
unsigned int String2Decimal(char *pcBuf)
{
    unsigned long ui32Speed;

    //
    // Parse the passed parameter as a decimal number.
    //
    ui32Speed = 0;
    while((*pcBuf >= '0') && (*pcBuf <= '9'))
    {
        ui32Speed *= 10;
        ui32Speed += (*pcBuf - '0');
        pcBuf++;
    }

    return (ui32Speed);
}

//*****************************************************************************
//
// Set the speed of the animation shown on the display.
//
//*****************************************************************************
void
IoSetAnimationSpeed(unsigned long ui32Speed)
{
    //
    // If the number is valid, set the new speed.
    //
    if(ui32Speed <= 100)
    {
        g_ui32AnimSpeed = ui32Speed;
        IoSetTimer(g_ui32AnimSpeed);
    }
}

//*****************************************************************************
//
// Get the current animation speed as an ASCII string.
//
//*****************************************************************************
unsigned int
Decimal2String(char* pcBuf, int number)
{
    unsigned int iBufLen, i, digit;
    iBufLen = 3;
    if((number/100)== 0)
    {
    	iBufLen = 2;
    	if((number/10)==0)
    	{
    		iBufLen = 1;
    	}
    }

     pcBuf +=iBufLen;
     for(i=0;i<iBufLen;i++)
     {
        digit = number %10;
        number = number /10;
        pcBuf--;

        switch(digit)
        {
                 case 0: *pcBuf = '0'; break;
                 case 1: *pcBuf = '1'; break;
                 case 2: *pcBuf = '2'; break;
                 case 3: *pcBuf = '3'; break;
                 case 4: *pcBuf = '4'; break;
                 case 5: *pcBuf = '5'; break;
                 case 6: *pcBuf = '6'; break;
                 case 7: *pcBuf = '7'; break;
                 case 8: *pcBuf = '8'; break;
                 case 9: *pcBuf = '9'; break;

 		}
     }
     return(iBufLen);
}

//*****************************************************************************
//
// Get the current animation speed as a number.
//
//*****************************************************************************
unsigned long
IoGetAnimationSpeed(void)
{
    return(g_ui32AnimSpeed);
}


//
// The interrupt handler for the timer used to pace the animation.
//


//new name TimerIntHandlerToggle
void
AnimTimerIntHandler(void)
{
    //
    // Clear the timer interrupt.
    //
    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

    //
    // toggle LED
    //
    g_ui32timer2count++;
    if(g_ui32timer2count & 1)
    {
    	turnLedOn(LED1);
    }
    else
    {
    	turnLedOff(LED1);
    }
}



//
// The interrupt handler for the timer3A to control PWM.
//


void AnimTimerIntHandlerPWM(void)
{
    //
    // Clear the timer interrupt.
    //
    TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
//    TimerLoadSet(TIMER3_BASE, TIMER_A, 24000000);
    //
    // toggle LED
    //
//    g_ui32timer2count++;
    ///////////////////////////////////////////////////////////
    static PWM_Handle pwm1;
    static PWM_Handle pwm2 = NULL;
    static PWM_Params params;
    static uint16_t   pwmPeriod = 3000;      // Period and duty in microseconds
    static uint16_t   duty = 0;
    static uint16_t   dutyInc = 100;
    static int flag = 0;
    if(!flag){
		PWM_Params_init(&params);
		params.period = pwmPeriod;
		pwm1 = PWM_open(Board_PWM0, &params);
		if (pwm1 == NULL) {
			System_abort("Board_PWM0 did not open");
		}

		if (Board_PWM1 != Board_PWM0) {
			params.polarity = PWM_POL_ACTIVE_LOW;
			pwm2 = PWM_open(Board_PWM1, &params);
			if (pwm2 == NULL) {
				System_abort("Board_PWM1 did not open");
			}
		}
		flag = 1;
		return;
    }

    {
        PWM_setDuty(pwm1, duty);

        if (pwm2) {
            PWM_setDuty(pwm2, duty);
        }

        duty = (duty + dutyInc);
        if (duty == pwmPeriod || (!duty)) {
            dutyInc = - dutyInc;
        }

        delay(0);
    }

}

void delay(int time){
	int i;
    for(i = 0; i< 10*time; i++);

}



////////////////////////////////////////////////////////////////////

//*****************************************************************************
//
// Interrupt handler for Timer0A.
//
// This function will be called periodically on the expiration of Timer0A It
// performs periodic tasks, such as looking for input on the physical buttons,
// and reporting usage statistics to the cloud.
//
//*****************************************************************************


void
Temp_Button_Update_IntHandler(void)
{
    //
    // Clear the timer interrupt.
    //
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Keep track of the number of times this interrupt handler has been
    // called.
    //
    g_ui32TimerIntCount++;

    //
    // Poll the GPIOs for the buttons to check for press events. Update global
    // variables as necessary.
    //
    UpdateButtons();

    //
    // Once per second, perform the following operations.
    //
    if(!(g_ui32TimerIntCount % APP_TICKS_PER_SEC))
    {
        //
        // Keep track of the total seconds of on-time
        //
        g_ui32SecondsOnTime++;

        //
        // Take a reading from the internal temperature sensor.
        //
        UpdateInternalTemp();

     }

    //
    // Make sure the running tally of the number of interrupts doesn't
    // overflow.
    //
    if(g_ui32TimerIntCount == (20 * APP_TICKS_PER_SEC))
    {
        //
        // Reset the interrupt count to zero.
        //
        g_ui32TimerIntCount = 0;

    }

}


//*****************************************************************************
//
// Configures Timer 0 as a general purpose, periodic timer for handling button
// presses.
//
//*****************************************************************************
void ConfigureTimer0(void)
{
    //
    // Enable the peripherals used by this example.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    //
    // Configure the two 32-bit periodic timers.
    //
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, g_ui32SysClock / APP_TICKS_PER_SEC);

    //
    // Lower the priority of this interrupt
    //
    IntPriorityGroupingSet(4);
    IntPrioritySet(INT_TIMER0A, 0xE0);

    //
    // Setup the interrupts for the timer timeouts.
    //
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A);
}

//*****************************************************************************
//
// Enables and configures ADC0 to read the internal temperature sensor into
// sample sequencer 3.
//
//*****************************************************************************
void
ConfigureADC0(void)
{
    //
    // Enable clock to ADC0.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    //
    // Configure ADC0 Sample Sequencer 3 for processor trigger operation.
    //
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    //
    // Increase the hold time of this sample sequencer to account for the
    // temperature sensor erratum (ADC#09).
    //
    HWREG(ADC0_BASE + ADC_O_SSTSH3) = 0x4;

    //
    // Configure ADC0 sequencer 3 for a single sample of the temperature
    // sensor.
    //
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_TS | ADC_CTL_IE |
                                 ADC_CTL_END);

    //
    // Enable the sequencer.
    //
    ADCSequenceEnable(ADC0_BASE, 3);

    //
    // Clear the interrupt bit for sequencer 3 to make sure it is not set
    // before the first sample is taken.
    //
    ADCIntClear(ADC0_BASE, 3);
}

//*****************************************************************************
//
// Polls the buttons, and updates global state accordingly.
//
//*****************************************************************************
void
UpdateButtons(void)
{
    uint8_t ui8Buttons, ui8ButtonsChanged;

    //
    // Check the current debounced state of the buttons.
    //
    ui8Buttons = ButtonsPoll(&ui8ButtonsChanged,0);

    //
    // If either button has been pressed, record that status to the
    // corresponding global variable.
    //
    if(BUTTON_PRESSED(USR_SW1, ui8Buttons, ui8ButtonsChanged))
    {
        g_ui32SW1Presses++;
    }

}

//*****************************************************************************
//
// Takes a reading from the internal temperature sensor, and updates the
// corresponding global statistics.
//
//*****************************************************************************
void
UpdateInternalTemp(void)
{
    uint32_t pui32ADC0Value[1], ui32TempValueC, ui32TempValueF;

    //
    // Take a temperature reading with the ADC.
    //
    ADCProcessorTrigger(ADC0_BASE, 3);

    //
    // Wait for the ADC to finish taking the sample
    //
    while(!ADCIntStatus(ADC0_BASE, 3, false))
    {
    }

    //
    // Clear the interrupt
    //
    ADCIntClear(ADC0_BASE, 3);

    //
    // Read the analog voltage measurement.
    //
    ADCSequenceDataGet(ADC0_BASE, 3, pui32ADC0Value);

    //
    // Convert the measurement to degrees Celcius and Fahrenheit, and save to
    // the global state variables.
    //
    ui32TempValueC = ((1475 * 4096) - (2250 * pui32ADC0Value[0])) / 40960;
    g_ui32InternalTempC = ui32TempValueC;
    ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;
    g_ui32InternalTempF = ui32TempValueF;
}



