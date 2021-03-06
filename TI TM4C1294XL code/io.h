//*****************************************************************************
//
// io.h - Prototypes for I/O routines for the enet_io example.
//
// Copyright (c) 2009-2014 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.0.12573 of the EK-TM4C1294XL Firmware Package.
//
//*****************************************************************************

#ifndef __IO_H__
#define __IO_H__

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Exported global variables.
//
//*****************************************************************************

void IoInit(void);
extern volatile unsigned long g_ui32AnimSpeed;
extern volatile unsigned long g_ui32timer2count;
extern volatile unsigned long g_ui32SW1Presses;
extern volatile unsigned long g_ui32SW2Presses;
extern volatile unsigned long g_ui32InternalTempF;
extern volatile unsigned long g_ui32InternalTempC;
extern volatile unsigned long g_ui32TimerIntCount;
extern volatile unsigned long g_ui32SecondsOnTime;
extern volatile unsigned long g_ui32LED_Status;
extern volatile unsigned long g_ui32LED_Toggle_Status;
extern volatile unsigned long g_ui32Temperature_U;

#ifdef __cplusplus
}
#endif

#endif // __IO_H__
