/*********************************************************************
**	Module Name:		DC-MOTOR  				    				**
**	Author:				Cbq  			    						**
**	Version:			1.0			    							**
**	CreateDate:			2008-05-19		    						**
**	Description:		            			    				**
**	Remark:				Huozq		    							**
**	Revision History:	2008-11-10			    					**
**********************************************************************/

/*********************************************************************
**	实验目的:通过学习直流电机实验，了解PWM波如何配置，来控制电机的  **
**           转动。大家在学习时还是要在去熟悉下我们课本的事件管理器 **
**           那一章，对其寄存器有个了解，好再去看程序，就都明白了   **                                               **
**	实验说明:因为直流电机就两根线，我们取了DSP的PWM1和PWM2来进行控  **
**           制,然后经过了两级放大输出。                            **
**	实验结果:运行程序可看到电机在转动，若要改变电机转速或转向,可在  **
**           DSP28_EV.c文件里按相应注释修改，即实现PWM波来控制电机  **                                     **
**********************************************************************/


#include "DSP28_Device.h"

#define	max(x,y) ((x) > (y) ? (x) : (y))
#define min(x,y) ((x) < (y) ? (x) : (y))

Uint32 EvaTimer1InterruptTp1;
Uint16 EvaTimer2InterruptTp2;
void Delay(int16  );			//延时函数
void init_evb(void);
interrupt void eva_timer1Tp1_isr(void);
interrupt void eva_timer2Tp2_isr(void);
interrupt void evb_timer3Tp3_isr(void);
interrupt void SCIRXINTA_ISR(void);
int16 Word[200] = {7,1,8,2,8,3,9,3,10,3,11,3,12,3,13,3,7,3,6,3,5,3,4,3,3,3,2,3,1,3,//26
					12,6,11,6,10,6,9,6,8,6,7,6,6,6,5,6,4,6,//18
					4,8,5,8,6,8,7,8,8,8,9,8,10,8,11,8,12,8,12,10,12,11,13,11,//20
					12,12,12,13,12,14,12,15,12,16,//10
					11,15,10,15,9,15,8,15,7,15,6,15,5,15,4,15,//16
					4,14,4,13,4,12,4,11,5,11,6,11,7,11,8,11,9,11,10,11,11,11,//24
					0,0};//2

Uint16 i = 2;
int16 servo_flag,step1_dir,step2_dir,step1_n=100,step2_n=200,temp_x,temp_y;
unsigned int Sci_VarRx[300];
unsigned int i,Run_Flag=0;

