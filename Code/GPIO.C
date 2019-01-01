/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2017 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "step.h"
#include "customtimer.h"
#include "struct.h"
#define MAIN
#include "main.h"
#include "draw.h"
#include "stdio.h"




/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/


void main (void) 
{


	InitTimer();
	GPIO1_PushPull_Mode	;
	hs.xspeed =100;
	hs.yspeed =100;
	hs.xdes =-640;
	hs.ydes =-640;
	xymoveT(&hs);
	hs.xdes =416;
	hs.ydes =384;
	xymoveT(&hs);

	hfingureInit(&fig)	;


	while(1)
	{
	selectfig();
	}
}


void hfingureInit(hfingure *hf)
{
	hf->nSeries = 0;
	hf->nCurrentCircle = 0;
	hf->nCurrentPoint = 0;
	
	hf->preinterface[0]=0;
	hf->preinterface[1]=0;
	
	hf->radius = 14;
	hf->xcenter = 8;
	hf->ycenter = -56;

	hf->preradius =14;
	hf->prexcenter =8;
	hf->preycenter =-56;
	
	hf->nfigpoint = 1;	
}






