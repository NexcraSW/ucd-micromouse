/*
 *  PDLLMotionProfile.c
 *  
 *  Copyright 2006 ProjectDensho. All rights reserved.
 *
 */

#include "PDLLMotionProfile.h"

#ifndef AVR_STUDIO
  FAKE_AVR_DEC;  
#endif

extern PDLLMotorLogic mainLogic;

//-------------------------------------------------------
// decides which functions are to be called and init's

void PDLLMotionProfile_init(PDLLMotionProfile *inMtnProfile)
{
  inMtnProfile->velocity.freq = INIT_VELOC;
  inMtnProfile->velocity.sum = INIT_SUM;
  
  inMtnProfile->accel.freq = FREQ_ACCEL;
  inMtnProfile->accel.sum = INIT_SUM;
  
  inMtnProfile->dispTarget = INIT_DISP;
  inMtnProfile->dispRem = INIT_DISP;
  
  inMtnProfile->veloTarget = DEFAULT_VELOC;
  inMtnProfile->veloCurrent = INIT_VELOC;
  
  inMtnProfile->accelCurrent = DEFAULT_ACCEL;
  inMtnProfile->accelMax = MAX_ACCEL;
  
  inMtnProfile->vel_sign = POS;
  inMtnProfile->acc_sign = POS;
  
  inMtnProfile->reachedDest = PDLLFLAG_F;
  
} // PDLLMotionProfile_init()

//------------------------------------------------------------
// adjusts the frequency of the freqDiv of velocity by 
// increasing (acceleration) or decreasing (deceleration)

void PDLLMotionProfile_linAccel(void *inMtnProf)
{
  ASSERT((inMtnProf != NULL), ASSERT_ID_2);
  
  //This statement helps fix a bug that occurs when 0 is passed in as an initial displacement  
  if ( ((PDLLMotionProfile *) inMtnProf)->dispRem == DEST_REACHED) 
  { 
    PDLLMotionProfile_linStop(inMtnProf); 
    return;
  }       
     
  // decelerate
  if ( ((PDLLMotionProfile *) inMtnProf)->veloCurrent > ((PDLLMotionProfile *) inMtnProf)->veloTarget )
  {
    ((PDLLMotionProfile *) inMtnProf)->veloCurrent -= DEFAULT_ACCEL;

    // shaved off too much - switch to constant velocity
    if ( ((PDLLMotionProfile *) inMtnProf)->veloCurrent < ((PDLLMotionProfile *) inMtnProf)->veloTarget )
    { ((PDLLMotionProfile *) inMtnProf)->veloCurrent = ((PDLLMotionProfile *) inMtnProf)->veloTarget; }
  }
  // accelerate
  else if ( ((PDLLMotionProfile *) inMtnProf)->veloCurrent < ((PDLLMotionProfile *) inMtnProf)->veloTarget )
  {
    ((PDLLMotionProfile *) inMtnProf)->veloCurrent += DEFAULT_ACCEL;

    // added too much - switch to constant velocity
    if ( ((PDLLMotionProfile *) inMtnProf)->veloCurrent > ((PDLLMotionProfile *) inMtnProf)->veloTarget )
    { ((PDLLMotionProfile *) inMtnProf)->veloCurrent = ((PDLLMotionProfile *) inMtnProf)->veloTarget; }

  }
  
  ASSERT( (( ((PDLLMotionProfile *) inMtnProf )->veloCurrent >= MIN_VELOC) &&
          ( ((PDLLMotionProfile *) inMtnProf )->veloCurrent <= MAX_VELOC)), ASSERT_ID_3 ); 
  
  // here's where the actual adjustment takes place
  ((PDLLMotionProfile *) inMtnProf)->velocity.freq = ((PDLLMotionProfile *) inMtnProf)->veloCurrent;
  
  // printf("===linAccel!===\n"); // db
    
} // PDLLMotionProfile_linAccel()

//--------------------------------------------------------------------------
// checks and updates the remaining distance and decrements
// the remaining distance for the linear velocity

