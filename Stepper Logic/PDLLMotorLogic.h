/*
 *  PDLLMotorLogic.h
 *  
 *  Copyright 2006 ProjectDensho. All rights reserved.
 *
 */

#include "PDConfig.h"

#ifndef PDLLMOTORLOGIC_H
    #define PDLLMOTORLOGIC_H
 

typedef struct PDLLMotorLogic
{
  struct PDLLMotionProfile linearMotion;
  struct PDLLMotionProfile angularMotion;
  struct PDLLFreqDiv leftMotor;
  struct PDLLFreqDiv rightMotor;
  
  PDLLDir dispDir;
  PDInt numLinSteps;         // accounting variable used to tell us when to take next cell actions...

} PDLLMotorLogic;

void (*PDLLMotorLogic_calcMtrFq) ();            // points to the correct function to calculate the motor freq

void PDLLMotorLogic_init(PDLLMotorLogic*);      // hard reset - resets EVERYTHING!
void PDLLMotorLogic_reset();                    // semi-soft reset - resets fps, flags and even motor freqs

void PDLLMotorLogic_prepForLaunchLin();            // soft reset - resets function pointers and flags
void PDLLMotorLogic_prepForLaunchAng();            // soft reset - resets function pointers and flags

void PDLLMotorLogic_commenceMovt();             // gets the ball rolling

void PDLLMotorLogic_mainTick();                 // ticks the freq div's

void PDLLMotorLogic_mtrFq();                    // calculates motor frequencies for straight movement
void PDLLMotorLogic_mtrFq_L();                  // calculates motor frequencies for left turns
void PDLLMotorLogic_mtrFq_R();                  // calculates motor frequencies for right turns

void PDLLMotorLogic_adjDirForAng();             // adjusts the direction pins for angular displacement

// called individually - can be used for in-place turns and simple linear motion
// called in combinations - can be used for dynamic turning and motion w/error correction

// use PDLLMotorLogic_commenceMovt() to start the movement after all the settings are in
// ALWAYS specify linear distance FIRST even if it's 0, and DO NOT combine (forward and backward) or (left and right)!!
// only one linear direction and one angular direction at a time....use commenceMovt() to begin movement
void PDLLMotorLogic_moveForward(PDLLInt numSteps);
void PDLLMotorLogic_moveBack(PDLLInt numSteps);
void PDLLMotorLogic_turnLeft(PDLLInt numSteps);
void PDLLMotorLogic_turnRight(PDLLInt numSteps);

#endif
