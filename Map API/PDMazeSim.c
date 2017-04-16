/*
 *  MazeSim.c
 *  MazeSimulator
 *
 *  Created by Paul De Leon on 10/22/05.
 *  Copyright 2005 Project Densho. All rights reserved.
 *
 */
 
#include "PDMazeSim.h"

#ifndef AVR_STUDIO 
extern char softMouseSymbol[SOFT_MOUSE_SYMBOL_LEN];
extern PDMap solnMap;
#endif

extern PDSoftMouse mainMouse;

//******************************AVR Studio***********************************

#ifdef AVR_STUDIO

//------------------------------------------------------------------------------------------
// runs the MappingLogic, StepperLogic, and SensorLogic all at the same time

 void runAll()
 { 
    long waitCounter = 0;
    PORTA = 0xC0;          //LEDs indicate that the mouse is ready for launch
    for (waitCounter = 0; waitCounter < MENU_WAIT_TIME; waitCounter++);   //REPLACE WITH WAIT STATEMENT WHEN POSSIBLE


    #ifdef ENABLE_MENU
    while ( (PINA & 0x80) );
	#endif  
	
	PDLLinitSensors();  

	PORTA = 0xFF;

   PDInt destCellIDs[NUM_CENTER_CELLS] = CENTER_CELL_IDS;
   PDInt startCellIDs[MIN_NUM_DEST_CELLS] = {START_CELL_ID};

    //createSolnMaze();                         // set up our fake maze

   floodFill(&mainMouse.internalMap, destCellIDs, NUM_CENTER_CELLS, STEP_BY_STEP);      // perhaps move this into straightRun
  
   straightRun(&mainMouse, destCellIDs, NUM_CENTER_CELLS, STEP_BY_STEP);                // go to center
   straightRun(&mainMouse, startCellIDs, MIN_NUM_DEST_CELLS, STEP_BY_STEP);             // comeback

 } // runAll()

//------------------------------------------------------------------------------------------
// runs the MappingLogic, StepperLogic, and SensorLogic all at the same time - in AKINA MODE...
 
 void runAkina()
 {
     long waitCounter = 0;
    PORTA = 0xC0;          //LEDs indicate that the mouse is ready for launch
    for (waitCounter = 0; waitCounter < MENU_WAIT_TIME; waitCounter++);   //REPLACE WITH WAIT STATEMENT WHEN POSSIBLE


    #ifdef ENABLE_MENU
    while ( (PINA & 0x80) );
	#endif  
	
	PDLLinitSensors();  

	PORTA = 0xFF;

   PDInt destCellIDs[NUM_CENTER_CELLS] = CENTER_CELL_IDS;
   PDInt startCellIDs[MIN_NUM_DEST_CELLS] = {START_CELL_ID};

    //createSolnMaze();                         // set up our fake maze

   floodFill(&mainMouse.internalMap, destCellIDs, NUM_CENTER_CELLS, STEP_BY_STEP);      // perhaps move this into straightRun
  
   akinaRun(destCellIDs, NUM_CENTER_CELLS, STEP_BY_STEP);
   akinaRun(startCellIDs, MIN_NUM_DEST_CELLS, STEP_BY_STEP);
 
 } // runAkina()

//------------------------------------------------------------------------------------------
// until we get sensors, hard code a soln maze here

void createSolnMaze()
{   
  #ifdef PDMAZE_3X3
  mainMouse.internalMap.vertWalls[FIRST_BYTE] = 19;
  mainMouse.internalMap.horizWalls[FIRST_BYTE] = 16;
  #endif
  
  // the 83 Japan World Championship Maze...
  #ifdef PDMAZE_16X16
  char vSolnWalls[TOTAL_WALL_SIZE_V_IN_BYTES] = {-13, -49, -2, 127, 120, 31, 112, 15, 96, 7, 96, 3, 0, 1, 2, 12, 
                                                -128, 1, -64, 3, 96, 39, -16, 13, -4, 31, -68, 63, -20, 127, 0}; // db
                                                
  char hSolnWalls[TOTAL_WALL_SIZE_H_IN_BYTES] = {-20, 127, -4, 126, -6, 31, -16, 14, -16, 7, -64, 2, -128, 1, 0, 0, 
                                                 -128, 1, -64, 3, -32, 7, -16, 31, -6, 27, 124, 55, -4, 109, 0}; // db
 
  int i = TOTAL_WALL_SIZE_V_IN_BYTES; // db
  int j = TOTAL_WALL_SIZE_H_IN_BYTES; // db

  for (i = 0; i < TOTAL_WALL_SIZE_V_IN_BYTES; i++)
  { mainMouse.internalMap.vertWalls[i] = vSolnWalls[i]; } // db
  
  for (j = 0; j < TOTAL_WALL_SIZE_H_IN_BYTES; j++)
  { mainMouse.internalMap.horizWalls[j] = hSolnWalls[j]; } // db
    
  #endif

} // createSolnMaze()

//******************************General IDE***********************************

#else

