/*
 *  PDLLType.h
 *  
 *
 *  Created by Paul De Leon on 1/12/06.
 *  Copyright 2006 ProjectDensho. All rights reserved.
 *
 */
 
#include "PDLLConst.h" 
 
#ifndef PDLLTYPE_H
	#define PDLLTYPE_H
	
  #ifdef AVR_STUDIO   
    typedef int PDLLInt;
    typedef long PDLLLong;
  #else 
    typedef short PDLLInt;
    typedef int PDLLLong;
  #endif

  typedef char PDLLDir;
  typedef char PDLLFlag;

#endif