void PDLLMotionProfile_linVeloc(void *inMtnProf)
{
  PDLLInt distanceAboutToCover = 0;
  
  ASSERT((inMtnProf != NULL),ASSERT_ID_4);

  mainLogic.numLinSteps++; // db
  
  #ifdef AVR_STUDIO
  if (PDLLreadSensor(CENTER_SENSOR) >= APPX_DIST_TO_FRONT_WALL)
  {
	((PDLLMotionProfile *) inMtnProf)->velocity.callBackFunc = PDLLMotionProfile_linFWallPull; 
    ((PDLLMotionProfile *) inMtnProf)->dispRem = APPX_DISP_TO_FRONT_WALL;        // stops short of the wall
	((PDLLMotionProfile *) inMtnProf)->dispTarget = APPX_DISP_TO_FRONT_WALL;
  }
  #endif

  // if initial displacement != 0, then a step will be taken
  if ( ((PDLLMotionProfile *) inMtnProf)->dispRem != DEST_REACHED)
  { ((PDLLMotionProfile *) inMtnProf)->dispRem--; }
  
  ASSERT( (( ((PDLLMotionProfile *) inMtnProf)->dispRem >= MIN_DISP) &&
          ( ((PDLLMotionProfile *) inMtnProf)->dispRem <= MAX_DISP)), ASSERT_ID_5 );
  
   distanceAboutToCover = ( ((PDLLMotionProfile *) inMtnProf)->veloCurrent * ((PDLLMotionProfile *) inMtnProf)->veloCurrent) /
                         (2 * MAX_ACCEL);
                         
  // arrived at destination
  if ( ((PDLLMotionProfile *) inMtnProf)->dispRem == DEST_REACHED)
  {    
    // next time velocity is called, everything will be zero'd out
    // we need the delay because the motor hasn't taken the actual step yet
    ((PDLLMotionProfile *) inMtnProf)->velocity.callBackFunc = PDLLMotionProfile_linStop;
  }
  // too close to destination, it's time to slow down --NOT OPTIMIZED--
  else if ( (distanceAboutToCover > ((PDLLMotionProfile *) inMtnProf)->dispTarget) ||
            (distanceAboutToCover >= ((PDLLMotionProfile *) inMtnProf)->dispRem) ) 
  {    
    // next time velocity is called, the motor will be slowing down
    // we need the delay because the motor hasn't taken the actual step yet    
    ((PDLLMotionProfile *) inMtnProf)->velocity.callBackFunc = PDLLMotionProfile_linBgnSlwDwn; 
  }
 
  /*
  #ifndef AVR_STUDIO
  printf("linVeloc!\n"); // db
  printf("===rem linear displacement = %d===\n",  ((PDLLMotionProfile *) inMtnProf)->dispRem); // db
  printf("===current linear velocity = %d===\n", ((PDLLMotionProfile *) inMtnProf)->veloCurrent); // db
  printf("===current linear accel = %d===\n", ((PDLLMotionProfile *) inMtnProf)->accelCurrent); // db
  #endif
  */
  
} // PDLLMotionProfile_linVeloc()

//--------------------------------------------------------------------------
// begin the slowdown sequence of the stepper motors

void PDLLMotionProfile_linBgnSlwDwn(void *inMtnProf)
{
  ASSERT((inMtnProf != NULL),ASSERT_ID_6);
  
  // undo what the incorrect velocity did, also takes into account next motor tick
  mainLogic.leftMotor.sum = mainLogic.angularMotion.velocity.sum + MIN_SUM;   
  mainLogic.rightMotor.sum = mainLogic.angularMotion.velocity.sum + MIN_SUM;
  
  ASSERT((mainLogic.leftMotor.sum >= MIN_SUM),ASSERT_ID_7); 
  ASSERT((mainLogic.rightMotor.sum >= MIN_SUM),ASSERT_ID_8);
  
  ((PDLLMotionProfile *) inMtnProf)->velocity.callBackFunc = PDLLMotionProfile_linSlowDown;
  PDLLMotionProfile_linSlowDown(inMtnProf);

} // PDLLMotionProfile_linBgnSlwDwn()

