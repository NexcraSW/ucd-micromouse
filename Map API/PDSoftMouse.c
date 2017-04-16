/*
 *  PDSoftMouse.c
 *  MazeSimulator
 *
 *  Created by Paul De Leon on 11/18/05.
 *  Copyright 2005 ProjectDensho. All rights reserved.
 *
 */

#include "PDSoftMouse.h"

#ifndef AVR_STUDIO
  FAKE_AVR_DEC;
#endif

extern PDSoftMouse mainMouse;
extern PDLLMotorLogic mainLogic;
extern PDMap solnMap;

//-------------------------------------------------------------------------------------------
// initializes the mouse with valid init values from a map - called only once per mouse instance

void initMouse(struct PDSoftMouse *inMouse)
{
  PDInt startX = 0;
  PDInt startY = 0;
 
  ASSERT((inMouse != NULL),ASSERT_ID_13);
   
  initMap(&inMouse->internalMap);
  
  startX = inMouse->internalMap.cellPhysCoords[START_CELL_ID].x;
  startY = inMouse->internalMap.cellPhysCoords[START_CELL_ID].y;
  
  ASSERT( ((startX >= X_MIN) && (startX < X_MAX)),ASSERT_ID_14 );
  ASSERT( ((startY >= Y_MIN) && (startY < Y_MAX)),ASSERT_ID_15 );
  
  setX(&inMouse->currentXY, startX);
  setY(&inMouse->currentXY, Y_OFFSET - startY);

  // map needs pointer to know mouse's current position - used for printing
  inMouse->internalMap.currentXYPosition = &inMouse->currentXY;
  ASSERT((inMouse->internalMap.currentXYPosition != NULL),ASSERT_ID_16);
  
  // this is where the mouse starts
  // setCellStatus(&inMouse->internalMap.mapCells[startY][startX], VISITED);
  
  // doesn't need to do any error correction initially
 // inMouse->errorCleanup = noCleanupAlign;

  // the mouse starts by pointing to the north
  inMouse->dir=INIT_DIR;
  
  // the mouse always starts by pointing north - setting to NULL forces mouse to start pointing north in straightRun()
  inMouse->moveNorth = NULL;
  inMouse->moveSouth = NULL;
  inMouse->moveEast = NULL;
  inMouse->moveWest = NULL;
  
  // mouse starts out pointing north
  inMouse->updateMouseXY = updateMouseXY_N;
  inMouse->checkForStopConditions = checkForStopConditions_N;

  inMouse->frontWallFound = NO_WALL;
  
} // initMouse() 

//-------------------------------------------------------------------------------------------

void updateMouseXY_N()
{
  PDInt y = 0;

  mainMouse.currentXY.y += MOVE_NORTH;
  
  y = mainMouse.currentXY.y;

  ASSERT( ( (y >= Y_MIN) && (y < Y_MAX) ), ASSERT_ID_26);

} // updateMouseXY()

//------------------------------------------------------------------------------------------- 
void updateMouseXY_S()
{
  PDInt y = 0;
  
  mainMouse.currentXY.y += MOVE_SOUTH;
  
  y = mainMouse.currentXY.y;
    
  ASSERT( ( (y >= Y_MIN) && (y < Y_MAX) ), ASSERT_ID_26);            
  
} // updateMouseXY() 

//-------------------------------------------------------------------------------------------
 
void updateMouseXY_E()
{
  PDInt x = 0;
  
  mainMouse.currentXY.x += MOVE_EAST;
  
  x = mainMouse.currentXY.x;
  
  ASSERT( ( (x >= X_MIN) && (x < X_MAX) ), ASSERT_ID_26);                          
  
} // updateMouseXY()

//-------------------------------------------------------------------------------------------
void updateMouseXY_W()
{
  PDInt x = 0;
  
  mainMouse.currentXY.x += MOVE_WEST;
  
  x = mainMouse.currentXY.x;
  
  ASSERT( ( (x >= X_MIN) && (x < X_MAX) ), ASSERT_ID_26);
  
} // updateMouseXY()

//-------------------------------------------------------------------------------------------
 
int checkForStopConditions_N()
{
  PDInt currentX = mainMouse.currentXY.x;
  PDInt currentY = Y_OFFSET - mainMouse.currentXY.y;

  #ifdef AVR_STUDIO
  
  fireSensors(&mainMouse, &mainMouse.internalMap);         // map the walls  
  
  if ( (checkFrontWall() == WALL_FOUND) || (mainMouse.internalMap.mapCells[currentY][currentX].floodFillValue < mainMouse.internalMap.mapCells[currentY + NORTH_OFFSET][currentX].floodFillValue) )
  {
    // only do this when the thing is moving!
    if (mainLogic.linearMotion.dispRem > 0)
    { mainLogic.linearMotion.dispRem = ONE_CELL_DISP; }

    return ONE_CELL_DISP;
  }
   else
  { return (ONE_CELL_DISP * 15); }
  
  #else
  PDMapCellWalls currentCellWalls = existWall(&mainMouse.internalMap.mapCells[currentY][currentX], '\0', solnMap.vertWalls, solnMap.horizWalls);
  
  fireSensors(&mainMouse, &solnMap);         // map the walls    
  // wall to the front      
  if ( ((currentCellWalls & NORTH_WALL) == NORTH_WALL) || (mainMouse.internalMap.mapCells[currentY][currentX].floodFillValue < mainMouse.internalMap.mapCells[currentY + NORTH_OFFSET][currentX].floodFillValue) )
  { 
    // only do this when the thing is moving!
    if (mainLogic.linearMotion.dispRem > 0)
    { mainLogic.linearMotion.dispRem = ONE_CELL_DISP; }

    return ONE_CELL_DISP; 
  }
  else
  { return (ONE_CELL_DISP * 15); }
  
  #endif
 
} // checkForStopConditions()

