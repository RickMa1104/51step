#define MAINCOMMON
#include "step.h"
#include "draw.h"
#include "H595.h"
#include "coordinate.h"
#include "stdlib.h"
#include "math.h"
#include "main.h"
	

			
code float figCircle[16][3] = {{-0.70711,-0.70711,1},{-0.92388,-0.38268,1},{-1,0,1},{-0.92388,0.38268,1},{-0.70711,0.70711,1},{-0.38268,0.92388,1},
{0,1,1},{0.38268,0.92388,1},{0.70711,0.70711,1},{0.92388,0.38268,1},{1,0,1},{0.92388,-0.38268,1},{0.70711,-0.70711,1},{0.38268,-0.92388,1},{0,-1,1},{-0.38268,-0.92388,1}};

code float figSquare1[4][3] = {{-0.70711,-0.70711,1},{-0.70711,0.70711,1},{0.70711,0.70711,1},{0.70711,-0.70711,1}}; //0 degree;
code float figSquare2[4][3] = {{-1,0,1},{0,1,1},{1,0,1},{0,-1,1}}; //45 degree;
void matchspeed(hfingure *hf,hstep *hs)
{
	
	if(hf->arrInfo[hf->nCurrentPoint][0]==0 || hf->arrInfo[hf->nCurrentPoint][1]==0)
	{
	hs->xspeed =hf->speedref;
	hs->yspeed =hf->speedref;									
	}

	else
	if(abs(hf->arrInfo[hf->nCurrentPoint][0]) >= abs(hf->arrInfo[hf->nCurrentPoint][1]))
	{
	hs->xspeed =hf->speedref;					
	hs->yspeed  =hf->speedref*(fabs(hf->arrInfo[hf->nCurrentPoint][0])/fabs(hf->arrInfo[hf->nCurrentPoint][1]));		
	}
	else
	{
	hs->yspeed =hf->speedref;					
	hs->xspeed =hf->speedref*(fabs(hf->arrInfo[hf->nCurrentPoint][1])/fabs(hf->arrInfo[hf->nCurrentPoint][0]));						
	}		
	
}	
	
void	drawfigure(hfingure *hf)
{

	while(hf->nCurrentPoint < hf->nfigpoint)
	{
	
		hs.xdes = hf->arrInfo[hf->nCurrentPoint][0];
		hs.ydes = hf->arrInfo[hf->nCurrentPoint][1];
		
		matchspeed(hf,&hs);
		xymoveT(&hs);				
		
		hf->nCurrentPoint++;
	}

		hf->nCurrentPoint = 0;					
		
}
void selectfig(void)
{
 	switch(fig.nSeries)
	{
		case(0):	
		fig.interface[0] = figCircle[0][0]*fig.radius + fig.xcenter;
		fig.interface[1] = figCircle[0][1]*fig.radius + fig.ycenter;					
		movein(&fig);
		fig.nSeries++;

		break;	
		
		case(1):
		CircleToCoordinate(&fig,figCircle,16);	
		break;	
		
		case(2):
		fig.xcenter += 48;		
		circlemove(&fig,figCircle);
		break;
		
		case(3):
		CircleToCoordinate(&fig,figCircle,16);				
		break;
		case(4):
		fig.ycenter += 48;		
		circlemove(&fig,figCircle);						
		break;
		
		case(5):
		CircleToCoordinate(&fig,figCircle,16);								
		break;
		
		case(6):
		fig.xcenter -= 48;		
		circlemove(&fig,figCircle);			
		break;
		
		case(7):
		CircleToCoordinate(&fig,figCircle,16);			
		break;
		
		case(8):
		fig.xcenter = 32;
		fig.ycenter = -32;		
		fig.radius  = 32;
		circlemove(&fig,figCircle);		
			
		break;
		
		case 9:
		CircleToCoordinate(&fig,figCircle,16);				
		break;
		
		case 10:
			fig.radius  = 24;
			circlemove(&fig,figCircle);		
		break;
		
		case 11:
		CircleToCoordinate(&fig,figCircle,16);						
		break;
		
		case 12:	
			fig.radius  = 16;
		  circlemove(&fig,figCircle);	
		break;
		
		case 13:
		CircleToCoordinate(&fig,figCircle,16);				
		break;
		
		case 14:
			fig.radius  = 32;
		  circlemove(&fig,figCircle);
		break;
		
		case 15:		
		CircleToCoordinate(&fig,figCircle,16);		
		break;
		
		case 16:	
		circlemove(&fig,figSquare1);		
		break;
		
		case 17:
		CircleToCoordinate(&fig,figSquare1,4);	
		break;
		
		case 18:
		circlemove(&fig,figSquare2);						
		break;
		
		case 19:
		CircleToCoordinate(&fig,figSquare2,4);				
		break;
		
		case 20:
			
		circlemove(&fig,figCircle);	
		fig.nSeries  = 9;
		break;
			
			
		default:break;
	}
	
	drawfigure(&fig);
}


