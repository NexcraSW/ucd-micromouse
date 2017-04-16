/*
 *  ProjectDensho.c
 *  ProjectDensho
 *
 *  Created by Paul De Leon on 3/5/06.
 *  Copyright 2006 ProjectDensho. All rights reserved.
 *
 *  This in the main file which oversees the operation of the mouse whether
 *  in a General IDE or in AVR_STUDIO
 */

#include "ProjectDensho.h"

#ifndef AVR_STUDIO
char softMouseSymbol[SOFT_MOUSE_SYMBOL_LEN] = {'<', '^', '>'};      // what map printer is going to use
PDMap solnMap = INIT_PDMAP;                                         // the solution maze
PDLLInt numLeftSteps = 0;
PDLLInt numRightSteps = 0;
#endif

PDSoftMouse mainMouse = INIT_PDSOFTMOUSE;
PDLLMotorLogic mainLogic;

volatile int sensor_cycle;
PDLLSensor_buffer sensorBuffer[3];
char errorCheckingMode = NORM_ERR_CHECKING;

void (*PDErCrTable_cursor[NUM_ERROR_CONFIGS]) ();


//******************************AVR Studio***********************************

#ifdef AVR_STUDIO
//---------------------------------------------------------------------------
// the main function for running in AVR Studio

int main()
{
        
  int menuSelect = 0;
  long waitCounter = 0;
  
  _CLI();			// DON'T let interrupts happen until everything is initialized...

  PDErCrTable_init();
  PDLLPorts_initPort();                     // set the TIMER ports for interrupts
   
 
  PDLLMotorLogic_init(&mainLogic);			// this will probably be called only in the beginning of the MapAPI
  initMouse(&mainMouse);                    // set various attributes of the mouse
  PDLLinitSensors();                       // this is SUPPOSED to go last!
 
 // ASSERT( (3 ==5), ASSERT_ID_12);
  
  //_CLI();  
/*
  DDRA = 0x3F;

  PORTA = 0xFF;
*/
  
/*** Questions ***

 can we:
       * use PSH_BUTTON_PORT instead of PORTA?
       * use PSH_BUTTON_DDR instead of DDRA?
       * use <defined constant> for PINA?
       
*****************/
 
#ifdef ENABLE_MENU
             
while (1)
{     
     if( !(PINA & 0x40) )
	  { menuSelect ++;
	    menuSelect %= MENU_CHOICE_MAX;
		for (waitCounter = 0; waitCounter < MENU_WAIT_TIME; waitCounter++);     //REPLACE WITH WAIT STATEMENT WHEN POSSIBLE
	  }  

     PORTA = ~( 1 << menuSelect );
      
      switch (menuSelect)
      {
        	  case 0:
			       while ( !(PINA & 0x80) )
				   {runAll();}
				   break;
			  case 1:
			        while ( !(PINA & 0x80) )
					{runSensorLogic();}
					break;
              case 2:
			        while ( !(PINA & 0x80) )
					{runStepperLogic();}
					break;
              case 3:
                   while ( !(PINA & 0x80) )
				   {runAkina();}
				   break;
     }

}  //WHILE

 #else
 runAll();
 // runSensorLogic();
 //runStepperLogic();  

 #endif

  return 0;
  
} // main()

//******************************General IDE***********************************

#else
// since we're not in AVR Studio
DECLARE_FAKE_AVR;           

//---------------------------------------------------------------------------
// the main function for running in a general IDE

int main(int argc, char *argv[])
{
  PDInt destCellIDs[NUM_CENTER_CELLS] = CENTER_CELL_IDS;
  PDInt startCellIDs[MIN_NUM_DEST_CELLS] = {START_CELL_ID};
    
  int numArgs = argc;
  char *mazeName = argv[1];
  char userAns = '\0';
  
  _CLI();			// DON'T let interrupts happen until everything is initialized...
  
  PDErCrTable_init();
  PDLLPorts_initPort();                     // set the TIMER ports for interrupts
  PDLLMotorLogic_init(&mainLogic);			// this will probably be called only in the beginning of the MapAPI

  initMouse(&mainMouse);                    // set various attributes of the mouse
  
  printf("=============Welcome to ProjectDensho Software Simulation!=============\n");
  
  solnMazeSetup(&solnMap, numArgs, mazeName);
  
  while (userAns != 'q')
  {
    printf("\n");
    printf("Please select one:\n");
    printf("1. Run MapAPI.\n");
    printf("2. Run StepperLogic.\n");
    printf("3. Run Mouse in Exploration Mode.\n");
    printf("4. Run Mouse in Optimized Run Mode.\n\n");
    printf("q. Exit Software Simulation.\n");
    
    printf("Your choice >>> ");
    scanf(" %c", &userAns);
    
    switch (userAns)
    {
      case '1':
        runMapAPI(numArgs, mazeName);
        break;
        
      case '2':
        runStepperLogicSWSim();
        break;
        
      case '3':
        // create some sort of explorationFunction which utilizes different dest cells
        // and explores the maze in a true breadth first fashion
        floodFill(&mainMouse.internalMap, destCellIDs, NUM_CENTER_CELLS, STEP_BY_STEP);      
        straightRun(&mainMouse, destCellIDs, NUM_CENTER_CELLS, STEP_BY_STEP);         // go to center
        straightRun(&mainMouse, startCellIDs, MIN_NUM_DEST_CELLS, STEP_BY_STEP);      // comeback
        break;
      case '4':
        floodFill(&mainMouse.internalMap, destCellIDs, NUM_CENTER_CELLS, STEP_BY_STEP);
        akinaRun(destCellIDs, NUM_CENTER_CELLS, STEP_BY_STEP);
        akinaRun(startCellIDs, MIN_NUM_DEST_CELLS, STEP_BY_STEP);
      case 'q':
        printf("Thanks, please don't forget your mouse.\n\n");
        break;

      default:
        printf("Not a valid choice. Please choose again.\n\n");
        break;
    }
  }

  return 0;
  
} // main()

#endif
