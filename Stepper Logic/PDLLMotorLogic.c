/*
 *  PDLLMotorLogic.c
 *  
 *  Copyright 2006 ProjectDensho. All rights reserved.
 *
 */

#include "PDLLMotorLogic.h"

#ifndef AVR_STUDIO
  FAKE_AVR_DEC;  
  extern PDLLInt numLeftSteps;
  extern PDLLInt numRightSteps;
#endif  

extern PDMap solnMap;
extern PDSoftMouse mainMouse;
extern PDLLMotorLogic mainLogic;
extern char errorCheckingMode;

//-------------------------------------------------------
// sets the function pointers and initial values

void PDLLMotorLogic_init(PDLLMotorLogic *inMtrLogic)
{
  PDLLMotionProfile_init(&inMtrLogic->linearMotion);
  PDLLMotionProfile_init(&inMtrLogic->angularMotion);

  inMtrLogic->linearMotion.velocity.callBackFunc = PDLLMotionProfile_linVeloc;
  inMtrLogic->linearMotion.velocity.callBackFuncParam = &inMtrLogic->linearMotion;

  inMtrLogic->linearMotion.accel.callBackFunc = PDLLMotionProfile_linAccel;
  inMtrLogic->linearMotion.accel.callBackFuncParam = &inMtrLogic->linearMotion;

  inMtrLogic->angularMotion.velocity.callBackFunc = PDLLMotionProfile_angVeloc;
  inMtrLogic->angularMotion.velocity.callBackFuncParam = &inMtrLogic->angularMotion;

  inMtrLogic->angularMotion.accel.callBackFunc = PDLLMotionProfile_angAccel;
  inMtrLogic->angularMotion.accel.callBackFuncParam = &inMtrLogic->angularMotion;
  
  PDLLFreqDiv_initLeftMotor(&inMtrLogic->leftMotor);
  PDLLFreqDiv_initRightMotor(&inMtrLogic->rightMotor);
  
  // by default go forward
  inMtrLogic->dispDir = DISP_DIR_POS;         
  PDLLMotorLogic_calcMtrFq = PDLLMotorLogic_mtrFq;

  inMtrLogic->linearMotion.reachedDest = PDLLFLAG_F;
  inMtrLogic->angularMotion.reachedDest = PDLLFLAG_F;
  
  inMtrLogic->numLinSteps = 0;
    
} // PDLLMotorLogic_init()

//-------------------------------------------------------
// semi-soft reset - resets fps, flags and even motor freqs

void PDLLMotorLogic_reset()
{
  mainLogic.linearMotion.accelCurrent = DEFAULT_ACCEL;
  mainLogic.linearMotion.accel.sum = INIT_SUM;
  mainLogic.linearMotion.veloTarget = DEFAULT_VELOC;

  mainLogic.angularMotion.accelCurrent = DEFAULT_ACCEL;
  mainLogic.angularMotion.accel.sum = INIT_SUM;
  mainLogic.angularMotion.veloTarget = DEFAULT_VELOC;

  mainLogic.linearMotion.velocity.callBackFunc = PDLLMotionProfile_linVeloc;
  mainLogic.linearMotion.accel.callBackFunc = PDLLMotionProfile_linAccel;

  mainLogic.angularMotion.velocity.callBackFunc = PDLLMotionProfile_angVeloc;    
  mainLogic.angularMotion.accel.callBackFunc = PDLLMotionProfile_angAccel;
  
  mainLogic.linearMotion.reachedDest = PDLLFLAG_F;
  mainLogic.angularMotion.reachedDest = PDLLFLAG_F;

  mainLogic.leftMotor.freq = INIT_FREQ;
  mainLogic.leftMotor.sum = INIT_SUM;
  
  mainLogic.rightMotor.freq = INIT_FREQ;
  mainLogic.rightMotor.sum = INIT_SUM;
  
} // PDLLMotorLogic_reset()

//-------------------------------------------------------
// resets function pointers and only some values - linear version

void PDLLMotorLogic_prepForLaunchLin()
{
  mainLogic.linearMotion.accelCurrent = DEFAULT_ACCEL;
  mainLogic.linearMotion.veloTarget = DEFAULT_VELOC;

  mainLogic.linearMotion.velocity.callBackFunc = PDLLMotionProfile_linVeloc;      // <--- here's what was causing way too many steps...
  mainLogic.linearMotion.accel.callBackFunc = PDLLMotionProfile_linAccel;

  mainLogic.linearMotion.reachedDest = PDLLFLAG_F;
  
  mainLogic.numLinSteps = 0;                               // reset the counter
  
} // PDLLMotorLogic_prepForLaunchLin()

//--------------------------------------------------------------------------
// resets function pointers and only some values - angular version

