/*
 *  PDMapCell.c
 *  MazeSimulator
 *
 *  Created by Paul De Leon on 10/24/05.
 *  Copyright 2005 Project Densho. All rights reserved.
 *
 */

#include "PDMapCell.h"

//-----------------------------------------------------------------------------------------
// initialize the cell's XY and ID

void initCell (struct PDMapCell *inMapCell, PDInt inX, PDInt inY)
{
  ASSERT((inMapCell != NULL),ASSERT_ID_18);
  ASSERT((inX >= 0 && inX < X_MAX),ASSERT_ID_19);
  ASSERT((inY >= 0 && inY < Y_MAX),ASSERT_ID_20);
  
  inMapCell->floodFillValue = UNDEF_FF_VALUE;
  inMapCell->cellID = (Y_DIM * inX) + (Y_OFFSET - inY);
  
  //inMapCell->cellStatus = '\0';       // might not need cellStatus
  
  inMapCell->cellSetIndex = inMapCell->cellID / BITS_PER_CELLSET;
  inMapCell->cellSetPosition = inMapCell->cellID % BITS_PER_CELLSET;
  
  setX(&inMapCell->cellXY, inX);
  setY(&inMapCell->cellXY, Y_OFFSET - inY);
  
} // initCell()

//-------------------------------------------------------------------------------------------
// checks if there is a wall in a certain direction - probably don't need to specify directions for now
// row and col may not be needed if the cell has it set already
// dir is probably not needed anymore
// will be changed to accomodate sensors...please remember that floodfill() may need the old version

PDMapCellWalls existWall(struct PDMapCell *inMapCell, char dir, char *vertWalls, char *horizWalls)
{
  PDInt wallArrayIndex = 0;         // this accesses the walls arrays
  
  PDInt row = 0;
  PDInt col = 0;
  
  PDInt northWallBitNum = 0;         // these are the bit numbers of the walls - used for shifts and indexing the arrays
  PDInt southWallBitNum = 0;
  PDInt eastWallBitNum = 0;
  PDInt westWallBitNum = 0;
    
  PDMapCellWalls cellWalls = '\0';
    
  ASSERT((inMapCell != NULL),ASSERT_ID_21);  
  ASSERT( ((vertWalls != NULL) && (horizWalls != NULL)),ASSERT_ID_22 );
  
  row = inMapCell->cellXY.y;
  col = inMapCell->cellXY.x;
    
  northWallBitNum = (X_DIM * row) + col;
  southWallBitNum = (X_DIM * (row - 1) ) + col;
  
  eastWallBitNum =  (Y_DIM * col) + row;
  westWallBitNum = (Y_DIM * (col - 1) ) + row;
  
  // checking all four sides of the cell
  
  // north
  wallArrayIndex = northWallBitNum / BITS_PER_CELLSET;
  if ( (row == (Y_DIM - 1) ) || (horizWalls[wallArrayIndex] & (1 << (northWallBitNum % BITS_PER_CELLSET) ) ) ) { cellWalls |= NORTH_WALL; }

  // south
  wallArrayIndex = southWallBitNum / BITS_PER_CELLSET;
  if ( (row == 0) || (horizWalls[wallArrayIndex] & (1 << (southWallBitNum % BITS_PER_CELLSET) ) ) ) { cellWalls |= SOUTH_WALL; }
  
  // east 
  wallArrayIndex = eastWallBitNum / BITS_PER_CELLSET;
  if ( (col == (X_DIM - 1) ) || (vertWalls[wallArrayIndex] & (1 << (eastWallBitNum % BITS_PER_CELLSET) ) ) ) { cellWalls |= EAST_WALL; }

  // west 
  wallArrayIndex = westWallBitNum / BITS_PER_CELLSET;
  if ( (col == 0) || (vertWalls[wallArrayIndex] & (1 << (westWallBitNum % BITS_PER_CELLSET) ) ) ) { cellWalls |= WEST_WALL; }

  return cellWalls;                                    // cellWalls => 0b 0000 NSEW

} // existWall()

//-------------------------------------------------------------------------------------------
// sets the floodFillValue of the cell 

void setFloodFillValue(struct PDMapCell *inMapCell, PDInt inFloodFillValue)
{
  ASSERT( ((inMapCell != NULL)),ASSERT_ID_23 );
  ASSERT( ((inFloodFillValue >= FLOOD_FILL_VALUE_MIN) && (inFloodFillValue < FLOOD_FILL_VALUE_MAX)),ASSERT_ID_24 );
  
  inMapCell->floodFillValue = inFloodFillValue;

} // setFloodFillValue() 

//-------------------------------------------------------------------------------------------
// set the status of a cell 

void setCellStatus(struct PDMapCell *inCell, PDInt inStatus)
{
  ASSERT((inCell != NULL),ASSERT_ID_2); // assert fires here...
  ASSERT( ((inStatus == NOT_VISITED) || (inStatus == VISITED) ||
          (inStatus == SW_REGION) || (inStatus == NW_REGION) || (inStatus == NE_REGION) || (inStatus == SE_REGION) ||
          (inStatus == HAS_VERTWALL) || (inStatus == HAS_HORIZWALL)),ASSERT_ID_26 );
          
 // inCell->cellStatus |= inStatus;   // might not need cellStatus

} // setCellStatus() 