void main(void)
{
	/*初始化系统*/
	InitSysCtrl();

	/*关中断*/
	DINT;
	IER = 0x0000;
	IFR = 0x0000;
	
	/*初始化PIE控制寄存器*/
	InitPieCtrl();
	
	/*初始化PIE矢量表*/
	InitPieVectTable();	

	#if SCIA_INT
	InitSci();
	for(i = 0; i < 300; i++)
    {
    	Sci_VarRx[i] = 0;
    }

	i = 0;

//	Send_Flag = 0;
	#endif
	
	/*初始化GPIO*/
	InitGpio();	


		

	EALLOW;
	PieVectTable.T1PINT = &eva_timer1Tp1_isr;
	PieVectTable.T2PINT = &eva_timer2Tp2_isr;
	PieVectTable.T3PINT = &evb_timer3Tp3_isr;
	PieVectTable.RXAINT = &SCIRXINTA_ISR;
	EDIS;
	
	/*初始化EV*/
	InitEv();
	init_evb();


	/*使能中断*/
	EvaTimer1InterruptTp1 = 0;
	EvaTimer2InterruptTp2 = 0;
	PieCtrl.PIEIER2.all = 0x0008;
	PieCtrl.PIEIER4.all = 0x0008;
	PieCtrl.PIEIER3.bit.INTx1 = 1;
	PieCtrl.PIEIER9.bit.INTx1 = 1;
	IER |= 0x0002;
	IER |= 0x0004;
	IER |= 0x0008; 
	IER |= 0x0100;
	

	EINT;
	ERTM;

//	for(;;)
//	{
		KickDog();	//剔除看门狗，防止对DSP进行复位
	//	EvaRegs.T1CON.bit.TENABLE = 1;//左电机逆时针
	//	EvaRegs.T2CON.bit.TENABLE = 1;//右电机不转

		#if SCIA_INT	
		/*	
		for(;;)
		{
			if(SciaRx_Ready() == 1)
			{
			Sci_VarRx[i] = SciaRegs.SCIRXBUF.all;
			i++;
			}
		}
		*/
		#endif
		#if 1
		while(Run_Flag==0)
		{
			;
		}
		while(Sci_VarRx[i] != 0)
	//	for(i=2;i<122;i+=2)
		{
			if(Sci_VarRx[i] >96) 
			{
				Sci_VarRx[i] -= 39;//10~15
			}	
		//	temp_x = Word[i]-Word[i-2];//x的差值
		//	temp_y = Word[i+1]-Word[i-1];//y的差值
			temp_x = Sci_VarRx[i]-Sci_VarRx[i-2];//x的差值
			temp_y = Sci_VarRx[i+1]-Sci_VarRx[i-1];//y的差值
			if(temp_x == 0 || temp_y == 0)
			{
			//	step1_n = (max(Word[i],Word[i-2]) - min(Word[i],Word[i-2])) * 200;
			//	step2_n = (max(Word[i+1],Word[i-1]) - min(Word[i+1],Word[i-1])) * 200;
				step1_n = (max(Sci_VarRx[i],Sci_VarRx[i-2]) - min(Sci_VarRx[i],Sci_VarRx[i-2])) * 200;
				step2_n = (max(Sci_VarRx[i+1],Sci_VarRx[i-1]) - min(Sci_VarRx[i+1],Sci_VarRx[i-1])) * 200;
			}
			else
			{
			//	step1_n = (max(Word[i],Word[i-2]) - min(Word[i],Word[i-2])) * 300;
			//	step2_n = (max(Word[i+1],Word[i-1]) - min(Word[i+1],Word[i-1])) * 300;
				step1_n = (max(Sci_VarRx[i],Sci_VarRx[i-2]) - min(Sci_VarRx[i],Sci_VarRx[i-2])) * 300;
				step2_n = (max(Sci_VarRx[i+1],Sci_VarRx[i-1]) - min(Sci_VarRx[i+1],Sci_VarRx[i-1])) * 300;
			}
			if(temp_x > 1 || temp_x < -1 || temp_y > 1 || temp_y < -1) 
			{
				EvbRegs.T3CMPR = 400;//抬起舵机
			}
			else
			{
				EvbRegs.T3CMPR = 390;//放下舵机
			}
			if(temp_x > 0)
			{
				if(temp_y > 0)//写右下,左电机逆时针转
				{
					//step1_dir = 0;
					GpioDataRegs.GPADAT.bit.GPIOA14 = 0;
					EvaRegs.T1CON.bit.TENABLE = 0;//右电机不转
					EvaRegs.T2CON.bit.TENABLE = 1;//左电机逆时针
					//step2_dir = 1;
				
				}
				else if(temp_y < 0)//右上，右电机逆时针转
				{
					
					//step2_dir = 0;
					GpioDataRegs.GPADAT.bit.GPIOA13 = 0;
					EvaRegs.T1CON.bit.TENABLE = 1;//右电机逆时针
					EvaRegs.T2CON.bit.TENABLE = 0;//左电机不转
				}
				else //右横,左右电机同时逆时针转
				{
					GpioDataRegs.GPADAT.bit.GPIOA13 = 0;
					GpioDataRegs.GPADAT.bit.GPIOA14 = 0;
					//step1_dir = 0;
					//step2_sir = 0;
					EvaRegs.T1CON.bit.TENABLE = 1;//左电机逆时针
					EvaRegs.T2CON.bit.TENABLE = 1;//右电机逆时针
				}
			}
			else if(temp_x == 0)
			{
				if(temp_y > 0)//写竖,左电机逆时针转，右电机顺时针转
				{
					//step1_dir = 0;
					//step2_dir = 1;
					GpioDataRegs.GPADAT.bit.GPIOA13 = 1;
					GpioDataRegs.GPADAT.bit.GPIOA14 = 0;
					EvaRegs.T1CON.bit.TENABLE = 1;//左电机逆时针
					EvaRegs.T2CON.bit.TENABLE = 1;//右电机顺时针
				
				}
				else//反竖，左电机顺时针转，右电机逆时针转
				{
					//step1_dir = 1;
					//step2_dir = 0;
					GpioDataRegs.GPADAT.bit.GPIOA13 = 0;
					GpioDataRegs.GPADAT.bit.GPIOA14 = 1;
					EvaRegs.T1CON.bit.TENABLE = 1;//左电机不转
					EvaRegs.T2CON.bit.TENABLE = 1;//右电机逆时针
				}
			}
			else
			{
				if(temp_y > 0)//写左下,右电机顺时针转
				{
					//step2_dir = 1;
					GpioDataRegs.GPADAT.bit.GPIOA13 = 1;
					EvaRegs.T1CON.bit.TENABLE = 1;//右电机顺时针
					EvaRegs.T2CON.bit.TENABLE = 0;//左电机不转
					//step2_dir = 1;
				
				}
				else if(temp_y < 0)//左上，左电机顺时针转
				{
					//step1_dir = 1;
					GpioDataRegs.GPADAT.bit.GPIOA14 = 1;
					EvaRegs.T1CON.bit.TENABLE = 0;//右电机不转
					EvaRegs.T2CON.bit.TENABLE = 1;//左电机顺时针转
				}
				else //左横,左右电机同时顺时针转
				{
					//step1_dir = 1;
					//step2_sir = 1;
					GpioDataRegs.GPADAT.bit.GPIOA13 = 1;
					GpioDataRegs.GPADAT.bit.GPIOA14 = 1;
					EvaRegs.T1CON.bit.TENABLE = 1;//左电机顺时针
					EvaRegs.T2CON.bit.TENABLE = 1;//右电机顺时针
				}
			
			}
			i += 2;
			Delay(max(max(step1_n,step2_n),450));

		}
		EvaRegs.T1CON.bit.TENABLE = 0;//左电机不转
		EvaRegs.T2CON.bit.TENABLE = 0;//右电机不转
		#endif

//	}

} 	

