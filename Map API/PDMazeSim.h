/*
 *  PDMazeSim.h
 *  MazeSimulator
 *
 *  Created by Paul De Leon on 10/22/05.
 *  Copyright 2005 Project Densho. All rights reserved.
 *
 */

#include "PDConfig.h"

#ifndef PDMAZESIM_H
  #define PDMAZESIM_H

void runAll();
void runAkina();
void createSolnMaze();

void runMapAPI(const int, const char*);
void solnMazeSetup(PDMap*, PDInt, const char*);
void outputSolutionMaze(PDMap*);
void runMouse(PDSoftMouse*, PDMap*, char);
char displayMainMenu(void);

  
#endif
