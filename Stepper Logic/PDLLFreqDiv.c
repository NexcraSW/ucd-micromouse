/*
 *  PDLLFreqDiv.c
 *  
 *
 *  Created by Paul De Leon on 1/12/06.
 *  Copyright 2006 ProjectDensho. All rights reserved.
 *
 */

#include "PDLLFreqDiv.h"

#ifndef AVR_STUDIO
  FAKE_AVR_DEC;  
#endif

//-----------------------------------------------------------------
// increments the sum of a PDLLFreqDiv and checks if it's time
// to make a step

 void PDLLFreqDiv_tick(PDLLFreqDiv * ifd)
 {
 	ifd->sum += ifd->freq;

	if (ifd->sum >= BASE_FREQ)
	{
      ifd->sum -= BASE_FREQ;
	  (*ifd->callBackFunc) (ifd->callBackFuncParam);
	}

 } // PDLLFreqDiv_tick()

//----------------------------------------------------------------------
// initialize the left motor, more importantly, it's function pointer

void PDLLFreqDiv_initLeftMotor(PDLLFreqDiv *inFreqDiv)
{
  inFreqDiv->freq = INIT_VELOC;
  inFreqDiv->sum = INIT_SUM;
  
  inFreqDiv->callBackFunc = PDLLPorts_stepLeftMotor;
  inFreqDiv->callBackFuncParam = inFreqDiv;

  // maybe init it's pin here too?

} // PDLLFreqDiv_initLeftMotor()

//----------------------------------------------------------------------
// initialize the right motor, more importantly, it's function pointer

void PDLLFreqDiv_initRightMotor(PDLLFreqDiv *inFreqDiv)
{
  inFreqDiv->freq = INIT_VELOC;
  inFreqDiv->sum = INIT_SUM;
  
  inFreqDiv->callBackFunc = PDLLPorts_stepRightMotor;
  inFreqDiv->callBackFuncParam = inFreqDiv;
  
    // maybe init it's pin here too?
  
} // PDLLFreqDiv_initRightMotor()
