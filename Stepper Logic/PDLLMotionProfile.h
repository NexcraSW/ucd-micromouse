/*
 *  PDLLMotionProfile.h
 *  
 *  Copyright 2006 ProjectDensho. All rights reserved.
 *
 */

#include "PDConfig.h"

#ifndef PDLLMOTIONPROFILE_H
    #define PDLLMOTIONPROFILE_H
 

typedef struct PDLLMotionProfile
{
        
 	struct PDLLFreqDiv velocity; 		// Divider that controls velocity
	struct PDLLFreqDiv accel; 			// Controls acceleration
    
	PDLLLong dispTarget; 			    // target displacement
    PDLLLong dispRem;                   // remaining displacement
    
	PDLLInt veloTarget; 				// target velocity
    PDLLInt veloCurrent;                // current velocity
    
    PDLLInt accelCurrent;               // current acceleration
	PDLLInt accelMax; 					// maximum acceleration
	
    PDLLInt vel_sign; 					// these may be no longer needed
	PDLLInt acc_sign;                   // since we can just pull a sign from the dispTarget
                                        // or we can even toggle the pin inside the moveForward() and moveBack() functions
                                        
    PDLLFlag reachedDest;               // set to 'T' when mouse reaches dest, 'F' when mouse hasn't yet
  
} PDLLMotionProfile;
 
void PDLLMotionProfile_init(PDLLMotionProfile*);

// may be some probs down the line with two sets of functions that are essentially the same
// perhaps merge into one set of functions later on

void PDLLMotionProfile_linAccel(void*);
void PDLLMotionProfile_linVeloc(void*);
void PDLLMotionProfile_linBgnSlwDwn(void*);
void PDLLMotionProfile_linSlowDown(void*);
void PDLLMotionProfile_linFWallPull(void*);
void PDLLMotionProfile_linStop(void*);

void PDLLMotionProfile_angAccel(void*);
void PDLLMotionProfile_angVeloc(void*);
void PDLLMotionProfile_angBgnSlwDwn(void*);
void PDLLMotionProfile_angSlowDown(void*);
void PDLLMotionProfile_angStop(void*);

  
#endif