//--------------------------------------------------------------------------
// slowdown sequence of the stepper motors

void PDLLMotionProfile_linSlowDown(void *inMtnProf)
{
  ASSERT((inMtnProf != NULL),ASSERT_ID_9);
 
  mainLogic.numLinSteps++; // db
 
  // if initial displacement != 0, then a step will be taken
  if ( ((PDLLMotionProfile *) inMtnProf)->dispRem != DEST_REACHED)   
    { ((PDLLMotionProfile *) inMtnProf)->dispRem--; }
 
   ASSERT(( ( ((PDLLMotionProfile *) inMtnProf)->dispRem >= MIN_DISP) &&
          ( ((PDLLMotionProfile *) inMtnProf)->dispRem <= MAX_DISP)),ASSERT_ID_10 );

  if ( ((PDLLMotionProfile *) inMtnProf)->dispRem == DEST_REACHED)
  {    
    // next time velocity is called, everything will be zero'd out
    // we need the delay because the motor hasn't taken the actual step yet
    ((PDLLMotionProfile *) inMtnProf)->velocity.callBackFunc = PDLLMotionProfile_linStop;
  }

  ((PDLLMotionProfile *) inMtnProf)->accelCurrent = NO_ACCEL;    // don't know if this should be here...
  
  // velocity is proportional to the distance remaining
  ((PDLLMotionProfile *) inMtnProf)->veloTarget = ((PDLLMotionProfile *) inMtnProf)->dispRem;

  // don't wanna go TOO slow now...
  if ( ((PDLLMotionProfile *) inMtnProf)->veloTarget < SLOW_DOWN_VELOC )
  { ((PDLLMotionProfile *) inMtnProf)->veloTarget = SLOW_DOWN_VELOC; }
  
  /*
  #ifndef AVR_STUDIO
  printf("linSlowDown!\n"); // db
  printf("===rem linear displacement = %d===\n",  ((PDLLMotionProfile *) inMtnProf)->dispRem); // db
  printf("===current linear velocity = %d===\n", ((PDLLMotionProfile *) inMtnProf)->veloCurrent); // db  
  printf("===current linear accel = %d===\n", ((PDLLMotionProfile *) inMtnProf)->accelCurrent); // db  
  #endif  
  */

} // PDLLMotionProfile_linSlowDown()

//--------------------------------------------------------------------------
// pulls to the front wall

void PDLLMotionProfile_linFWallPull(void *inMtnProf)
{
  // PDInt currentX = mainMouse.currentXY.x;
  // PDInt currentY = mainMouse.currentXY.y; 

  // ASSERT( ((currentX >= X_MIN) && (currentX < X_MAX)),ASSERT_ID_19 );
   //ASSERT( ((currentY >= Y_MIN) && (currentY < Y_MAX)),ASSERT_ID_20 );

   // pulling to the beginning of the next cell...
   if ( ((PDLLMotionProfile *) inMtnProf)->dispRem != DEST_REACHED)   
   { ((PDLLMotionProfile *) inMtnProf)->dispRem--; }

   // velocity is proportional to the distance remaining
   ((PDLLMotionProfile *) inMtnProf)->veloTarget = (((PDLLMotionProfile *) inMtnProf)->dispRem * 4);

   // don't wanna go TOO slow now...
   if ( ((PDLLMotionProfile *) inMtnProf)->veloTarget < SLOW_DOWN_VELOC)
   { ((PDLLMotionProfile *) inMtnProf)->veloTarget = SLOW_DOWN_VELOC * 4; }

   // now pull to the wall till you hit it..
   if ( ((PDLLMotionProfile *) inMtnProf)->dispRem == DEST_REACHED)   
   {
     ((PDLLMotionProfile *) inMtnProf)->accelCurrent = NO_ACCEL;

     if (PDLLreadSensor(CENTER_SENSOR) < DIST_TO_FRONT_WALL)
     { ((PDLLMotionProfile *) inMtnProf)->dispRem++; }
	 else
	 {  
	   alignSkew();
	   ((PDLLMotionProfile *) inMtnProf)->velocity.callBackFunc = PDLLMotionProfile_linStop;   
	 }
   }

} // PDLLMotionProfile_linFWallPull()