//-------------------------------------------------------------------------------------------
int checkForStopConditions_S()
{
  PDInt currentX = mainMouse.currentXY.x;
  PDInt currentY = Y_OFFSET - mainMouse.currentXY.y;

  fireSensors(&mainMouse, &mainMouse.internalMap);         // map the walls    
  
  #ifdef AVR_STUDIO  
  if ( (checkFrontWall() == WALL_FOUND) || (mainMouse.internalMap.mapCells[currentY][currentX].floodFillValue < mainMouse.internalMap.mapCells[currentY + SOUTH_OFFSET][currentX].floodFillValue) )
  {
    // only do this when the thing is moving!
    if (mainLogic.linearMotion.dispRem > 0)
    { mainLogic.linearMotion.dispRem = ONE_CELL_DISP; }

    return ONE_CELL_DISP;
  }
   else
  { return (ONE_CELL_DISP * 15); }
  #else
  PDMapCellWalls currentCellWalls = existWall(&mainMouse.internalMap.mapCells[currentY][currentX], '\0', solnMap.vertWalls, solnMap.horizWalls);

  fireSensors(&mainMouse, &solnMap);         // map the walls        
  
  // wall to the front
  if ( ( (currentCellWalls & SOUTH_WALL) == SOUTH_WALL) || (mainMouse.internalMap.mapCells[currentY][currentX].floodFillValue < mainMouse.internalMap.mapCells[currentY + SOUTH_OFFSET][currentX].floodFillValue) )
  {
    // only do this when the thing is moving!
    if (mainLogic.linearMotion.dispRem > 0)
    { mainLogic.linearMotion.dispRem = ONE_CELL_DISP; }

    return ONE_CELL_DISP; 
  }
  else
  { return (ONE_CELL_DISP * 15); }
  
  #endif

} // checkForStopConditions()

//-------------------------------------------------------------------------------------------
int checkForStopConditions_E()
{
  PDInt currentX = mainMouse.currentXY.x;
  PDInt currentY = Y_OFFSET - mainMouse.currentXY.y;  

  #ifdef AVR_STUDIO
  fireSensors(&mainMouse, &mainMouse.internalMap);         // map the walls    
  
  if ( (checkFrontWall() == WALL_FOUND) || (mainMouse.internalMap.mapCells[currentY][currentX].floodFillValue < mainMouse.internalMap.mapCells[currentY][currentX + EAST_OFFSET].floodFillValue) )
  {
    // only do this when the thing is moving!
    if (mainLogic.linearMotion.dispRem > 0)
    { mainLogic.linearMotion.dispRem = ONE_CELL_DISP; }
    
    return ONE_CELL_DISP;
  }
   else
  { return (ONE_CELL_DISP * 15); }
  #else
  PDMapCellWalls currentCellWalls = existWall(&mainMouse.internalMap.mapCells[currentY][currentX], '\0', solnMap.vertWalls, solnMap.horizWalls);

  fireSensors(&mainMouse, &solnMap);         // map the walls        
  
  // wall to the front
  if ( ( (currentCellWalls & EAST_WALL) == EAST_WALL)|| (mainMouse.internalMap.mapCells[currentY][currentX].floodFillValue < mainMouse.internalMap.mapCells[currentY][currentX + EAST_OFFSET].floodFillValue) )
  { 
    // only do this when the thing is moving!
    if (mainLogic.linearMotion.dispRem > 0)
    { mainLogic.linearMotion.dispRem = ONE_CELL_DISP; }

    return ONE_CELL_DISP; 
  }
  else
  { return (ONE_CELL_DISP * 15); }
  
  #endif

 
} // checkForStopConditions()

//-------------------------------------------------------------------------------------------
int checkForStopConditions_W()
{
  PDInt currentX = mainMouse.currentXY.x;
  PDInt currentY = Y_OFFSET - mainMouse.currentXY.y;

  #ifdef AVR_STUDIO
    
  fireSensors(&mainMouse, &mainMouse.internalMap);         // map the walls    
  
  if ( (checkFrontWall() == WALL_FOUND) || (mainMouse.internalMap.mapCells[currentY][currentX].floodFillValue < mainMouse.internalMap.mapCells[currentY][currentX + WEST_OFFSET].floodFillValue) )
  {
    // only do this when the thing is moving!
    if (mainLogic.linearMotion.dispRem > 0)
    { mainLogic.linearMotion.dispRem = ONE_CELL_DISP; }

    return ONE_CELL_DISP;
  }
   else
  { return (ONE_CELL_DISP * 15); }
  
  #else
  PDMapCellWalls currentCellWalls = existWall(&mainMouse.internalMap.mapCells[currentY][currentX], '\0', solnMap.vertWalls, solnMap.horizWalls);

  fireSensors(&mainMouse, &solnMap);         // map the walls        
  
  // wall to the front
  if ( ( (currentCellWalls & WEST_WALL) == WEST_WALL) || (mainMouse.internalMap.mapCells[currentY][currentX].floodFillValue < mainMouse.internalMap.mapCells[currentY][currentX + WEST_OFFSET].floodFillValue) )
  { 
    // only do this when the thing is moving!
    if (mainLogic.linearMotion.dispRem > 0)
    { mainLogic.linearMotion.dispRem = ONE_CELL_DISP; }

    return ONE_CELL_DISP; 
  }
  else
  { return (ONE_CELL_DISP * 15); }
  
  #endif

} // checkForStopConditions()

