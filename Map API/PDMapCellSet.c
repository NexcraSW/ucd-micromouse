/*
 *  PDMapCellSet.c
 *  MazeSimulator
 *
 *  Created by Paul De Leon on 11/26/05.
 *  Copyright 2005 ProjectDensho. All rights reserved.
 *
 */

#include "PDMapCellSet.h"

//-------------------------------------------------------------------------
// zero out the cell set

void initCellSet(struct PDMapCellSet *inCellSet)
{
  PDInt i = 0;
  
  ASSERT( ((inCellSet != NULL)),ASSERT_ID_27 );
  
  for (i = 0; i < TOTAL_NUM_CELLS_IN_BYTES; i++)
  {
    inCellSet->mapCellSet[i] = '\0';
  }

} // initCellSet()

//-------------------------------------------------------------------------
// add a cell to the inCellSet

void addCell(struct PDMapCellSet *inCellSet, PDInt cellID)
{
  PDInt byteIndex = cellID / 8;
  PDInt bitPosition = cellID % 8;
  
  ASSERT( ((inCellSet != NULL)),ASSERT_ID_28 );
  ASSERT( (( (cellID >= 0) && (cellID < TOTAL_NUM_CELLS) )),ASSERT_ID_29 );

    
  inCellSet->mapCellSet[byteIndex] = inCellSet->mapCellSet[byteIndex] | (1 << bitPosition);

} // addCell()

//-------------------------------------------------------------------------
// remove a cell from the inCellSet

void removeCell(struct PDMapCellSet *inCellSet, PDInt cellID)
{
  PDInt byteIndex = cellID / 8;
  PDInt bitPosition = cellID % 8;
  
  ASSERT( ((inCellSet != NULL)),ASSERT_ID_30 );
  ASSERT( (( (cellID >= 0) && (cellID < TOTAL_NUM_CELLS) )),ASSERT_ID_31 );

  inCellSet->mapCellSet[byteIndex] = inCellSet->mapCellSet[byteIndex] & ~(1 << bitPosition);

} // removeCell()

//-------------------------------------------------------------------------
// find a cell from the inCellSet

char findCell(struct PDMapCellSet *inCellSet, PDInt cellID)
{
  PDInt byteIndex = cellID / 8;
  PDInt bitPosition = cellID % 8;
  char mask;

  ASSERT( ((inCellSet != NULL)),ASSERT_ID_0 );
  ASSERT( (( (cellID >= 0) && (cellID < TOTAL_NUM_CELLS) )),ASSERT_ID_1 );
  
  mask = inCellSet->mapCellSet[byteIndex];
  
  mask = mask | (1 << bitPosition);
  mask = (mask) ^ (inCellSet->mapCellSet[byteIndex]);       // do you mean '&'?
  
  if (mask == '\0')
  {
    return CELL_FOUND;
  }
  
  return CELL_NOT_FOUND;
  
} // findCell()

//------------------------------------------------------------------------------------
// indicates whether the cell set is empty or not

char isEmpty(struct PDMapCellSet *inCellSet)
{
  PDInt i = 0;
  char testCellSet = '\0';
  
  ASSERT((inCellSet != NULL),ASSERT_ID_2);
  
  for (i = 0; i < TOTAL_NUM_CELLS_IN_BYTES; i++) { testCellSet |= inCellSet->mapCellSet[i]; }
  
  if (testCellSet == '\0') { return CELLSET_EMPTY; }
    
  return CELLSET_NOT_EMPTY;
  
} // isEmpty()

//-------------------------------------------------------------------------------------
// returns lowest numbered cell in the cell set then removes it from the cell set

PDInt nextCell(struct PDMapCellSet *inCellSet)
{
  PDInt i = 0;
  PDInt bitPos = 0;
  PDInt cellID = 0;
  
  ASSERT((inCellSet != NULL),ASSERT_ID_3);
  
  for (i = 0; i < TOTAL_NUM_CELLS_IN_BYTES; i++)
  {
    if (inCellSet->mapCellSet[i] != '\0')                       // there's a cell here
    {
      ASSERT( ((bitPos >= 0) && (bitPos < BITS_PER_CELLSET)),ASSERT_ID_4 );
      
      while (bitPos < BITS_PER_CELLSET)
      {
        if (inCellSet->mapCellSet[i] & (1 << bitPos) )         // found lowest cell
        {
          cellID = bitPos + (BITS_PER_CELLSET * i);
          removeCell(inCellSet, cellID);
          
          return cellID;
        }
        
        bitPos++;
      }
    }
  }
  
  return CELLSET_EMPTY;
  
} // nextCell()

