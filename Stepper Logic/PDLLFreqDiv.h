/*
 *  PDLLFreqDiv.h
 *  
 *
 *  Created by Paul De Leon on 1/12/06.
 *  Copyright 2006 ProjectDensho. All rights reserved.
 *
 */

#include "PDConfig.h"

#ifndef PDLLFREQDIV_H
	#define PDLLFREQDIV_H

typedef struct PDLLFreqDiv
{
  PDLLInt freq;
  PDLLInt sum;
  
  void (*callBackFunc) (void*);
  void *callBackFuncParam; 
  
} PDLLFreqDiv;

  void PDLLFreqDiv_tick(PDLLFreqDiv*);
  
  void PDLLFreqDiv_initLeftMotor(PDLLFreqDiv*);
  void PDLLFreqDiv_initRightMotor(PDLLFreqDiv*);
  
#endif