//-------------------------------------------------------------------------------------------
// updates the mouse's internal map wall array - relies on whatever (x,y) coordinates mouse currently has

PDMapCellWalls fireSensors(struct PDSoftMouse *inMouse, struct PDMap *mapSoln)
{
  PDInt currentX = 0;                 // mouse's current logical position in the maze - different from actual physical position from maze
  PDInt currentY = 0;                 // so that sensors can fire and read into wall arrays for one-cell ahead

  // these access individual walls in the two arrays
  PDInt northWallBitNum = 0;
  PDInt southWallBitNum = 0;
  PDInt eastWallBitNum = 0;
  PDInt westWallBitNum = 0;            
                
  PDMapCellWalls currentCellWalls = NO_WALL;
  PDMapCellWalls currentCellStatus = NO_WALL;   // generalIDE - used for testing sensor fp's
                                                // AVR Studio - used for storing status of wall from each direction
  ASSERT((inMouse != NULL),ASSERT_ID_17);
  ASSERT((mapSoln != NULL),ASSERT_ID_18);            // may be NULL when code is run on actual mouse without a solution map
   
  currentX = inMouse->currentXY.x;
  currentY = inMouse->currentXY.y;
  
  ASSERT( ((currentX >= X_MIN) && (currentX < X_MAX)),ASSERT_ID_19 );
  ASSERT( ((currentY >= Y_MIN) && (currentY < Y_MAX)),ASSERT_ID_20 );
  
  // checking if function pointers are ok
  ASSERT(( 
          ( (inMouse->northWallCheckFunc == checkFrontWall) && (inMouse->southWallCheckFunc == clearBackWall) 
            && (inMouse->eastWallCheckFunc == checkRightWall) && (inMouse->westWallCheckFunc == checkLeftWall) ) ||
            
          ( (inMouse->northWallCheckFunc == clearBackWall) && (inMouse->southWallCheckFunc == checkFrontWall) 
            && (inMouse->eastWallCheckFunc == checkLeftWall) && (inMouse->westWallCheckFunc == checkRightWall) ) ||
            
          ( (inMouse->northWallCheckFunc == checkLeftWall) && (inMouse->southWallCheckFunc == checkRightWall) 
            && (inMouse->eastWallCheckFunc == checkFrontWall) && (inMouse->westWallCheckFunc == clearBackWall) ) ||
            
          ( (inMouse->northWallCheckFunc == checkRightWall) && (inMouse->southWallCheckFunc == checkLeftWall) 
            && (inMouse->eastWallCheckFunc == clearBackWall) && (inMouse->westWallCheckFunc == checkFrontWall) )),ASSERT_ID_21 
        );
  
  #ifndef AVR_STUDIO
  currentCellWalls = existWall(&inMouse->internalMap.mapCells[Y_OFFSET - currentY][currentX], '\0', mapSoln->vertWalls, mapSoln->horizWalls);
  
  // a weak check to see if SensorLogic output should be suppressed
  if (inMouse->moveNorth != gen_move) 
  {
    currentCellStatus = (*inMouse->northWallCheckFunc) ();
    printf("North wall to the %c of the mouse\n", currentCellStatus);
  
    currentCellStatus = (*inMouse->southWallCheckFunc) ();
    printf("South wall to the %c of the mouse\n", currentCellStatus);
  
    currentCellStatus = (*inMouse->eastWallCheckFunc) ();  
    printf("East wall to the  %c of the mouse\n", currentCellStatus);
    
    currentCellStatus = (*inMouse->westWallCheckFunc) ();
    printf("West wall to the  %c of the mouse\n\n", currentCellStatus);
  }
  
  #else
  // the correctness of the assignments to currentCellWalls should be verified
  currentCellStatus = (*inMouse->northWallCheckFunc) ();
  currentCellWalls |= (currentCellStatus << NORTH_WALL_BIT_POS);
 
  currentCellStatus = (*inMouse->southWallCheckFunc) ();
  currentCellWalls |= (currentCellStatus << SOUTH_WALL_BIT_POS);
  
  currentCellStatus = (*inMouse->eastWallCheckFunc) ();  
  currentCellWalls |= (currentCellStatus << EAST_WALL_BIT_POS);
  
  currentCellStatus = (*inMouse->westWallCheckFunc) ();
  currentCellWalls |= (currentCellStatus << WEST_WALL_BIT_POS);

  // use this when reading off a hard-coded maze
  // currentCellWalls = existWall(&inMouse->internalMap.mapCells[Y_OFFSET - currentY][currentX], '\0', mapSoln->vertWalls, mapSoln->horizWalls);
  
  #endif
  
  northWallBitNum = (X_DIM * currentY) + currentX;
  southWallBitNum = (X_DIM * (currentY - 1) ) + currentX;
  
  eastWallBitNum =  (Y_DIM * currentX) + currentY;
  westWallBitNum = (Y_DIM * (currentX - 1) ) + currentY;

  // add walls to the wall arrays if applicable
  if ( (currentCellWalls & NORTH_WALL) == NORTH_WALL)
  {
    if ( (northWallBitNum >= FIRST_BIT) && (northWallBitNum < TOTAL_WALL_SIZE_H) )       // is a non-boundary wall
      { inMouse->internalMap.horizWalls[northWallBitNum / BITS_PER_CELLSET] |= (1 << (northWallBitNum % BITS_PER_CELLSET) ); }
  }
  else  // shouldn't be a wall there...
  {
     if ( (northWallBitNum >= FIRST_BIT) && (northWallBitNum < TOTAL_WALL_SIZE_H) )       // is a non-boundary wall
     { inMouse->internalMap.horizWalls[northWallBitNum / BITS_PER_CELLSET] &= ~(1 << (northWallBitNum % BITS_PER_CELLSET) ); }
  }

  if ( (currentCellWalls & SOUTH_WALL) == SOUTH_WALL)
  {
    if ( (southWallBitNum >= FIRST_BIT) && (southWallBitNum < TOTAL_WALL_SIZE_H) )       // is a non-boundary wall
      { inMouse->internalMap.horizWalls[southWallBitNum / BITS_PER_CELLSET] |= (1 << (southWallBitNum % BITS_PER_CELLSET) ); }
  }
  else  // shouldn't be a wall there...
  {
    if ( (southWallBitNum >= FIRST_BIT) && (southWallBitNum < TOTAL_WALL_SIZE_H) )       // is a non-boundary wall
      { inMouse->internalMap.horizWalls[southWallBitNum / BITS_PER_CELLSET] &= ~(1 << (southWallBitNum % BITS_PER_CELLSET) ); }
  }

  if ( (currentCellWalls & EAST_WALL) == EAST_WALL)
  {
    if ( (eastWallBitNum >= FIRST_BIT) && (eastWallBitNum < TOTAL_WALL_SIZE_V) )       // is a non-boundary wall
      { inMouse->internalMap.vertWalls[eastWallBitNum / BITS_PER_CELLSET] |= (1 << (eastWallBitNum % BITS_PER_CELLSET) ); }
  }
  else // shouldn't be a wall there....
  {
    if ( (eastWallBitNum >= FIRST_BIT) && (eastWallBitNum < TOTAL_WALL_SIZE_V) )       // is a non-boundary wall
      { inMouse->internalMap.vertWalls[eastWallBitNum / BITS_PER_CELLSET] &= ~(1 << (eastWallBitNum % BITS_PER_CELLSET) ); }  
  }

  if ( (currentCellWalls & WEST_WALL) == WEST_WALL)
  {
    if ( (westWallBitNum >= FIRST_BIT) && (westWallBitNum < TOTAL_WALL_SIZE_V) )       // is a non-boundary wall
      { inMouse->internalMap.vertWalls[westWallBitNum / BITS_PER_CELLSET] |= (1 << (westWallBitNum % BITS_PER_CELLSET) ); }
  }
  else // shouldn't be a wall there...
  {
    if ( (westWallBitNum >= FIRST_BIT) && (westWallBitNum < TOTAL_WALL_SIZE_V) )       // is a non-boundary wall
      { inMouse->internalMap.vertWalls[westWallBitNum / BITS_PER_CELLSET] &= ~(1 << (westWallBitNum % BITS_PER_CELLSET) ); }  
  }
  
  return currentCellWalls;
  
} // fireSensors() 

