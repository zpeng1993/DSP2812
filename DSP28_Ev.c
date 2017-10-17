//
//      TMDX ALPHA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	DSP28_Ev.c
//
// TITLE:	DSP28 Event Manager Initialization & Support Functions.
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
// InitEv: 
//---------------------------------------------------------------------------
// This function initializes to a known state.
//
/*这里的配置大家可参照课本时间管理器*/
void InitEv(void)
{

	EvaRegs.ACTR.all = 0x0006;   ///通过对比较方式控制寄存器的配置，得PWM2低有效,PWM1高有效,改变设置可使电机反转
	EvaRegs.DBTCONA.all = 0x0530;///使能死区定时器，分频25/16=1.5625M,死区时间5*0.64us=3.2us
								
	EvaRegs.COMCONA.all = 0xa600; ///比较器控制A

	EvaRegs.T1CNT = 0;			///定时器1初值设为0
	EvaRegs.T1PR = 1953;        ////定时器1周期值0.64us*N
	EvaRegs.T1CMPR = 1300;          //定时器1比较值

							
	EvaRegs.EVAIMRA.bit.T1PINT = 1;
	EvaRegs.EVAIMRB.bit.T2PINT = 1;
	EvaRegs.EVAIFRA.bit.T1PINT = 1;
	EvaRegs.EVAIFRB.bit.T2PINT = 1;

	EvaRegs.T2CNT = 0;
	EvaRegs.T2PR = 1953;            //定时器1周期值0.64us*1563=1ms
	EvaRegs.T2CMPR = 1300;

	EvaRegs.T1CON.all = 0x144E;
	EvaRegs.T1CON.bit.TENABLE = 0;
	EvaRegs.T2CON.all = 0x144E;
	EvaRegs.T2CON.bit.TENABLE = 0;
	
	EvaRegs.GPTCONA.bit.TCOMPOE = 1;
	EvaRegs.GPTCONA.bit.T1PIN = 1;
	EvaRegs.GPTCONA.bit.T2PIN = 2;
	//                     EvaRegs.T2CON.all = 0x144E;                                                                                                                                                                                                                

	EvaRegs.CMPR1=1500;//	占空比越小,转得越快

	


	



	
}	
	
//===========================================================================
// No more.
//===========================================================================
