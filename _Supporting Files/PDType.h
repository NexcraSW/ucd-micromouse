/*
 *  PDType.h
 *  MazeSimulator
 *
 *  Created by Paul De Leon on 11/26/05.
 *  Copyright 2005 ProjectDensho. All rights reserved.
 *
 */

// will probably get combined with PDLLType.h

#ifndef PDTYPE_H
    #define PDTYPE_H

  #ifdef AVR_STUDIO
    typedef char PDShort;           // 8 bits - signed
    typedef unsigned char PDUShort; // 8 bits - unsigned
    typedef int PDInt;              // 16 bits
    typedef long PDLong;            // 32 bits
  #else
    typedef char PDShort;           // 8 bits - signed
    typedef unsigned char PDUShort; // 8 bits - unsigned  
    typedef short PDInt;            // 16 bits
    typedef int PDLong;             // 32 bits
  #endif
  
  typedef char PDMapCellWalls;
  typedef char PDDirection;                // points the mouse
  typedef char PDErrType;
  typedef char PDFlag;

#endif