//--------------------------------------------------------------------------
// zero everything out so that the motors stop stepping

void PDLLMotionProfile_linStop(void *inMtnProf)
{
  //LED_PORT = 0x00; // db
  ASSERT((inMtnProf != NULL),ASSERT_ID_11);
  
  ((PDLLMotionProfile *) inMtnProf)->accelCurrent = NO_ACCEL;
  
  ((PDLLMotionProfile *) inMtnProf)->veloTarget = STOPPED_VELOC;  
  ((PDLLMotionProfile *) inMtnProf)->veloCurrent = STOPPED_VELOC;
  ((PDLLMotionProfile *) inMtnProf)->velocity.freq = STOPPED_VELOC;
  ((PDLLMotionProfile *) inMtnProf)->velocity.sum = RESET_SUM;
  
  ((PDLLMotionProfile *) inMtnProf)->dispTarget = DEST_REACHED;
  ((PDLLMotionProfile *) inMtnProf)->dispRem = DEST_REACHED;

  // how about resetting sum's here too?

  ((PDLLMotionProfile *) inMtnProf)->reachedDest = PDLLFLAG_T;                 // breaks out of outer motor execution loop
  
  /*
  #ifndef AVR_STUDIO
  printf("linStop!\n"); // db
  printf("===rem linear displacement = %d===\n",  ((PDLLMotionProfile *) inMtnProf)->dispRem); // db
  printf("===current linear velocity = %d===\n", ((PDLLMotionProfile *) inMtnProf)->veloCurrent); // db  
  printf("===current linear accel = %d===\n", ((PDLLMotionProfile *) inMtnProf)->accelCurrent); // db
  #endif
  */

// maybe the function pointers should be reset here?

} // PDLLMotionProfile_linStop()

//--------------------------------------------------------------------------

void PDLLMotionProfile_angAccel(void *inMtnProf)
{
   ASSERT((inMtnProf != NULL), ASSERT_ID_12);
   
   //This statement helps fix a bug that occurs when 0 is passed in as an initial displacement  
   if ( ((PDLLMotionProfile *) inMtnProf)->dispRem == DEST_REACHED)
     { 
       PDLLMotionProfile_angStop(inMtnProf); 
       return;
     }       
  
  // decelerate
  if ( ((PDLLMotionProfile *) inMtnProf)->veloCurrent > ((PDLLMotionProfile *) inMtnProf)->veloTarget )
  {
    ((PDLLMotionProfile *) inMtnProf)->veloCurrent -= DEFAULT_ACCEL;
    
    // shaved off too much - switch to constant velocity
    if ( ((PDLLMotionProfile *) inMtnProf)->veloCurrent < ((PDLLMotionProfile *) inMtnProf)->veloTarget )
    { ((PDLLMotionProfile *) inMtnProf)->veloCurrent = ((PDLLMotionProfile *) inMtnProf)->veloTarget; }
  }
  // accelerate
  else if ( ((PDLLMotionProfile *) inMtnProf)->veloCurrent < ((PDLLMotionProfile *) inMtnProf)->veloTarget )
  { 
    ((PDLLMotionProfile *) inMtnProf)->veloCurrent += DEFAULT_ACCEL;
  
    // added too much - switch to constant velocity
    if ( ((PDLLMotionProfile *) inMtnProf)->veloCurrent > ((PDLLMotionProfile *) inMtnProf)->veloTarget )
    { ((PDLLMotionProfile *) inMtnProf)->veloCurrent = ((PDLLMotionProfile *) inMtnProf)->veloTarget; }
  }
  
  ASSERT( (( ((PDLLMotionProfile *) inMtnProf )->veloCurrent >= MIN_VELOC) &&
          ( ((PDLLMotionProfile *) inMtnProf )->veloCurrent <= MAX_VELOC)),ASSERT_ID_13 ); 
  
  // here's where the actual adjustment takes place
  ((PDLLMotionProfile *) inMtnProf)->velocity.freq = ((PDLLMotionProfile *) inMtnProf)->veloCurrent;
  
//  printf("===ANGAccel!===\n"); // db

} // PDLLMotionProfile_angAccel()

