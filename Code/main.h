#include "struct.h"
#ifndef _MAIN_H_
#define _MAIN_H_

void hfingureInit(hfingure *hf)	 ;

#ifdef MAIN 
#define MAINEXTERN 
#else
#define MAINEXTERN extern
#endif
 
MAINEXTERN hstep hs;
MAINEXTERN hfingure fig;

#endif