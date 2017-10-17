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
/*��������ô�ҿɲ��տα�ʱ�������*/
void InitEv(void)
{

	EvaRegs.ACTR.all = 0x0006;   ///ͨ���ԱȽϷ�ʽ���ƼĴ��������ã���PWM2����Ч,PWM1����Ч,�ı����ÿ�ʹ�����ת
	EvaRegs.DBTCONA.all = 0x0530;///ʹ��������ʱ������Ƶ25/16=1.5625M,����ʱ��5*0.64us=3.2us
								
	EvaRegs.COMCONA.all = 0xa600; ///�Ƚ�������A

	EvaRegs.T1CNT = 0;			///��ʱ��1��ֵ��Ϊ0
	EvaRegs.T1PR = 1953;        ////��ʱ��1����ֵ0.64us*N
	EvaRegs.T1CMPR = 1300;          //��ʱ��1�Ƚ�ֵ

							
	EvaRegs.EVAIMRA.bit.T1PINT = 1;
	EvaRegs.EVAIMRB.bit.T2PINT = 1;
	EvaRegs.EVAIFRA.bit.T1PINT = 1;
	EvaRegs.EVAIFRB.bit.T2PINT = 1;

	EvaRegs.T2CNT = 0;
	EvaRegs.T2PR = 1953;            //��ʱ��1����ֵ0.64us*1563=1ms
	EvaRegs.T2CMPR = 1300;

	EvaRegs.T1CON.all = 0x144E;
	EvaRegs.T1CON.bit.TENABLE = 0;
	EvaRegs.T2CON.all = 0x144E;
	EvaRegs.T2CON.bit.TENABLE = 0;
	
	EvaRegs.GPTCONA.bit.TCOMPOE = 1;
	EvaRegs.GPTCONA.bit.T1PIN = 1;
	EvaRegs.GPTCONA.bit.T2PIN = 2;
	//                     EvaRegs.T2CON.all = 0x144E;                                                                                                                                                                                                                

	EvaRegs.CMPR1=1500;//	ռ�ձ�ԽС,ת��Խ��

	


	



	
}	
	
//===========================================================================
// No more.
//===========================================================================