//-------------------------------------------------------------------------------------------
// runs the floodfill algorithm on the given map

void floodFill(struct PDMap *inMap, PDInt *destCellIDs, PDInt numDestCells, char stepByStep)
{
  PDMapCellSet currentCellSet = INIT_PDMAPCELLSET;          // holds set of cells to be operated on
  PDMapCellSet neighborCellSet = INIT_PDMAPCELLSET;
  PDMapCellSet nextCellSet = INIT_PDMAPCELLSET;             // holds next cell set to be processed
  PDMapCellSet tempCellSet = INIT_PDMAPCELLSET;             // used for swapping

  PDMapCellWalls currentCellWalls = '\0';

  PDInt currentCellID = 0;
  PDInt currentCellX = 0;
  PDInt currentCellY = 0; 

  PDInt currentCellNeighborID = 0;
  PDInt currentCellNeighborX = 0;
  PDInt currentCellNeighborY = 0;

  PDInt currentLevel = 1;  

  PDInt destCellX = 0;
  PDInt destCellY = 0;
  PDInt i = 0;              
  PDInt j = 0;
    
  ASSERT((inMap != NULL),ASSERT_ID_22);
  ASSERT((destCellIDs != NULL),ASSERT_ID_23);
  ASSERT(( (numDestCells >= MIN_NUM_DEST_CELLS) && (numDestCells <= NUM_CENTER_CELLS)),ASSERT_ID_24 );
    
  #ifndef AVR_STUDIO
  if (stepByStep)
  {
    printf("Running\n");
    printf("floodfill\n"); 
    printf("on current maze....\n\n");
  }
  #endif 
     

  // erase all old floodFillValues
  for (j = Y_MIN; j < Y_MAX; j++) { for (i = X_MIN; i < X_MAX; i++) { inMap->mapCells[j][i].floodFillValue = UNDEF_FF_VALUE; } }

  // re-enter the destCells
  for (i = 0; i < numDestCells; i++) 
  {
    ASSERT((destCellIDs[i] < TOTAL_NUM_CELLS),ASSERT_ID_3);  // assert fires here...
    
    addCell(&currentCellSet, destCellIDs[i]);
    
    destCellX = inMap->cellPhysCoords[destCellIDs[i] ].x;
    destCellY = inMap->cellPhysCoords[destCellIDs[i] ].y;
   
    ASSERT( ((destCellX >= 0) && (destCellX < X_MAX)),ASSERT_ID_26 );
    ASSERT( ((destCellY >= 0) && (destCellY < Y_MAX)),ASSERT_ID_27 );
    
    inMap->mapCells[destCellY][destCellX].floodFillValue = DEST_FF_VALUE;
  }
  
  while (isEmpty(&currentCellSet) != CELLSET_EMPTY)
  {
    while (isEmpty(&currentCellSet) != CELLSET_EMPTY)
    {
      currentCellID = nextCell(&currentCellSet);
      ASSERT( ((currentCellID >= 0) && (currentCellID < TOTAL_NUM_CELLS)),ASSERT_ID_28 );
      
      currentCellX = inMap->cellPhysCoords[currentCellID].x;
      currentCellY = inMap->cellPhysCoords[currentCellID].y;
      
      ASSERT(( (currentCellX >= 0) && (currentCellX < X_MAX)),ASSERT_ID_29);
      ASSERT(( (currentCellY >= 0) && (currentCellY < Y_MAX)),ASSERT_ID_30);
  
      currentCellWalls = existWall(&inMap->mapCells[currentCellY][currentCellX], '\0', inMap->vertWalls, inMap->horizWalls);
      neighborCellSet = findCellNeighbors(inMap, currentCellWalls, currentCellX, currentCellY);
      
      while (isEmpty(&neighborCellSet) != CELLSET_EMPTY)
      {
        currentCellNeighborID = nextCell(&neighborCellSet);
        ASSERT(( (currentCellNeighborID >= 0) && (currentCellNeighborID < TOTAL_NUM_CELLS)),ASSERT_ID_31);
        
        currentCellNeighborX = inMap->cellPhysCoords[currentCellNeighborID].x;
        currentCellNeighborY = inMap->cellPhysCoords[currentCellNeighborID].y;
        
        ASSERT( ((currentCellNeighborX >= 0) && (currentCellNeighborX < X_MAX)),ASSERT_ID_0 );
        ASSERT( ((currentCellNeighborY >= 0) && (currentCellNeighborY < Y_MAX)),ASSERT_ID_1 );
        
        if (inMap->mapCells[currentCellNeighborY][currentCellNeighborX].floodFillValue == UNDEF_FF_VALUE)
        {
          inMap->mapCells[currentCellNeighborY][currentCellNeighborX].floodFillValue = currentLevel;
          addCell(&nextCellSet, inMap->mapCells[currentCellNeighborY][currentCellNeighborX].cellID);
        }
      }
    }
    
    currentLevel++;
    
    tempCellSet = currentCellSet;
    currentCellSet = nextCellSet;
    nextCellSet = tempCellSet;
  }

} // floodFill()