void PDLLMotorLogic_prepForLaunchAng()
{
  mainLogic.angularMotion.accelCurrent = DEFAULT_ACCEL;
  mainLogic.angularMotion.veloTarget = DEFAULT_VELOC;
  
  mainLogic.angularMotion.velocity.callBackFunc = PDLLMotionProfile_angVeloc;    
  mainLogic.angularMotion.accel.callBackFunc = PDLLMotionProfile_angAccel;
  
  mainLogic.angularMotion.reachedDest = PDLLFLAG_F;
    
} // PDLLMotorLogic_prepForLaunchAng()

//--------------------------------------------------------------------------
// gets the motors movin' - a different function may be used for "garage configuaritons

void PDLLMotorLogic_commenceMovt()
{
  #ifdef AVR_STUDIO
    //----- running with interrupts - mainTick() gets called in the ISR ---------

    // <should probably start the clock here...>
    
    _SEI();			              // now let the fun begin...
    while ( (mainLogic.linearMotion.reachedDest != PDLLFLAG_T) || (mainLogic.angularMotion.reachedDest != PDLLFLAG_T) ) 
	{
	  ASSERT( ((mainLogic.linearMotion.reachedDest == PDLLFLAG_F) || (mainLogic.angularMotion.reachedDest == PDLLFLAG_F) ||
	          (mainLogic.linearMotion.reachedDest == PDLLFLAG_T) || (mainLogic.angularMotion.reachedDest == PDLLFLAG_T)), ASSERT_ID_9);
	}
    _CLI();                       // done!

    PDLLMotorLogic_reset();       // cleanup function

    //--------------- testing in AVR Studio w/o interrupts ----------------------
    // while ( (mainLogic.linearMotion.reachedDest != PDLLFLAG_T) || (mainLogic.angularMotion.reachedDest != PDLLFLAG_T) ) 
    // { PDLLMotorLogic_mainTick(); }                        
  #else
    // testing via a general IDE with GCC
    while( (mainLogic.linearMotion.reachedDest != PDLLFLAG_T) || (mainLogic.angularMotion.reachedDest != PDLLFLAG_T) )
    { 
      PDLLMotorLogic_mainTick(); 
      
      if (mainLogic.numLinSteps == ONE_CELL_DISP + 2)
      {
        mainLogic.numLinSteps = 0;                               // reset the counter
        (*mainMouse.updateMouseXY) ();                           // update XY coordinates
        fireSensors(&mainMouse, &solnMap);                       // map the walls
        (*mainMouse.checkForStopConditions) ();                  // check if it's time to stop
      }
    }

    printf("\nMouse has arrived at destination!\n");
    printf("Left motor took %d steps\n", numLeftSteps);
    printf("Right motor took %d steps\n\n", numRightSteps);

    PDLLMotorLogic_reset();
    
    numLeftSteps = 0;             // reset for next round
    numRightSteps = 0;
    
  #endif
  
} // PDLLMotorLogic_commenceMovt()

//--------------------------------------------------------------------------
// this function is called everytime a tick occurs i.e. timer0 has overflowed

void PDLLMotorLogic_mainTick()
{
  // some checks before we do anything
  ASSERT((mainLogic.linearMotion.accel.callBackFunc == PDLLMotionProfile_linAccel),ASSERT_ID_22);
  ASSERT((mainLogic.angularMotion.accel.callBackFunc == PDLLMotionProfile_angAccel),ASSERT_ID_23);
  
  ASSERT(( (mainLogic.linearMotion.velocity.callBackFunc == PDLLMotionProfile_linVeloc) ||
          (mainLogic.linearMotion.velocity.callBackFunc == PDLLMotionProfile_linBgnSlwDwn) ||
          (mainLogic.linearMotion.velocity.callBackFunc == PDLLMotionProfile_linSlowDown) ||
		  (mainLogic.linearMotion.velocity.callBackFunc == PDLLMotionProfile_linFWallPull) ||
          (mainLogic.linearMotion.velocity.callBackFunc == PDLLMotionProfile_linStop)),ASSERT_ID_24 );
          
  ASSERT( ((mainLogic.angularMotion.velocity.callBackFunc == PDLLMotionProfile_angVeloc) ||
          (mainLogic.angularMotion.velocity.callBackFunc == PDLLMotionProfile_angBgnSlwDwn) ||
          (mainLogic.angularMotion.velocity.callBackFunc == PDLLMotionProfile_angSlowDown) ||
          (mainLogic.angularMotion.velocity.callBackFunc == PDLLMotionProfile_angStop)),ASSERT_ID_1 );
          
  ASSERT((mainLogic.leftMotor.callBackFunc == PDLLPorts_stepLeftMotor),ASSERT_ID_26);
  ASSERT((mainLogic.rightMotor.callBackFunc == PDLLPorts_stepRightMotor),ASSERT_ID_27);

  ASSERT(( (PDLLMotorLogic_calcMtrFq == PDLLMotorLogic_mtrFq_L) ||
          (PDLLMotorLogic_calcMtrFq == PDLLMotorLogic_mtrFq_R) ||
          (PDLLMotorLogic_calcMtrFq == PDLLMotorLogic_mtrFq)),ASSERT_ID_28);

  // linear calculations
  PDLLFreqDiv_tick(&mainLogic.linearMotion.accel);
  PDLLFreqDiv_tick(&mainLogic.linearMotion.velocity);

  // angular calculations
  PDLLFreqDiv_tick(&mainLogic.angularMotion.accel);
  PDLLFreqDiv_tick(&mainLogic.angularMotion.velocity);
  
  // calculate the freqs for each motor based on linear and angular characteristics
  PDLLMotorLogic_calcMtrFq();
  
  // check and see if either of the motors have to be reversed
  PDLLMotorLogic_adjDirForAng();
    
  // let the motors do their thing
  PDLLFreqDiv_tick(&mainLogic.leftMotor);
  PDLLFreqDiv_tick(&mainLogic.rightMotor);

} 
// PDLLMotorLogic_mainTick()

