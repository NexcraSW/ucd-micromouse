/*
 *  PDMapCell.h
 *  MazeSimulator
 *
 *  Created by Paul De Leon on 10/24/05.
 *  Copyright 2005 Project Densho. All rights reserved.
 *
 */

#include "PDConfig.h"

#ifndef PDMAPCELL_H
    #define PDMAPCELL_H
    
#define initCellArgs                    struct PDMapCell*, PDInt, PDInt
#define setFloodFillValueArgs           struct PDMapCell*, PDInt
#define existWallArgs                   struct PDMapCell*, char, char*, char*

typedef struct PDMapCell
{
  PDMapCellCoordinate cellXY;
  PDInt floodFillValue;             // we could get away with making this a char!
  PDUShort cellID;
  PDUShort cellSetIndex;
  PDUShort cellSetPosition;
  // char cellStatus;

} PDMapCell;

void initCell (initCellArgs);
void setFloodFillValue(struct PDMapCell*, PDInt);
void setCellStatus(struct PDMapCell*, PDInt);

PDMapCellWalls existWall(existWallArgs);
    
#endif
