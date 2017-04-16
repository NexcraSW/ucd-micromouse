/*
 *  PDMap.h
 *  MazeSimulator
 *
 *  Created by Paul De Leon on 10/24/05.
 *  Copyright 2005 ProjectDensho. All rights reserved.
 *
 */


#include "PDConfig.h"

#ifndef PDMAP_H
    #define PDMAP_H
    
typedef struct PDMap
{
  PDMapCellCoordinate cellPhysCoords[TOTAL_NUM_CELLS];
  PDMapCellCoordinate cellLogicCoords[TOTAL_NUM_CELLS];
  PDMapCellCoordinate *currentXYPosition;
  PDMapCellSet        mapSetCells;
  PDMapCell           mapCells[Y_DIM][X_DIM];
  
  #ifndef AVR_STUDIO
  char                ASCIIRep[ASCII_Y_DIM][ASCII_X_DIM];       // only needed for outputting to a monitor
  #endif
  
  char                vertWalls[TOTAL_WALL_SIZE_V_IN_BYTES];
  char                horizWalls[TOTAL_WALL_SIZE_H_IN_BYTES];
  char                direction;
  
} PDMap;

void initMap(struct PDMap*);
PDMapCellSet findCellNeighbors(struct PDMap*, PDMapCellWalls, PDInt, PDInt);

#endif