//--------------------------------------------------------------------------

void PDLLMotionProfile_angVeloc(void *inMtnProf)
{
  PDLLInt distanceAboutToCover = 0;
  
  ASSERT((inMtnProf != NULL), ASSERT_ID_14);
  
  // if initial displacement != 0, then a step will be taken
  if ( ((PDLLMotionProfile *) inMtnProf)->dispRem != DEST_REACHED)
  { ((PDLLMotionProfile *) inMtnProf)->dispRem--; }
  
  ASSERT(( ( ((PDLLMotionProfile *) inMtnProf)->dispRem >= MIN_DISP) &&
          ( ((PDLLMotionProfile *) inMtnProf)->dispRem <= MAX_DISP)),ASSERT_ID_15 );

  distanceAboutToCover = ( ((PDLLMotionProfile *) inMtnProf)->veloCurrent * ((PDLLMotionProfile *) inMtnProf)->veloCurrent) /
                         (2 * MAX_ACCEL);
                         
  // arrived at destination
  if ( ((PDLLMotionProfile *) inMtnProf)->dispRem == DEST_REACHED)
  {    
    // next time velocity is called, everything will be zero'd out
    // we need the delay because the motor hasn't taken the actual step yet
    ((PDLLMotionProfile *) inMtnProf)->velocity.callBackFunc = PDLLMotionProfile_angStop;
  }
  // too close to destination, it's time to slow down --NOT OPTIMIZED--
  else if ( (distanceAboutToCover > ((PDLLMotionProfile *) inMtnProf)->dispTarget) ||
            (distanceAboutToCover >= ((PDLLMotionProfile *) inMtnProf)->dispRem) ) 
  {    
    // next time velocity is called, the motor will be slowing down
    // we need the delay because the motor hasn't taken the actual step yet    
    ((PDLLMotionProfile *) inMtnProf)->velocity.callBackFunc = PDLLMotionProfile_angBgnSlwDwn;    
  }
  
  /*
  #ifndef AVR_STUDIO
  printf("ANGVeloc!\n"); // db
  printf("===rem ANGULAR displacement = %d===\n",  ((PDLLMotionProfile *) inMtnProf)->dispRem); // db
  printf("===current ANGULAR velocity = %d===\n", ((PDLLMotionProfile *) inMtnProf)->veloCurrent); // db
  printf("===current ANGULAR accel = %d===\n", ((PDLLMotionProfile *) inMtnProf)->accelCurrent); // db
  #endif
  */

} // PDLLMotionProfile_angVeloc()

//--------------------------------------------------------------------------
// begin the angular slowdown sequence

void PDLLMotionProfile_angBgnSlwDwn(void *inMtnProf)
{
  ASSERT((inMtnProf != NULL),ASSERT_ID_16);
  
  // undo what the incorrect velocity did also takes into account next motor tick
  mainLogic.leftMotor.sum = mainLogic.linearMotion.velocity.sum + MIN_SUM;   
  mainLogic.rightMotor.sum = mainLogic.linearMotion.velocity.sum + MIN_SUM;
  
  ASSERT((mainLogic.leftMotor.sum >= MIN_SUM),ASSERT_ID_17);  
  ASSERT((mainLogic.rightMotor.sum >= MIN_SUM), ASSERT_ID_18);
  
  ((PDLLMotionProfile *) inMtnProf)->velocity.callBackFunc = PDLLMotionProfile_angSlowDown;
  PDLLMotionProfile_angSlowDown(inMtnProf);

} // PDLLMotionProfile_angBgnSlwDwn()

//--------------------------------------------------------------------------
// the angular slowdown sequence

