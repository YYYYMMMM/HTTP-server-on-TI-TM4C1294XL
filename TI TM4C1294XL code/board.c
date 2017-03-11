/*
 * board.c - TM4C1294 launchpad configuration
 *
 * Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

#include "simplelink.h"
#include "inc/tm4c1294ncpdt.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "driverlib/ssi.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/fpu.h"
#include "driverlib/uart.h"
#include "board.h"

P_EVENT_HANDLER        pIrqEventHandler = 0;

_u8 IntIsMasked;
uint32_t g_ui32SysClock;

void CC3100_Semaphore_Post();

void initClk()
{
    /*The FPU should be enabled because some compilers will use floating-
    * point registers, even for non-floating-point code.  If the FPU is not
    * enabled this will cause a fault.  This also ensures that floating-
    * point operations could be added to this application and would work
    * correctly and use the hardware floating-point unit.  Finally, lazy
    * stacking is enabled for interrupt handlers.  This allows floating-
    * point instructions to be used within interrupt handlers, but at the
    * expense of extra stack usage. */
    FPUEnable();
    FPULazyStackingEnable();

    // Make sure the main oscillator is enabled because this is required by
     // the PHY.  The system must have a 25MHz crystal attached to the OSC
     // pins.  The SYSCTL_MOSC_HIGHFREQ parameter is used when the crystal
     // frequency is 10MHz or higher.
     //
     SysCtlMOSCConfigSet(SYSCTL_MOSC_HIGHFREQ);

     //
     // Run from the PLL at 120 MHz.
     //
     g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                              SYSCTL_OSC_MAIN |
                                              SYSCTL_USE_PLL |
                                              SYSCTL_CFG_VCO_480), 120000000);
}

void stopWDT()
{
}

int registerInterruptHandler(P_EVENT_HANDLER InterruptHdl , void* pValue)
{
    pIrqEventHandler = InterruptHdl;

    return 0;
}

void CC3100_disable()
{
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6, PIN_LOW);
}

void CC3100_enable()
{
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6, PIN_HIGH);
}

void CC3100_InterruptEnable()
{
    GPIOIntEnable(GPIO_PORTM_BASE,GPIO_PIN_3);
#ifdef SL_IF_TYPE_UART
    ROM_UARTIntEnable(UART1_BASE, UART_INT_RX);
#endif
}

void CC3100_InterruptDisable()
{
     GPIOIntDisable(GPIO_PORTM_BASE,GPIO_PIN_3);
#ifdef SL_IF_TYPE_UART
     ROM_UARTIntDisable(UART1_BASE, UART_INT_RX);
#endif
}

void MaskIntHdlr()
{
	IntIsMasked = TRUE;
}

void UnMaskIntHdlr()
{
	IntIsMasked = FALSE;
}

void Delay(unsigned long interval)
{
	SysCtlDelay( (g_ui32SysClock/(3*1000))*interval );
}

void GPIOM_intHandler()
{
    unsigned long intStatus;
    intStatus = GPIOIntStatus(GPIO_PORTM_BASE, 0);
    GPIOIntClear(GPIO_PORTM_BASE,intStatus);

    if(intStatus & GPIO_PIN_3)
    {
#ifndef SL_IF_TYPE_UART
    	if(pIrqEventHandler)
        {
            pIrqEventHandler(0);
        }
#endif
#ifdef TIRTOS
    	CC3100_Semaphore_Post();
#endif
    }
}

void UART1_intHandler()
{
	unsigned long intStatus;
	intStatus =UARTIntStatus(UART1_BASE,0);
	UARTIntClear(UART1_BASE,intStatus);

#ifdef SL_IF_TYPE_UART
	if((pIrqEventHandler != 0) && (IntIsMasked == FALSE))
	{
		pIrqEventHandler(0);
	}
#endif
}

void initLEDs()
{
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
	  GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0|GPIO_PIN_1);
	  GPIOPinWrite(GPIO_PORTN_BASE,GPIO_PIN_0, PIN_LOW);
	  GPIOPinWrite(GPIO_PORTN_BASE,GPIO_PIN_1, PIN_LOW);
}

void turnLedOn(char ledNum)
{
    switch(ledNum)
    {
      case LED1:
    	  GPIOPinWrite(GPIO_PORTN_BASE,GPIO_PIN_0, PIN_HIGH);
        break;
      case LED2:
    	  GPIOPinWrite(GPIO_PORTN_BASE,GPIO_PIN_1, PIN_HIGH);
        break;
    }
}



void turnLedOff(char ledNum)
{
    switch(ledNum)
    {
      case LED1:
    	  GPIOPinWrite(GPIO_PORTN_BASE,GPIO_PIN_0, PIN_LOW);
        break;
      case LED2:
    	  GPIOPinWrite(GPIO_PORTN_BASE,GPIO_PIN_1, PIN_LOW);
        break;
    }
}


void toggleLed(char ledNum)
{
    switch(ledNum)
    {
      case LED1:
    	  if(GPIOPinRead(GPIO_PORTN_BASE,GPIO_PIN_0))
    	  {
    		  GPIOPinWrite(GPIO_PORTN_BASE,GPIO_PIN_0, PIN_LOW);
    	  }
    	  else
    	  {
    		  GPIOPinWrite(GPIO_PORTN_BASE,GPIO_PIN_0, PIN_HIGH);
    	  }
        break;
      case LED2:
    	  if(GPIOPinRead(GPIO_PORTN_BASE,GPIO_PIN_1))
    	  {
    	     GPIOPinWrite(GPIO_PORTN_BASE,GPIO_PIN_1, PIN_LOW);
    	  }
    	  else
    	  {
    	     GPIOPinWrite(GPIO_PORTN_BASE,GPIO_PIN_0, PIN_HIGH);
    	  }
        break;
    }

}

unsigned char GetLEDStatus()
{
  unsigned char status = 0;

  if(GPIOPinRead(GPIO_PORTN_BASE,GPIO_PIN_0))
    status |= (1 << 0);
  if(GPIOPinRead(GPIO_PORTN_BASE,GPIO_PIN_1))
    status |= (1 << 1);

  return status;
}

void initAntSelGPIO()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //use PF1 and PF2
   	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2);
   	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2, PIN_HIGH);
   	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, PIN_LOW);
}

void SelAntenna(int antenna)
{
    switch(antenna)
   {
        case ANT1:
        {
          	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2, PIN_HIGH);
           	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, PIN_LOW);
        }
            break;
        case ANT2:
        {
           	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2, PIN_LOW);
           	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, PIN_HIGH);
        }
            break;
   }
}



