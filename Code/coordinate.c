#include "coordinate.h"

extern const float figCircle[16][3];
void toRelative( hfingure *fig)
{
		unsigned char i ;

		for(i=0;i<fig->nfigpoint-1;i++)
		{					
					fig->arrInfo[i][0] = (-fig->figbuff[i+1][0] + fig->figbuff[i][0]);
					fig->arrInfo[i][1] = (-fig->figbuff[i+1][1] + fig->figbuff[i][1]);							
		}

		fig->arrInfo[fig->nfigpoint-1][0] = (-fig->figbuff[0][0]+fig->figbuff[fig->nfigpoint-1][0]);
		fig->arrInfo[fig->nfigpoint-1][1] = (-fig->figbuff[0][1]+fig->figbuff[fig->nfigpoint-1][1]);
		
    	radiusToSpeed(fig);
}


void DirectToRelative( hfingure *fig,const signed int src[][3],unsigned char n)
{

		unsigned char i;
		fig->nfigpoint = n;		
		for(i=0;i<n-1;i++)
		{	
			fig->arrInfo[i][0] = -src[i+1][0] + src[i][0];
			fig->arrInfo[i][1] = -src[i+1][1] + src[i][1];			
		}	

		fig->arrInfo[n-1][0] = -src[0][0] + src[n-1][0];
		fig->arrInfo[n-1][1] = -src[0][1] + src[n-1][1];		
		
		if(fig->nCurrentCircle<30)
		{
			fig->nCurrentCircle++;
		} 
		else
		{
			fig->nCurrentCircle = 0;
			fig->nSeries++;
			
		}
		
		 radiusToSpeed(fig);
}

void CircleToCoordinate( hfingure *fig,const float src[][3],unsigned char n)
{

	unsigned char i;
	for(i=0;i < n;i++)
	{	
		fig->figbuff[i][0] = src[i][0]*fig->radius + fig->xcenter;
		fig->figbuff[i][1] = src[i][1]*fig->radius + fig->ycenter;			
	}	
	fig->nfigpoint = n;	
  toRelative(fig);
	
	if(fig->nCurrentCircle<30)
	{
		fig->nCurrentCircle++;
	} 
	else
	{
		fig->nCurrentCircle = 0;
		fig->nSeries++;
	}
}



void movein(hfingure *fig)
{
					
	fig->arrInfo[0][0] = (-fig->interface[0] + fig->preinterface[0]);			
	fig->arrInfo[0][1] = (-fig->interface[1] + fig->preinterface[1]);					

}

void circlemove(hfingure *fig,const float pos[][3])
{
	
		fig->nfigpoint = 1;
	
		fig->preinterface[0] = fig->interface[0] ;
		fig->preinterface[1] = fig->interface[1] ;

		fig->interface[0] = pos[0][0]*fig->radius + fig->xcenter;
		fig->interface[1] = pos[0][1]*fig->radius + fig->ycenter;	
	
   	movein(fig);
	
		fig->nSeries++;	
}

void pointmove(hfingure *fig,const signed int pos[][3])
{
		fig->nfigpoint = 1;
	
		fig->preinterface[0] = fig->interface[0] ;
		fig->preinterface[1] = fig->interface[1] ;

		fig->interface[0] = pos[0][0];
		fig->interface[1] = pos[0][1];	

		movein(fig);	
		fig->nSeries++;
		
}


void radiusToSpeed(hfingure *fig)
{

  if(fig->radius >= 20)
  {
	fig->speedref = 9; 
  }else
  {
	fig->speedref = 9; 

  }
	
}
