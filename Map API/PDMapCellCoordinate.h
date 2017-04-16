/*
 *  PDMapCellCoordinate.h
 *  MazeSimulator
 *
 *  Created by Paul De Leon on 11/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "PDConfig.h"

#ifndef PDMAPCELLCOORDINATE_H
    #define PDMAPCELLCOORDINATE_H

typedef struct PDMapCellCoordinate
{
  PDUShort x;
  PDUShort y;
  
} PDMapCellCoordinate;

void setX(struct PDMapCellCoordinate*, PDInt);
void setY(struct PDMapCellCoordinate*, PDInt);
    
#endif
