//
//      TMDX ALPHA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	DSP28_Gpio.c
//
// TITLE:	DSP28 General Purpose I/O Initialization & Support Functions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  0.55| 06 May 2002 | L.H. | EzDSP Alpha Release
//  0.56| 20 May 2002 | L.H. | No change
//  0.57| 27 May 2002 | L.H. | No change
//###########################################################################

#include "DSP28_Device.h"

//---------------------------------------------------------------------------
// InitGpio: 
//---------------------------------------------------------------------------
// This function initializes the Gpio to a known state.
//红蓝线A相，低电平ni时针
//
void InitGpio(void)
{

	EALLOW;
	GpioMuxRegs.GPAMUX.all=0x00ff; ///配置PA口的低8位为功能口,本直流电机中只使用到了PWM1和PWM2
	GpioMuxRegs.GPBMUX.all=0x00ff;///配置PB口的低8位为功能口,本直流电机中只使用到了PWM7和PWM8
	//GpioMuxRegs.GPDMUX.bit.T1CTRIP_PDPA_GPIOD0 = 0;
	//GpioMuxRegs.GPDDIR.bit.GPIOD0 = 1;
	GpioMuxRegs.GPADIR.bit.GPIOA13 = 1;
	GpioMuxRegs.GPADIR.bit.GPIOA14 = 1;
	EDIS;
//	GpioDataRegs.GPDDAT.bit.GPIOD0 = 0;
//	GpioDataRegs.GPASET.bit.GPIOA13 = 1;//step1_dir A13-high
	GpioDataRegs.GPACLEAR.bit.GPIOA13 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIOA14 = 1;//step2_dir A14-low
}	
	
//===========================================================================
// No more.
//===========================================================================