interrupt void eva_timer1Tp1_isr(void)
{
	EvaTimer1InterruptTp1++;
		
	if(EvaTimer1InterruptTp1  > max(step1_n,step2_n))
	{
	
		EvaRegs.T1CON.bit.TENABLE = 0;
		EvaTimer1InterruptTp1 = 0;
//		GpioDataRegs.GPATOGGLE.bit.GPIOA13 = 1;

	}

	EvaRegs.EVAIMRA.bit.T1PINT = 1;
	EvaRegs.EVAIFRA.all = 0x0080;
	PieCtrl.PIEACK.all = PIEACK_GROUP2;
}

interrupt void eva_timer2Tp2_isr(void)
{
	EvaTimer2InterruptTp2++;
	if(EvaTimer2InterruptTp2 > max(step1_n,step2_n)) 
	{
		EvaRegs.T2CON.bit.TENABLE = 0;
		EvaTimer2InterruptTp2 = 0;
//		GpioDataRegs.GPATOGGLE.bit.GPIOA14 = 1;
	}
	EvaRegs.EVAIMRB.bit.T2PINT = 1;
	EvaRegs.EVAIFRB.bit.T2PINT = 1;
	PieCtrl.PIEACK.all = PIEACK_GROUP3;

}

interrupt void evb_timer3Tp3_isr(void)
{
//	EvaTimer2InterruptTp2++;
//	if(EvaTimer2InterruptTp2 == 400) EvbRegs.T3CMPR = 0x4E2;
//	if(EvaTimer2InterruptTp2 == 800) EvbRegs.T3CMPR = 0x271;
//	if(EvaTimer2InterruptTp2 == 1200) EvbRegs.T3CMPR = 0x4E2;
//	if(EvaTimer2InterruptTp2 == 1600) EvbRegs.T3CMPR = 0x271;
//	if(EvaTimer2InterruptTp2 == 1800) EvbRegs.T3CMPR = 0x271;
	
	EvbRegs.EVBIMRA.bit.T3PINT = 1;
	EvbRegs.EVBIFRA.bit.T3PINT = 1;
	PieCtrl.PIEACK.all = PIEACK_GROUP4;
}

interrupt void SCIRXINTA_ISR(void)
{
	PieCtrl.PIEACK.bit.ACK9 = 1;
	if(SciaRx_Ready() == 1)
	{
		Sci_VarRx[i] = SciaRegs.SCIRXBUF.all;
		i++;
		if(i > 1) Run_Flag = 1;
	}
	EINT;

}

void init_evb()
{
	EvbRegs.T3PR = 7812;
	EvbRegs.T3CMPR = 390;
	EvbRegs.T3CNT = 0;
	EvbRegs.T3CON.all = 0x1642;//25M/64
/*
	EvbRegs.T4PR = 0x00FF;
	EvbRegs.T4CMPR = 0x0030;
	EvbRegs.T4CNT = 0;
	EvbRegs.T4CON.all = 0x1042;
*/
	EvbRegs.GPTCONB.bit.TCOMPOE = 1;
	EvbRegs.GPTCONB.bit.T3PIN = 1;
	//EvbRegs.GPTCONB.bit.T4PIN = 2;
/*
	EvbRegs.CMPR4 = 0x0C00;
	EvbRegs.CMPR5 = 0x3C00;
	EvbRegs.CMPR6 = 0xFc00;

	EvbRegs.ACTRB.all = 0x0666;
	EvbRegs.DBTCONB.all = 0x0530;

	EvbRegs.COMCONB.all = 0xA600;

*/

	EvbRegs.EVBIMRA.bit.T3PINT = 1;
	EvbRegs.EVBIFRA.bit.T3PINT = 1;
	
	
	

}

void Delay(int16  data)			//延时函数
{
	int16	i_delay,j_delay;
	for (i_delay=0;i_delay<data;i_delay++) 
	{
		for(j_delay=0;j_delay<5000;j_delay++)
		{
		;
		} 
	}	
}

//===========================================================================
// No more.
//===========================================================================
