#include "struct.h"
#ifndef _step_h_
#define _step_h_
void ymove(signed int *n );
void xmove(signed int *n );
void xymoveT(hstep *hs);

#ifdef MAINSTEP
#define EXTERN 
#else
#define EXTERN 	extern
#endif

#define YA P10
#define YB P11
#define YC P12
#define YD P13

#define XA P14
#define XB P15
#define XC P16
#define XD P17

EXTERN data volatile unsigned char pwmxa;
EXTERN data volatile unsigned char pwmxb;
EXTERN data volatile unsigned char pwmxc;
EXTERN data volatile unsigned char pwmxd;


EXTERN data volatile unsigned char pwmya;
EXTERN data volatile unsigned char pwmyb;
EXTERN data volatile unsigned char pwmyc;
EXTERN data volatile unsigned char pwmyd;


EXTERN bit bxflag;
EXTERN bit byflag;	

EXTERN volatile unsigned char  nxspeed;
EXTERN volatile unsigned char  nyspeed;

EXTERN volatile unsigned char nxcounter;
EXTERN volatile unsigned char nycounter;
#endif