//------------------------------------------------------------------------------------------
// runs the MapAPI without any stepper logic
 
void runMapAPI(const int numArgs, const char *mazeName)
{
  char userAns = '\0';
  
  ASSERT( ((NUM_CENTER_CELLS >= MIN_NUM_DEST_CELLS) && (NUM_CENTER_CELLS <= TOTAL_NUM_CELLS)),ASSERT_ID_8 );
  
  printf("\n===============Welcome to Project Densho Map API!=====================\n");
         
  while (userAns != 'q')
  {
    userAns = displayMainMenu();

    switch(userAns)
    {
      case '1':
        outputSolutionMaze(&solnMap);
        break;
      
      case '2':
        PDMapPrinter_outputMapFromWalls(&solnMap);
        break;
      case '3':
        runMouse(&mainMouse, &solnMap, NOT_STEP_BY_STEP);
        break;
      
      case '4':
        runMouse(&mainMouse, &solnMap, STEP_BY_STEP);      
        break;
      
      case 'q':
        printf("Bye-bye!\n");
        break;
      
      default:
        printf("\nSorry, that's not a valid choice\n\n");
        break;
    }
  }
  
} // runMapAPI() 

//-------------------------------------------------------------------------------------------
// setup the solution maze

void solnMazeSetup(PDMap *mapSoln, PDInt numArgs, const char *mapSolnName)
{
  char dumChar = '\0';

  if ( (mapSolnName == NULL) || (numArgs != NUM_ARGS) )
  { 
    printf("usage: ProjectDensho.out [ASCII_Maze_File]\n"); 
    exit(EXIT_FAILURE);
  }
     
  initMap(mapSoln);
  PDMapPrinter_readMap(mapSoln, mapSolnName);
    
  printf("Maze:\"%s\" was read in successfully!\nHit enter to continue...\n\n", mapSolnName);
  scanf("%c", &dumChar);
  
} // solnMazeSetup()

//-------------------------------------------------------------------------------------------
// output the completed floodfill maze 

void outputSolutionMaze(PDMap *inMap)
{
  PDInt destCellIDs[NUM_CENTER_CELLS] = CENTER_CELL_IDS;

  printf("====================optimal solution from Solution Map=============================\n");
  
  floodFill(inMap, destCellIDs, NUM_CENTER_CELLS, NOT_STEP_BY_STEP);  
  PDMapPrinter_outputMap(inMap);
  
} // outputSolutionMaze()

//-------------------------------------------------------------------------------------------
// run the mouse through the maze represented by inMap

void runMouse(PDSoftMouse *inMouse, PDMap *inMap, char stepByStep)
{
  PDInt destCellIDs[NUM_CENTER_CELLS] = CENTER_CELL_IDS;
  PDInt startCellIDs[MIN_NUM_DEST_CELLS] = {START_CELL_ID};
  PDInt numSteps = 0;
  
  char userAns = '\0';
  
  // will have to use function pointers to choose correct exploration algo later

  floodFill(&inMouse->internalMap, destCellIDs, NUM_CENTER_CELLS, stepByStep);
  
  printf("============================Starting Configuration===========================\n");
  PDMapPrinter_outputMap(&inMouse->internalMap);
  printf("The mouse currently looks like this:  %s\n\n", softMouseSymbol);
  
  while (userAns != 'q')
  {
    printf("----Please control the mouse---\n");
    printf("1. Go to the center\n");
    printf("2. Go back to start\n\n");
    
    printf("q. Take mouse out of maze and leave\n\n");
    
    printf("Your choice >>> ");
    scanf(" %c", &userAns);
    
    switch (userAns)
    {
      case '1':
        printf("===================Destination: Center======================\n");
        numSteps = straightRun(inMouse, destCellIDs, NUM_CENTER_CELLS, stepByStep);
        break;
        
      case '2':
        printf("===================Destination: Start Cell======================\n");
        numSteps = straightRun(inMouse, startCellIDs, MIN_NUM_DEST_CELLS, stepByStep);
        break;

      case 'q':
        printf("Thanks and don't forget your mouse!\n\n");
        break;
        
      default:
        printf("\nSorry, that's not a valid choice\n\n");
        break;
    }
    
    PDMapPrinter_outputMap(&inMouse->internalMap);
    printf("The mouse took %d moves to get to it's destination cell\n\n", numSteps);
  }

  initMap(&inMouse->internalMap);          // reset for next time
  
} // runMouse()

//-------------------------------------------------------------------------------------------
// display the menu 

char displayMainMenu(void)
{
  char userAns = '\0';

  printf("----------Please choose one:----------\n");
  
  printf("1. Output solution maze\n");
  printf("2. Output single dash maze based on wall arrays\n");
  printf("3. Run mouse through maze quickly\n");
  printf("4. Run mouse through maze step-by-step\n\n");
  
  printf("q. Exit ProjectDensho map API\n\n");
  
  printf("Your Choice >>> ");
  scanf(" %c", &userAns);
  
  return userAns;

} // displayMainMenu() 

#endif
