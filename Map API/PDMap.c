/*
 *  PDMap.c
 *  MazeSimulator
 *
 *  Created by Paul De Leon on 10/24/05.
 *  Copyright 2005 Project Densho. All rights reserved.
 *
 */

#include "PDMap.h"

//-------------------------------------------------------------------------------------------
// inits the floodfill values of all the cells of the map 

void initMap(struct PDMap *inMap)
{
  PDMapCell initPDMapCell = INIT_PDMAPCELL;             // sets the function pointers
  
  PDInt currentCellID = FIRST_CELL_ID;                     // indexes the cell physical coordinates array
  PDInt i = 0;
  PDInt j = 0;

  ASSERT( ((X_DIM > 0) && (Y_DIM > 0)),ASSERT_ID_5 );
  ASSERT((inMap != NULL),ASSERT_ID_6);
  
  // initialize all the cells' values
  for (j = 0; j < Y_MAX; j++)
  {
    for (i = 0; i < X_MAX; i++)
    {
      inMap->mapCells[j][i] = initPDMapCell;
      initCell(&inMap->mapCells[j][i], i, j);
    }
  }
  
  // set all the physical coordinates for each cell
  currentCellID = FIRST_CELL_ID;
  
  for (i = X_MIN; i < X_MAX; i++)
  { 
    for (j = Y_MAX - 1; j >= Y_MIN; j--)
    {
      ASSERT((currentCellID < TOTAL_NUM_CELLS),ASSERT_ID_7);
      
      inMap->cellPhysCoords[currentCellID].x = i;
      inMap->cellPhysCoords[currentCellID].y = j;
      currentCellID++;
    }
  }

  // set all the logical coordinates for each cell 
  currentCellID = FIRST_CELL_ID;
  
  for (i = X_MIN; i < X_MAX; i++)
  { 
    for (j = Y_MAX - 1; j >= Y_MIN; j--)
    {
      ASSERT((currentCellID < TOTAL_NUM_CELLS),ASSERT_ID_8);

      inMap->cellLogicCoords[currentCellID].x = i;
      inMap->cellLogicCoords[currentCellID].y = Y_OFFSET - j;
      currentCellID++;
    }
  }
  
  // zero out vertical wall array
  for (i = 0; i < TOTAL_WALL_SIZE_V_IN_BYTES; i++) { inMap->vertWalls[i] = '\0';  }
  
  // zero out horiz wall array
  for (i = 0; i < TOTAL_WALL_SIZE_H_IN_BYTES; i++) { inMap->horizWalls[i] = '\0'; }
  
  inMap->vertWalls[FIRST_BYTE] = FIRST_WALL;  // this is a guaranteed wall
    
} // initMap() 

//-------------------------------------------------------------------------------------------
// finds the neighbors of a given cell in a map

PDMapCellSet findCellNeighbors(struct PDMap *inMap, PDMapCellWalls currentCellWalls, PDInt x, PDInt y)
{
  PDMapCellSet currentCellNeighbors = INIT_PDMAPCELLSET;
  PDInt i = 0;
  
  ASSERT((inMap != NULL),ASSERT_ID_9);
  ASSERT((x >= X_MIN && x < X_MAX),ASSERT_ID_10);
  ASSERT((y >= Y_MIN && y < Y_MAX),ASSERT_ID_11);

  for (i = 0; i < TOTAL_NUM_CELLS_IN_BYTES; i++) { ASSERT((currentCellNeighbors.mapCellSet[i] == '\0'),ASSERT_ID_12); }
  
  if ( (currentCellWalls & NORTH_WALL) == NO_WALL) 
  { 
    currentCellNeighbors.mapCellSet[inMap->mapCells[y + NORTH_OFFSET][x].cellSetIndex] |=  (1 << inMap->mapCells[y + NORTH_OFFSET][x].cellSetPosition);
  }      
  
  if ( (currentCellWalls & SOUTH_WALL) == NO_WALL) 
  { 
    currentCellNeighbors.mapCellSet[inMap->mapCells[y + SOUTH_OFFSET][x].cellSetIndex] |=  (1 << inMap->mapCells[y + SOUTH_OFFSET][x].cellSetPosition);
  }  
  
  if ( (currentCellWalls & EAST_WALL) == NO_WALL) 
  { 
    currentCellNeighbors.mapCellSet[inMap->mapCells[y][x + EAST_OFFSET].cellSetIndex] |=  (1 << inMap->mapCells[y][x + EAST_OFFSET].cellSetPosition);
  }  
  
  if ( (currentCellWalls & WEST_WALL) == NO_WALL) 
  { 
    currentCellNeighbors.mapCellSet[inMap->mapCells[y][x + WEST_OFFSET].cellSetIndex] |=  (1 << inMap->mapCells[y][x + WEST_OFFSET].cellSetPosition);
  }
  
  return currentCellNeighbors;

}  // findCellNeighbors()
