#ifndef _STRUCT_H_
#define _STRUCT_H_


typedef struct{
	signed int xdes;
	signed int ydes;
	unsigned char xspeed;
	unsigned char yspeed;

}hstep;

typedef struct {
	
	volatile unsigned char nSeries;
	
	unsigned char nfigpoint;
	unsigned char nCurrentPoint;
	unsigned char nCurrentCircle;
	
	unsigned char radius;
	signed char xcenter;
	signed char ycenter;
	
	unsigned char preradius;
	signed char prexcenter;
	signed char preycenter;
	
	unsigned int  speedref;
	signed char arrInfo[20][3] ;	
	signed char figbuff[20][3];
	
	signed char interface[3];
	signed char preinterface[3];
} hfingure;


#endif