//--------------------------------------------------------------------------------------
// calculates motor frequencies for straight movement

void PDLLMotorLogic_mtrFq()
{
  int linearVelo = mainLogic.linearMotion.veloCurrent;
  
  if (mainLogic.dispDir == DISP_DIR_NEG) { linearVelo = mainLogic.linearMotion.veloCurrent * -1; }
  
  mainLogic.leftMotor.freq = linearVelo;
  mainLogic.rightMotor.freq = linearVelo; //crooked motor

} // PDLLMotorLogic_mtrFq()

//--------------------------------------------------------------------------------------
// calculates motor frequencies for left turns
// need to see what testing shows first before decision is made whether to divide by 2 or not

void PDLLMotorLogic_mtrFq_L()
{
  int linearVelo = mainLogic.linearMotion.veloCurrent;
  int angularVelo = mainLogic.angularMotion.veloCurrent;
//  int angularVelo = mainLogic.angularMotion.veloCurrent / 2;
  
  if (mainLogic.dispDir == DISP_DIR_NEG) 
  { 
    linearVelo = mainLogic.linearMotion.veloCurrent * -1; 
    angularVelo = mainLogic.angularMotion.veloCurrent * -1;
  }

  mainLogic.leftMotor.freq = linearVelo - angularVelo;
  mainLogic.rightMotor.freq = linearVelo + angularVelo;
  
} // PDLLMotorLogic_mtrFq_L()

//--------------------------------------------------------------------------------------
// calculates motor frequencies for right turns

void PDLLMotorLogic_mtrFq_R()
{
  int linearVelo = mainLogic.linearMotion.veloCurrent;
  int angularVelo = mainLogic.angularMotion.veloCurrent;
//  int angularVelo = mainLogic.angularMotion.veloCurrent / 2;
  
  if (mainLogic.dispDir == DISP_DIR_NEG) 
  { 
    linearVelo = mainLogic.linearMotion.veloCurrent * -1; 
    angularVelo = mainLogic.angularMotion.veloCurrent * -1;
  }

  mainLogic.leftMotor.freq = linearVelo + angularVelo;
  mainLogic.rightMotor.freq = linearVelo - angularVelo;
 
} // PDLLMotorLogic_mtrFq_R()

//--------------------------------------------------------------------------------------
// if needed, toggle the direction pins for angular displacement

void PDLLMotorLogic_adjDirForAng()
{
   if (mainLogic.leftMotor.freq < 0)            // left motor has committed to the reverse direction
   { 
     mainLogic.leftMotor.freq *= -1;
     PDLLPorts_setLDirPins(DISP_DIR_NEG);
   }
   else { PDLLPorts_setLDirPins(DISP_DIR_POS); }
  
   if (mainLogic.rightMotor.freq < 0)           // right motor has committed to the reverse direction
   { 
     mainLogic.rightMotor.freq *= -1; 
     PDLLPorts_setRDirPins(DISP_DIR_NEG);
   }
   else { PDLLPorts_setRDirPins(DISP_DIR_POS); }
         
} // PDLLMotorLogic_adjDirForAng()

//--------------------------------------------------------------------------------------

