#include "N76E003.h"
#include "Common.h"
#include "SFR_Macro.h"
#include "wifi.h"
UINT8 data S0CNT=0;
 
 void SerialPort0_ISR(void) interrupt 4 
{
    if (RI==1) 
    {                                       /* if reception occur */
		clr_RI;  
		
		if(S0CNT>=23)
		{
		S0CNT =0;
		}
		wifi_queue_buf[S0CNT]  = SBUF;
		S0CNT++;

   
    }
    if(TI==1)
    {
        clr_TI;                             /* if emission occur */
    }
}