void PDLLMotionProfile_angSlowDown(void *inMtnProf)
{
  ASSERT((inMtnProf != NULL),ASSERT_ID_19);
 
  // if initial displacement != 0, then a step will be taken
  if ( ((PDLLMotionProfile *) inMtnProf)->dispRem != DEST_REACHED)
  { ((PDLLMotionProfile *) inMtnProf)->dispRem--; }
 
   ASSERT(( ( ((PDLLMotionProfile *) inMtnProf)->dispRem >= MIN_DISP) &&
          ( ((PDLLMotionProfile *) inMtnProf)->dispRem <= MAX_DISP)),ASSERT_ID_20 );

  if ( ((PDLLMotionProfile *) inMtnProf)->dispRem == DEST_REACHED)
  {    
    // next time velocity is called, everything will be zero'd out
    // we need the delay because the motor hasn't taken the actual step yet
    ((PDLLMotionProfile *) inMtnProf)->velocity.callBackFunc = PDLLMotionProfile_angStop;
  }

  ((PDLLMotionProfile *) inMtnProf)->accelCurrent = NO_ACCEL;      // still don't know if this should be here...
  
  // velocity is proportional to the distance remaining
  ((PDLLMotionProfile *) inMtnProf)->veloTarget = (((PDLLMotionProfile *) inMtnProf)->dispRem * 3);

  // don't wanna go TOO slow now...
  if ( ((PDLLMotionProfile *) inMtnProf)->veloTarget < (SLOW_DOWN_VELOC * 3) )
  { ((PDLLMotionProfile *) inMtnProf)->veloTarget = SLOW_DOWN_VELOC * 3; }
    
  /*
  #ifndef AVR_STUDIO
  printf("ANGSlowDown!\n"); // db
  printf("===rem ANGULAR displacement = %d===\n",  ((PDLLMotionProfile *) inMtnProf)->dispRem); // db
  printf("===current ANGULAR velocity = %d===\n", ((PDLLMotionProfile *) inMtnProf)->veloCurrent); // db  
  printf("===current ANGULAR accel = %d===\n", ((PDLLMotionProfile *) inMtnProf)->accelCurrent); // db  
  #endif 
  */
  
} // PDLLMotionProfile_angSlowDown()

//--------------------------------------------------------------------------

void PDLLMotionProfile_angStop(void *inMtnProf)
{
   ASSERT((inMtnProf != NULL),ASSERT_ID_21);
  
  ((PDLLMotionProfile *) inMtnProf)->accelCurrent = NO_ACCEL;
    
  ((PDLLMotionProfile *) inMtnProf)->veloTarget = STOPPED_VELOC;  
  ((PDLLMotionProfile *) inMtnProf)->veloCurrent = STOPPED_VELOC;
  ((PDLLMotionProfile *) inMtnProf)->velocity.freq = STOPPED_VELOC;
  ((PDLLMotionProfile *) inMtnProf)->velocity.sum = RESET_SUM;
  
  ((PDLLMotionProfile *) inMtnProf)->dispTarget = DEST_REACHED;
  ((PDLLMotionProfile *) inMtnProf)->dispRem = DEST_REACHED;
  
  ((PDLLMotionProfile *) inMtnProf)->reachedDest = PDLLFLAG_T;                 // breaks out of outer motor execution loop
    
  // how about resetting sum's here too?    
    
  /*
  #ifndef AVR_STUDIO
  printf("ANGStop!\n"); // db
  printf("===rem ANGULAR displacement = %d===\n",  ((PDLLMotionProfile *) inMtnProf)->dispRem); // db
  printf("===current ANGULAR velocity = %d===\n", ((PDLLMotionProfile *) inMtnProf)->veloCurrent); // db  
  printf("===current ANGULAR accel = %d===\n", ((PDLLMotionProfile *) inMtnProf)->accelCurrent); // db
  #endif
  */
  
// maybe the function pointers should be reset here? - or use the prepForLaunch() function

} // PDLLMotionProfile_angStop()
