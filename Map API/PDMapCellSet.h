/*
 *  PDMapCellSet.h
 *  MazeSimulator
 *
 *  Created by Paul De Leon on 11/26/05.
 *  Copyright 2005 ProjectDensho. All rights reserved.
 *
 */

#include "PDConfig.h"

#ifndef PDMAPCELLSET_H
    #define PDMAPCELLSET_H
    
typedef struct PDMapCellSet
{
  char mapCellSet[TOTAL_NUM_CELLS_IN_BYTES];

} PDMapCellSet;

void initCellSet(struct PDMapCellSet*);  
void addCell(struct PDMapCellSet*, PDInt);
void removeCell(struct PDMapCellSet*, PDInt);
char findCell(struct PDMapCellSet*, PDInt);
char isEmpty(struct PDMapCellSet*);
PDInt nextCell(struct PDMapCellSet*);

#endif