//-------------------------------------------------------------------------------------------
// runs floodFill everytime the mouse gets stuck

PDInt straightRun(struct PDSoftMouse *inMouse, PDInt *destCellIDs, PDInt numDestCells, char stepByStep)
{
  PDMapCellWalls currentCellWalls = NO_WALL;       // stores which cells are available to move to
  
  PDInt numMoves = 0;                              // keeps a running total on how much the mouse had to move to get to the center 
  PDInt centerReached = NOT_REACHED;
  
  PDInt lowestFFValue = 0;         // used to decide which next available cell to go to
  
  PDInt currentCellX = 0;          // the memory cell the mouse is currently in
  PDInt currentCellY = 0;
  
  PDInt destCellsIndex = 0;        // traverse though the dest cells
  
  PDInt moveCorrectionY = 0;
  PDInt moveCorrectionX = 0;
   
  PDShort ffStuck = FF_NOT_STUCK;  // have to detect if floodfill is stuck i.e. can't get to the dest cell
  
  char nextCellDir = NO_DIR;
  
  #ifndef AVR_STUDIO
  char moveKey = '\0';             // dummy char for scanf to eat
  #endif
  
  ASSERT((inMouse != NULL),ASSERT_ID_2);
  ASSERT((destCellIDs != NULL),ASSERT_ID_3);
  ASSERT( ((numDestCells >= MIN_NUM_DEST_CELLS) && (numDestCells <= NUM_CENTER_CELLS)),ASSERT_ID_4 );
  
  //---initial setup---
  if (stepByStep)
  {
    #ifndef AVR_STUDIO
    printf("Let's begin taking the maze step by step...\n");
    printf("Hit enter to continue...\n\n");
    scanf("%c", &moveKey);
	#endif
    
    // if it's the very first time, the mouse starts by pointing north
    if ( ( (inMouse->moveNorth == NULL) && (inMouse->moveSouth == NULL) && (inMouse->moveEast == NULL) && (inMouse->moveWest == NULL) ) ||
         ( (inMouse->moveNorth == gen_move) && (inMouse->moveSouth == gen_move) && (inMouse->moveEast == gen_move) && (inMouse->moveWest == gen_move) ) )
    {
      inMouse->moveNorth = n_moveNorth;       // the mouse always starts by pointing north
      inMouse->moveSouth = n_moveSouth;
      inMouse->moveEast = n_moveEast;
      inMouse->moveWest = n_moveWest;
    }
  }
  else  // to match the test case solns
  {
    inMouse->moveNorth = gen_move; 
    inMouse->moveSouth = gen_move;
    inMouse->moveEast = gen_move;
    inMouse->moveWest = gen_move;
  }
   
  // <don't we need to run floodfill here?> - ran before this function is called
       
  while (centerReached == NOT_REACHED)
  {
    currentCellX = inMouse->currentXY.x;
    currentCellY = Y_OFFSET - inMouse->currentXY.y;
  
    ASSERT( ((currentCellX >= X_MIN) && (currentCellX < X_MAX)),ASSERT_ID_5 );
    ASSERT( ((currentCellY >= Y_MIN) && (currentCellY < Y_MAX)),ASSERT_ID_6 );
    
	#ifndef AVR_STUDIO
    if (stepByStep) { printf("===============Current Position: x = %d   y = %d====================\n", inMouse->currentXY.x, inMouse->currentXY.y); }
    #endif

    for (destCellsIndex = 0; destCellsIndex < numDestCells; destCellsIndex++)     // reached the center
    {
      if ( inMouse->internalMap.mapCells[currentCellY][currentCellX].cellID == destCellIDs[destCellsIndex]) 
      { centerReached = REACHED; 
	    //return numMoves; 
	  }
    }

    #ifndef AVR_STUDIO
    if (stepByStep)
    {
      PDMapPrinter_outputMap(&inMouse->internalMap);
      printf("Hit enter to continue...\n");       
      scanf("%c", &moveKey);
    }
	#endif

    // values to reset after each round
    lowestFFValue = inMouse->internalMap.mapCells[currentCellY][currentCellX].floodFillValue;   
    nextCellDir = NO_DIR;
    
    // function fireSensors() looks at one cell ahead, existWall() looks in current cell
    currentCellWalls = existWall(&inMouse->internalMap.mapCells[currentCellY][currentCellX], '\0', inMouse->internalMap.vertWalls, inMouse->internalMap.horizWalls);
    
    // decide which direction to go to next i.e. choose a direction with no walls
    if ( (currentCellWalls & NORTH_WALL) == NO_WALL)
    {
      if (inMouse->internalMap.mapCells[currentCellY + NORTH_OFFSET][currentCellX].floodFillValue < lowestFFValue)
      {
        nextCellDir = NORTH_DIR;
		moveCorrectionY = MOVE_NORTH;
		moveCorrectionX = 0;

        lowestFFValue = inMouse->internalMap.mapCells[currentCellY + NORTH_OFFSET][currentCellX].floodFillValue;
      }
    }
    
    if ( (currentCellWalls & SOUTH_WALL) == NO_WALL)
    {
      if (inMouse->internalMap.mapCells[currentCellY + SOUTH_OFFSET][currentCellX].floodFillValue < lowestFFValue)
      {
        nextCellDir = SOUTH_DIR;
		moveCorrectionY = MOVE_SOUTH;
		moveCorrectionX = 0;

        lowestFFValue = inMouse->internalMap.mapCells[currentCellY + SOUTH_OFFSET][currentCellX].floodFillValue;
      }
    }
    
    if ( (currentCellWalls & EAST_WALL) == NO_WALL)
    {
      if (inMouse->internalMap.mapCells[currentCellY][currentCellX + EAST_OFFSET].floodFillValue < lowestFFValue)
      {
        nextCellDir = EAST_DIR;
		moveCorrectionY = 0;
		moveCorrectionX = MOVE_EAST;

        lowestFFValue = inMouse->internalMap.mapCells[currentCellY][currentCellX + EAST_OFFSET].floodFillValue;
      }
    }
    
    if ( (currentCellWalls & WEST_WALL) == NO_WALL)
    {
      if (inMouse->internalMap.mapCells[currentCellY][currentCellX + WEST_OFFSET].floodFillValue < lowestFFValue)
      {
        nextCellDir = WEST_DIR;
		moveCorrectionY = 0;
		moveCorrectionX = MOVE_WEST;

        lowestFFValue = inMouse->internalMap.mapCells[currentCellY][currentCellX + WEST_OFFSET].floodFillValue;
      }
    }
        
    // makes sure the function pointers are not null and are all in the correct set
    ASSERT(( 
            ((inMouse->moveNorth == n_moveNorth) && (inMouse->moveSouth == n_moveSouth) && 
             (inMouse->moveEast == n_moveEast) && (inMouse->moveWest == n_moveWest))   
            ||
              ((inMouse->moveNorth == s_moveNorth) && (inMouse->moveSouth == s_moveSouth) && 
               (inMouse->moveEast == s_moveEast) && (inMouse->moveWest == s_moveWest))
            ||
              ((inMouse->moveNorth == e_moveNorth) && (inMouse->moveSouth == e_moveSouth) && 
               (inMouse->moveEast == e_moveEast) && (inMouse->moveWest == e_moveWest))   
            ||
              ((inMouse->moveNorth == w_moveNorth) && (inMouse->moveSouth == w_moveSouth) && 
               (inMouse->moveEast == w_moveEast) && (inMouse->moveWest == w_moveWest))   
            ||
              ((inMouse->moveNorth == gen_move) && (inMouse->moveSouth == gen_move) && 
               (inMouse->moveEast == gen_move) && (inMouse->moveWest == gen_move))),ASSERT_ID_7
         );
        
    // here's where the moving gets done
    if      (nextCellDir == NORTH_DIR) { inMouse->currentXY.y += MOVE_NORTH; numMoves++; (*inMouse->moveNorth) (destCellIDs, numDestCells, NO_CONT_RUN); ffStuck = FF_NOT_STUCK; }
    else if (nextCellDir == SOUTH_DIR) { inMouse->currentXY.y += MOVE_SOUTH; numMoves++; (*inMouse->moveSouth) (destCellIDs, numDestCells, NO_CONT_RUN); ffStuck = FF_NOT_STUCK; }
    else if (nextCellDir == EAST_DIR)  { inMouse->currentXY.x += MOVE_EAST; numMoves++;  (*inMouse->moveEast) (destCellIDs, numDestCells, NO_CONT_RUN); ffStuck = FF_NOT_STUCK; }
    else if (nextCellDir == WEST_DIR)  { inMouse->currentXY.x += MOVE_WEST; numMoves++;  (*inMouse->moveWest) (destCellIDs, numDestCells, NO_CONT_RUN); ffStuck = FF_NOT_STUCK; }
    else                               { floodFill(&inMouse->internalMap, destCellIDs, numDestCells, stepByStep); ffStuck++; }

    if (ffStuck >= FF_STUCK)
    {
      _CLI();
      LED_PORT = ALL_HIGH;
      LED_PORT = 0xDF;
      LED_PORT &= FF_STUCK_ERROR_1;
      
      printf("Floodfill Stuck....\n");
      exit(-1);
    }

  } // while
    
  return numMoves;
    
} // straightRun()

