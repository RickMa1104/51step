#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "customtimer.h"
#include "step.h"

bit BIT_TMP;
bit bxa;
bit bxb;
bit bxc;
bit bxd;

data volatile  unsigned char pwmcounter=0;



void InitTimer(void)	    //sys 1/12;
{
//	TMOD &= 0xf0;                   
//	TMOD |= 0x02;   
    TMOD = 0x22; 
	TL1 = 0;
	TH1 = 256-42;  //20us;             
	TL0 = 0;
	TH0 = (256-42 );   //20us         
 
    set_EA; 
	ET0 = 1; 
	ET1 = 1;                                             
	TR0 = 1;
	TR1 = 1;	                      	
}



void Timer0_ISR(void) interrupt 1

{
	if(pwmcounter<pwmxa)
	{
		XA = 1;
	} else
	{
	    XA = 0;
	}

 	if(pwmcounter<pwmxb)
	{
		XB = 1;
	} else
	{
	    XB = 0;
	}

	if(pwmcounter<pwmxc)
	{
		XC = 1;
	} else
	{
	    XC = 0;
	}

	if(pwmcounter<pwmxd)
	{
		XD = 1;
	} else
	{
	    XD = 0;
	}

	if(pwmcounter<pwmya)
	{
		YA = 1;
	} else
	{
	    YA = 0;
	}

 	if(pwmcounter<pwmyb)
	{
		YB = 1;
	} else
	{
	    YB = 0;
	}

	if(pwmcounter<pwmyc)
	{
		YC = 1;
	} else
	{
	    YC = 0;
	}

	if(pwmcounter<pwmyd)
	{
		YD = 1;
	} else
	{
	    YD = 0;
	}

	if(pwmcounter<8)
	{
	pwmcounter ++;
	}
	else
	{
   	pwmcounter =0;
	}

	   	TF0 = 0;
}

void Timer1_ISR(void) interrupt 3
{

	if(nxcounter<nxspeed)
	nxcounter++;
	else
	bxflag = 1;		

	if(nycounter<nyspeed)
	nycounter++;
	else
	byflag = 1;		

   	TF1 = 0;
}


//
//void Timer2_Delay500us(unsigned int u32CNT)
//{
//    
//    set_WDCLR;
//    clr_T2DIV2;				//Timer2 Clock = Fsys/4 
//    clr_T2DIV1;
//    set_T2DIV0;
//    set_TR2;                                		//Start Timer2
//    while (u32CNT != 0)
//    {	    
//        TL2 = LOBYTE(TIMER_DIV4_VALUE_100us);		//Find  define in "Function_define.h" "TIMER VALUE"
//        TH2 = HIBYTE(TIMER_DIV4_VALUE_100us);
//        while (TF2 != 1);                   		//Check Timer2 Time-Out Flag
//        clr_TF2;
//        u32CNT --;
//    }
//    clr_TR2;                                		//Stop Timer2
//}
