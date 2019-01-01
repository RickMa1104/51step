/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2016 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Nuvoton Technoledge Corp. 
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Apr/21/2016
//***********************************************************************************************************

#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"

//----------------------------------------------------------------------------------
// UART0 baud rate initial setting 
//----------------------------------------------------------------------------------
void InitialUART0_Timer1(UINT32 u32Baudrate)    //T1M = 1, SMOD = 1
{
	P06_Quasi_Mode;		
	P07_Quasi_Mode;		
	
	SCON = 0x50;     	//UART0 Mode1,REN=1,TI=1
	TMOD |= 0x20;    	//Timer1 Mode1
	
	set_SMOD;        	//UART0 Double Rate Enable
	set_T1M;
	clr_BRCK;        	//Serial port 0 baud rate clock source = Timer1
	
	
	TH1 = 256 - (1000000/u32Baudrate+1);               /*16 MHz */  	
	
	set_TR1;
	set_TI;						//For printf function must setting TI = 1
}


void Send_Data_To_UART0 (UINT8 c)
{
    TI = 0;
    SBUF = c;
    while(TI==0);
}


//----------------------------------------------------------------------------------
// UART1 baud rate initial setting 
//----------------------------------------------------------------------------------
void InitialUART1_Timer3(UINT32 u32Baudrate) //use timer3 as Baudrate generator
{
	P02_Quasi_Mode;		
	P16_Quasi_Mode;		
	
	SCON_1 = 0x50;   	//UART1 Mode1,REN_1=1,TI_1=1
	T3CON = 0x08;   	//T3PS2=0,T3PS1=0,T3PS0=0(Prescale=1), UART1 in MODE 1
	clr_BRCK;
	
	
	RH3    = HIBYTE(65536 - (1000000/u32Baudrate)-1);  		/*16 MHz */
	RL3    = LOBYTE(65536 - (1000000/u32Baudrate)-1);			/*16 MHz */
	
	
	set_TR3;         //Trigger Timer3
}



void Send_Data_To_UART1 (UINT8 c)
{
    TI_1 = 0;
    SBUF_1 = c;
    while(TI_1==0);
}



