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
bit BIT_TMP;

//------------------------------------------------------------------------------
void Timer2_Delay500us(UINT32 u32CNT)
{
    
    set_WDCLR;
    clr_T2DIV2;																	//Timer2 Clock = Fsys/4 
    clr_T2DIV1;
    set_T2DIV0;
    set_TR2;                                		//Start Timer2
    while (u32CNT != 0)
    {	    
        TL2 = LOBYTE(TIMER_DIV4_VALUE_500us);		//Find  define in "Function_define.h" "TIMER VALUE"
        TH2 = HIBYTE(TIMER_DIV4_VALUE_500us);
        while (TF2 != 1);                   		//Check Timer2 Time-Out Flag
        clr_TF2;
        u32CNT --;
    }
    clr_TR2;                                		//Stop Timer2
}
//------------------------------------------------------------------------------
