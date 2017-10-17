//
//      TMDX ALPHA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	DSP28_Sci.c
//
// TITLE:	DSP28 SCI Initialization & Support Functions.
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
// InitSPI: 
//---------------------------------------------------------------------------
// This function initializes the SPI(s) to a known state.
//
void InitSci(void)
{
	// Initialize SCI-A:

	EALLOW;
	GpioMuxRegs.GPFMUX.all = 0x0030;
	EDIS;
	/* loopback   8 bit data */
	SciaRegs.SCICCR.all = 0x07;	// 1 bit stop, disable parity, idle mode, 8 bits data 
	
	SciaRegs.SCICTL1.all = 0x03; // enable TX 
	SciaRegs.SCICTL2.all = 0x03; //
	
	SciaRegs.SCIHBAUD = 0x01;
	SciaRegs.SCILBAUD = 0x44;
	
	SciaRegs.SCICTL1.all = 0x23;
	
	PieCtrl.PIEIER9.bit.INTx1 = 1;
	PieCtrl.PIEIER9.bit.INTx2 = 1;

	//tbd...
 	

	// Initialize SCI-B:

	//tbd...
}	
	

/********************************************************************************
	name:	int SciaTx_Ready(void)
	input:	none
	output:	i	1:	ready
			0:	busy
*********************************************************************************/

int SciaTx_Ready(void)
{
	unsigned int k;
	if(SciaRegs.SCICTL2.bit.TXRDY == 1)
	{
		k = 1;
	}
	else
	{
		k = 0;
	}
	return(k);
}

/********************************************************************************
	name:	int SciaRx_Ready(void)
	input:	none
	output:	i	1:	new data
			0:	none
*********************************************************************************/

int SciaRx_Ready(void)
{
	unsigned int m;
	if(SciaRegs.SCIRXST.bit.RXRDY == 1)
	{
		m = 1;
	}
	else
	{
		m = 0;
	}
	return(m);
}




//===========================================================================
// No more.
//===========================================================================
