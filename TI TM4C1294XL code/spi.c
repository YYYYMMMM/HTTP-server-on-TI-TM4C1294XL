/*
 * spi.c - TM4C1294 launchpad spi interface implementation
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

#ifndef SL_IF_TYPE_UART
#include "simplelink.h"
#include "board.h"
#include "spi.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "inc/hw_ints.h"
#include "driverlib/ssi.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

#define ASSERT_CS()         GPIOPinWrite(GPIO_PORTH_BASE,GPIO_PIN_2, PIN_LOW)
#define DEASSERT_CS()       GPIOPinWrite(GPIO_PORTH_BASE,GPIO_PIN_2, PIN_HIGH)
extern uint32_t g_ui32SysClock;


int spi_Close(Fd_t fd)
{
    /* Disable WLAN Interrupt ... */
    CC3100_InterruptDisable();

    return NONOS_RET_OK;
}

Fd_t spi_Open(char *ifName, unsigned long flags)
{
    /* Configure CS (PH2) and nHIB (PC6) lines */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6);
    GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_2);
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6, PIN_LOW);
    GPIOPinWrite(GPIO_PORTH_BASE,GPIO_PIN_2, PIN_HIGH);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    GPIOPinConfigure(GPIO_PD3_SSI2CLK);
    GPIOPinConfigure(GPIO_PD2_SSI2FSS);
    GPIOPinConfigure(GPIO_PD1_SSI2XDAT0);
    GPIOPinConfigure(GPIO_PD0_SSI2XDAT1);

    GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2
            | GPIO_PIN_3);


    /* CLOCK IS CONFIGURED AT 12 MHz */
    SSIConfigSetExpClk(SSI2_BASE, g_ui32SysClock, SSI_FRF_MOTO_MODE_0,
            SSI_MODE_MASTER, 12000000, 8);

    SSIEnable(SSI2_BASE);

    /* configure host IRQ line (pin PM3)*/
    GPIOPinTypeGPIOInput(GPIO_PORTM_BASE, GPIO_PIN_3);
    GPIOPadConfigSet(GPIO_PORTM_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA,
            GPIO_PIN_TYPE_STD_WPD);
    GPIOIntTypeSet(GPIO_PORTM_BASE, GPIO_PIN_3, GPIO_RISING_EDGE);
    GPIOIntClear(GPIO_PORTM_BASE,GPIO_PIN_3);
    GPIOIntDisable(GPIO_PORTM_BASE,GPIO_PIN_3);
    IntEnable(INT_GPIOM);
    IntMasterEnable();

    /* 1 ms delay */
    SysCtlDelay( (g_ui32SysClock/(3*1000))*50 );

    /* Enable WLAN interrupt */
    CC3100_InterruptEnable();

    return NONOS_RET_OK;
}


int spi_Write(Fd_t fd, unsigned char *pBuff, int len)
{
    int len_to_return = len;
    unsigned long ulDummy;

    ASSERT_CS();

    while(len)
    {
        while(SSIDataPutNonBlocking(SSI2_BASE, (unsigned long)*pBuff) != TRUE);
        while(SSIDataGetNonBlocking(SSI2_BASE, &ulDummy) != TRUE);
        pBuff++;
        len--;
    }
    DEASSERT_CS();

    return len_to_return;
}


int spi_Read(Fd_t fd, unsigned char *pBuff, int len)
{
    int i = 0;
    unsigned long ulBuff;

    ASSERT_CS();

    for(i=0; i< len; i++)
    {
        while(SSIDataPutNonBlocking(SSI2_BASE, 0xFF) != TRUE);
        while(SSIDataGetNonBlocking(SSI2_BASE, &ulBuff) != TRUE);
        pBuff[i] = (unsigned char)ulBuff;
    }
    DEASSERT_CS();

    return len;
}
#endif /* SL_IF_TYPE_UART */