void PDLLMotorLogic_moveForward(PDLLInt numSteps)
{
  PDLLPorts_setLDirPins(DISP_DIR_POS);
  PDLLPorts_setRDirPins(DISP_DIR_POS);
        
  PDLLMotorLogic_calcMtrFq = PDLLMotorLogic_mtrFq;
  
  // this was the only cleanest way i could think of making up for a missing step
  if (numSteps != 0) { numSteps++; }
  
  mainLogic.dispDir = DISP_DIR_POS;
  mainLogic.linearMotion.dispTarget += numSteps;
  mainLogic.linearMotion.dispRem += numSteps;

  PDLLMotorLogic_prepForLaunchLin();

} // PDLLMotorLogic_moveForward()

//--------------------------------------------------------------------------------------

void PDLLMotorLogic_moveBack(PDLLInt numSteps)
{
  if (mainLogic.linearMotion.dispRem == NO_DISP)
  {
    PDLLPorts_setLDirPins(DISP_DIR_NEG);
    PDLLPorts_setRDirPins(DISP_DIR_NEG);
  
    PDLLMotorLogic_calcMtrFq = PDLLMotorLogic_mtrFq;

    // this was the only cleanest way i could think of making up for a missing step
    if (numSteps != 0) { numSteps++; }
  
     mainLogic.dispDir = DISP_DIR_NEG;
     mainLogic.linearMotion.dispTarget += numSteps;
     mainLogic.linearMotion.dispRem += numSteps;

     PDLLMotorLogic_prepForLaunchLin();
  }

} // PDLLMotorLogic_moveBack()

//--------------------------------------------------------------------------------------

void PDLLMotorLogic_turnLeft(PDLLInt numSteps)
{
  // mainTick() does direction pin adjustments for angular movement

  mainLogic.dispDir = DISP_DIR_POS;  // angular movement is for forward motion by default    
  PDLLMotorLogic_calcMtrFq = PDLLMotorLogic_mtrFq_L; 
  
  // this was the only cleanest way i could think of making up for a missing step
  if (numSteps != 0) { numSteps++; }

  mainLogic.angularMotion.dispTarget += numSteps;
  mainLogic.angularMotion.dispRem += numSteps;

  PDLLMotorLogic_prepForLaunchAng();

} // PDLLMotorLogic_turnLeft()

//--------------------------------------------------------------------------------------
void PDLLMotorLogic_turnRight(PDLLInt numSteps)
{
  // mainTick() does direction pin adjustments for angular movement
    
  mainLogic.dispDir = DISP_DIR_POS;  // angular movement is for forward motion by default     
  PDLLMotorLogic_calcMtrFq = PDLLMotorLogic_mtrFq_R; 
  
  // this was the only cleanest way i could think of making up for a missing step
  if (numSteps != 0) { numSteps++; }

  mainLogic.angularMotion.dispTarget += numSteps;
  mainLogic.angularMotion.dispRem += numSteps;
  
  PDLLMotorLogic_prepForLaunchAng();

} // PDLLMotorLogic_turnRight()

//-------------------------------------------------------------------------------------------
// our ISR for timer 0's overflow - fires mainTick()

#ifdef AVR_STUDIO

SIGNAL(SIG_OVERFLOW0)
{
  TIFR |= 1 << TOV0;          
  
  if( (mainLogic.linearMotion.reachedDest != PDLLFLAG_T) || (mainLogic.angularMotion.reachedDest != PDLLFLAG_T) ) 
  { 
    PDLLMotorLogic_mainTick(); 

	MOTOR_PORT_L &= ~(PIN_HIGH << MOTOR_PIN_STEP_L);         // 3977 needs a one ms hold time
	MOTOR_PORT_R &= ~(PIN_HIGH << MOTOR_PIN_STEP_R);         // 3977 needs a one ms hold time
  }

  // moved about one cell, time to map walls and check floodfill values/incoming front walls

  if ( (mainLogic.numLinSteps == ONE_CELL_DISP + 2) || 
       ( (mainMouse.frontWallFound == NO_WALL) && (checkFrontWall() == WALL_FOUND) ) )
  {
    // can't have it enter this if statement again if this is the case...
    if (checkFrontWall() == WALL_FOUND) { mainMouse.frontWallFound = WALL_FOUND; }

    mainLogic.numLinSteps = 0;                               // reset the counter
    (*mainMouse.updateMouseXY) ();                           // update XY coordinates
    fireSensors(&mainMouse, &mainMouse.internalMap);         // map the walls
   
    // gotta see if were dealing with ribs or not
    if ( (PDLLcheckLeftRib() != WALL_FOUND) || (PDLLcheckRightRib() != WALL_FOUND) )
    { errorCheckingMode = NORM_ERR_CHECKING; }
    else
    { errorCheckingMode = RIB_ERR_CHECKING; }
    
    (*mainMouse.checkForStopConditions) ();                  // check if it's time to stop
  }

} // SIGNAL()

#endif
