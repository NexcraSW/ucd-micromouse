/*
 *  PDMapCellCoordinate.c
 *  MazeSimulator
 *
 *  Created by Paul De Leon on 11/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "PDMapCellCoordinate.h"

//-------------------------------------------------------------------------
// sets the X value, checks in in db'ing mode

void setX(struct PDMapCellCoordinate *inCellCoordinate, PDInt inX)
{
  ASSERT( ((inX >= X_MIN) && (inX < X_MAX)),ASSERT_ID_16 );
  inCellCoordinate->x = inX;
  
} // setX()

//-------------------------------------------------------------------------
// sets the Y value, checks in in db'ing mode

void setY(struct PDMapCellCoordinate *inCellCoordinate, PDInt inY)
{
  ASSERT( ((inY >= Y_MIN) && (inY < Y_MAX)),ASSERT_ID_17);
  inCellCoordinate->y = inY;

} // setY() 
