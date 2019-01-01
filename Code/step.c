#include "N76E003.h"
#include "SFR_Macro.h"
#include "common.h"
#include "struct.h"
#define MAINSTEP
#include "step.h"


data volatile unsigned int yposition =30000;
data volatile unsigned int xposition =30000;


const unsigned char  sintable[] ={0,2,3,4,6,7,7,8};
const unsigned char  costable[] ={8,8,7,7,6,4,3,2};	

void ymove(signed int *n )
{
  unsigned char index ;
  index = yposition%32;
  switch (index>>3)
  {
 	case 0:
	pwmya =  costable[index]  ;
	pwmyb =	 sintable[index]  ;
	pwmyc = 0;
	pwmyd = 0 ;
	break;

	case 1:
	pwmya =  0;
	pwmyb =	 costable[index-8] ;
	pwmyc =  sintable[index-8] ;
	pwmyd = 0;
	break;

	case 2:
 	pwmya = 0;
	pwmyb =	0;
	pwmyc = costable[index-16];
	pwmyd = sintable[index-16] ;
	break;

	case 3:
	pwmya =  sintable[index-24] ;
	pwmyb =	 0;
	pwmyc =  0;
	pwmyd =  costable[index-24] ;
	break;
  }

   if(*n>0)
   {
	  (*n)--;
	   yposition--;
   } 
   else if(*n<0)
   {
	   (*n)++;
	   yposition++;
   }

}

void xmove(signed int *n )
{
  unsigned char index ;
  index = xposition%32;
  switch (index>>3)
  {
 	case 0:
	pwmxa =  costable[index]  ;
	pwmxb =	 sintable[index]  ;
	pwmxc = 0;
	pwmxd = 0 ;
	break;

	case 1:
	pwmxa =  0;
	pwmxb =	 costable[index-8] ;
	pwmxc =  sintable[index-8] ;
	pwmxd = 0;
	break;

	case 2:
 	pwmxa = 0;
	pwmxb =	0;
	pwmxc = costable[index-16];
	pwmxd = sintable[index-16] ;
	break;

	case 3:
	pwmxa =  sintable[index-24] ;
	pwmxb =	 0;
	pwmxc =  0;
	pwmxd =  costable[index-24] ;
	break;
  }

   if(*n > 0)
   {
	   (*n)--;
	  xposition--;
   } 
   else if(*n < 0)
   {
	   (*n)++;
	   xposition++;
   }
}

void xymoveT(hstep *hs)
{
	nxspeed =  hs->xspeed;
	nyspeed =  hs->yspeed ;
	nxcounter =0;
	nycounter =0;
	bxflag=1;
	byflag =1;

	while( hs->xdes!=0 ||  hs->ydes!=0)
	{

		if(bxflag==1)
		{
			bxflag =0;
			nxcounter=0;
			
			if( hs->xdes != 0)
			{
    		  xmove(&hs->xdes);
			}
	
	 		
		}
		
		if(byflag ==1)
		{
			byflag = 0;
			nycounter=0;
			
			if( hs->ydes  != 0)
			{
      		  ymove(& hs->ydes);

			}					

		}

	}
}

