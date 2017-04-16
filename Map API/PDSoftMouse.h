/*
 *  PDSoftMouse.h
 *  MazeSimulator
 *
 *  Created by Paul De Leon on 11/18/05.
 *  Copyright 2005 ProjectDensho. All rights reserved.
 *
 */

#include "PDConfig.h"

#ifndef PDSOFTMOUSE_H
    #define PDSOFTMOUSE_H
      
typedef struct PDSoftMouse
{
  PDMap               internalMap;
  PDMapCellCoordinate currentXY;
  PDDirection dir;
  PDFlag frontWallFound;
 
  void (*errorCleanup) ();
  
  void (*updateMouseXY) ();             // used for updating XY coordinate during movement
  int (*checkForStopConditions) ();    // during movement - checks for frontwall or outdated floodfill values
  
  int (*moveNorth) ( PDInt*,  PDInt, PDFlag);
  int (*moveSouth) ( PDInt*,  PDInt, PDFlag);
  int (*moveEast) ( PDInt*,  PDInt, PDFlag);
  int (*moveWest) ( PDInt*,  PDInt, PDFlag);
 
  PDMapCellWalls (*northWallCheckFunc) ();
  PDMapCellWalls (*southWallCheckFunc) ();
  PDMapCellWalls (*eastWallCheckFunc) ();
  PDMapCellWalls (*westWallCheckFunc) ();
  
    
} PDSoftMouse;

 void initMouse(struct PDSoftMouse*);
 
 void updateMouseXY_N();
 void updateMouseXY_S();
 void updateMouseXY_E();
 void updateMouseXY_W();   
 
 int checkForStopConditions_N();
 int checkForStopConditions_S();
 int checkForStopConditions_E(); 
 int checkForStopConditions_W(); 
 
 PDMapCellWalls fireSensors(struct PDSoftMouse*, struct PDMap*);

 void floodFill(struct PDMap*, PDInt*, PDInt,char);
 PDInt straightRun(struct PDSoftMouse*, PDInt*, PDInt, char);        // this is really just an exploration run..should rename it
 PDInt akinaRun(PDInt*, PDInt, char);                                // this is a faster version of the exploration run



 // void magnetRun(struct PDMap*);            // favor walls that bring you theoritically to the center
 // void vertWallRun(struct PDMap*);          // favor vert walls over horiz
 // void horizWallRun(struct PDMap*);         // favor horiz walls over vert
 // void dfsRun(struct PDMap*);               // scan walls all the way till the end, then choose farthest one
 // void bfsRun(struct PDMap*);               // do a straight run once center is hit, search all around it to scan in more walls
 // void intersectionRun(struct PDMap*);      // run floodfill at every intersection
 // void regionRun(struct PDMap*);            // do a straight run, then pinpoint region which is least explored, then set dest to that region

#endif