//-------------------------------------------------------------------------------------------
// runs floodfill and front wall alignment every ONE_CELL_DISP number of steps

PDInt akinaRun(PDInt *destCellIDs, PDInt numDestCells, char stepByStep)
{
  PDMapCellWalls currentCellWalls = NO_WALL;       // stores which cells are available to move to
  
  PDInt numMoves = 0;                              // keeps a running total on how much the mouse had to move to get to the center 
  PDInt centerReached = NOT_REACHED;

  PDInt lowestFFValue = 0;         // used to decide which next available cell to go to
  
  PDInt currentCellX = 0;          // the memory cell the mouse is currently in
  PDInt currentCellY = 0;
  
  PDInt destCellsIndex = 0;        // traverse though the dest cells
  
  PDLLInt nextMoveSteps = 0;       // covers the base case

  PDShort ffStuck = FF_NOT_STUCK;  // have to detect if floodfill is stuck i.e. can't get to the dest cell
  
  PDDirection   nextCellDir = NO_DIR;

  #ifndef AVR_STUDIO
  char moveKey = '\0';             // dummy char for scanf to eat
  #endif

  ASSERT((destCellIDs != NULL),ASSERT_ID_3);
  ASSERT( ((numDestCells >= MIN_NUM_DEST_CELLS) && (numDestCells <= NUM_CENTER_CELLS)),ASSERT_ID_4 );
  
  //---initial setup---
  if (stepByStep)
  {
    #ifndef AVR_STUDIO
    printf("Let's begin taking the maze step by step...\n");
    printf("Hit enter to continue...\n\n");
    scanf("%c", &moveKey);
	#endif
    
    // if it's the very first time, the mouse starts by pointing north
    if ( ( (mainMouse.moveNorth == NULL) && (mainMouse.moveSouth == NULL) && (mainMouse.moveEast == NULL) && (mainMouse.moveWest == NULL) ) ||
         ( (mainMouse.moveNorth == gen_move) && (mainMouse.moveSouth == gen_move) && (mainMouse.moveEast == gen_move) && (mainMouse.moveWest == gen_move) ) )
    {
      mainMouse.moveNorth = n_moveNorth;       // the mouse always starts by pointing north
      mainMouse.moveSouth = n_moveSouth;
      mainMouse.moveEast = n_moveEast;
      mainMouse.moveWest = n_moveWest;
    }
  }

  while (centerReached == NOT_REACHED)
  {
    currentCellX = mainMouse.currentXY.x;
    currentCellY = Y_OFFSET - mainMouse.currentXY.y;
  
    ASSERT( ((currentCellX >= X_MIN) && (currentCellX < X_MAX)),ASSERT_ID_5 );
    ASSERT( ((currentCellY >= Y_MIN) && (currentCellY < Y_MAX)),ASSERT_ID_6 );
    
	#ifndef AVR_STUDIO
    if (stepByStep) 
    { 
      printf("===============Current Position: x = %d   y = %d====================\n", mainMouse.currentXY.x, mainMouse.currentXY.y);
      PDMapPrinter_outputMap(&mainMouse.internalMap);
      printf("Hit enter to continue...\n");       
      scanf("%c", &moveKey);
    }

    #endif

    // check if we have reached the center aka the destination
    for (destCellsIndex = 0; destCellsIndex < numDestCells; destCellsIndex++)
    {
      if ( mainMouse.internalMap.mapCells[currentCellY][currentCellX].cellID == destCellIDs[destCellsIndex]) 
      { centerReached = REACHED; }
    }
    
	addFrontWall();  	// add a front wall if it wasn't read in before    
    floodFill(&mainMouse.internalMap, destCellIDs, numDestCells, stepByStep);
    
    // values to reset after each round
    lowestFFValue = mainMouse.internalMap.mapCells[currentCellY][currentCellX].floodFillValue;   
    nextCellDir = NO_DIR;
    
    // function fireSensors() looks at one cell ahead, existWall() looks in current cell
    currentCellWalls = existWall(&mainMouse.internalMap.mapCells[currentCellY][currentCellX], '\0', mainMouse.internalMap.vertWalls, mainMouse.internalMap.horizWalls);
    
    // decide which direction to go to next i.e. choose a direction with no walls
    if ( (currentCellWalls & NORTH_WALL) == NO_WALL)
    {
      if (mainMouse.internalMap.mapCells[currentCellY + NORTH_OFFSET][currentCellX].floodFillValue < lowestFFValue)
      {
        nextCellDir = NORTH_DIR;
        lowestFFValue = mainMouse.internalMap.mapCells[currentCellY + NORTH_OFFSET][currentCellX].floodFillValue;
      }
    }
    
    if ( (currentCellWalls & SOUTH_WALL) == NO_WALL)
    {
      if (mainMouse.internalMap.mapCells[currentCellY + SOUTH_OFFSET][currentCellX].floodFillValue < lowestFFValue)
      {
        nextCellDir = SOUTH_DIR;
        lowestFFValue = mainMouse.internalMap.mapCells[currentCellY + SOUTH_OFFSET][currentCellX].floodFillValue;
      }
    }
    
    if ( (currentCellWalls & EAST_WALL) == NO_WALL)
    {
      if (mainMouse.internalMap.mapCells[currentCellY][currentCellX + EAST_OFFSET].floodFillValue < lowestFFValue)
      {
        nextCellDir = EAST_DIR;
        lowestFFValue = mainMouse.internalMap.mapCells[currentCellY][currentCellX + EAST_OFFSET].floodFillValue;
      }
    }
    
    if ( (currentCellWalls & WEST_WALL) == NO_WALL)
    {
      if (mainMouse.internalMap.mapCells[currentCellY][currentCellX + WEST_OFFSET].floodFillValue < lowestFFValue)
      {
        nextCellDir = WEST_DIR;
        lowestFFValue = mainMouse.internalMap.mapCells[currentCellY][currentCellX + WEST_OFFSET].floodFillValue;
      }
    }
    
    // makes sure the function pointers are not null and are all in the correct set
    ASSERT(( 
            ((mainMouse.moveNorth == n_moveNorth) && (mainMouse.moveSouth == n_moveSouth) && 
             (mainMouse.moveEast == n_moveEast) && (mainMouse.moveWest == n_moveWest))   
            ||
              ((mainMouse.moveNorth == s_moveNorth) && (mainMouse.moveSouth == s_moveSouth) && 
               (mainMouse.moveEast == s_moveEast) && (mainMouse.moveWest == s_moveWest))
            ||
              ((mainMouse.moveNorth == e_moveNorth) && (mainMouse.moveSouth == e_moveSouth) && 
               (mainMouse.moveEast == e_moveEast) && (mainMouse.moveWest == e_moveWest))   
            ||
              ((mainMouse.moveNorth == w_moveNorth) && (mainMouse.moveSouth == w_moveSouth) && 
               (mainMouse.moveEast == w_moveEast) && (mainMouse.moveWest == w_moveWest))   
            ||
              ((mainMouse.moveNorth == gen_move) && (mainMouse.moveSouth == gen_move) && 
               (mainMouse.moveEast == gen_move) && (mainMouse.moveWest == gen_move))),ASSERT_ID_7
      );
     
    // turn in the correct direction
    if      (nextCellDir == NORTH_DIR) { mainMouse.currentXY.y += MOVE_NORTH; numMoves++; nextMoveSteps = (*mainMouse.moveNorth) (destCellIDs, numDestCells, CONT_RUN); ffStuck = FF_NOT_STUCK; }
    else if (nextCellDir == SOUTH_DIR) { mainMouse.currentXY.y += MOVE_SOUTH; numMoves++; nextMoveSteps = (*mainMouse.moveSouth) (destCellIDs, numDestCells, CONT_RUN); ffStuck = FF_NOT_STUCK; }
    else if (nextCellDir == EAST_DIR)  { mainMouse.currentXY.x += MOVE_EAST; numMoves++; nextMoveSteps = (*mainMouse.moveEast) (destCellIDs, numDestCells, CONT_RUN); ffStuck = FF_NOT_STUCK; }
    else if (nextCellDir == WEST_DIR)  { mainMouse.currentXY.x += MOVE_WEST; numMoves++; nextMoveSteps = (*mainMouse.moveWest) (destCellIDs, numDestCells, CONT_RUN); ffStuck = FF_NOT_STUCK; }
    else                               { floodFill(&mainMouse.internalMap, destCellIDs, numDestCells, stepByStep); ffStuck++; }    
    
    if (ffStuck >= FF_STUCK)
    {
      _CLI();
      LED_PORT = ALL_HIGH;
      LED_PORT = mainMouse.currentXY.y;
	 // LED_PORT = 0xDF;
     // LED_PORT &= FF_STUCK_ERROR_1;
      
      printf("Floodfill Stuck....\n");
      exit(-1);
    }
    
    // makes mouse move until floodfill values become out of date or there's a front wall
    if (nextCellDir != NO_DIR)
    {
      PDLLMotorLogic_moveForward(nextMoveSteps);
      PDLLMotorLogic_commenceMovt();
	  mainMouse.frontWallFound = NO_WALL;		// just in case it was toggled from the last movement
    }
  }
  
  return numMoves;

} // akinaRun()
