/*
 *  PDLLConst.h
 *  
 *
 *  Created by Mark Serrano on 1/14/06.
 *  Copyright 2006 ProjectDensho. All rights reserved.
 *
 */

#ifndef PDLLCONST_H
    #define PDLLCONST_H

//------------Settings for the MotorLogic simulation----------------

//---The prescaled frequency of the MCU---

// matches the example
#define BASE_FREQ       8192

//---motor characteristics - will need to experiment to find the best values for these---

// init values
#define INIT_FREQ       0
#define INIT_SUM        0                /* Don't have to split this between V and a for now */

#define INIT_DISP       0
#define INIT_VELOC      0
#define INIT_ACCEL      0
 
// default values
#define DEFAULT_ACCEL   10  // 100
#define DEFAULT_VELOC   475//600//475//400 //300 // 150
#define CRAWL_VELOC     270 //270//220
#define SLOW_DOWN_VELOC 80 //50
#define RESET_SUM       0

#define RESET_NUM_USER_STEPS -1
#define NO_DISP        0
#define NO_CELL_DISP   0

/* calibrate */
#define ANG_ER_CRT_DISP 1   // 15
#define LIN_ER_CRT_DISP 25   //30 //35  //40  // 80
#define ANG_RIB_ER_DISP 4 //2  //14
#define ANG_RIB_CLEAN 8    //12

#define ONE_CELL_DISP   344 //343 //340    // 340
#define HALF_CELL_DISP  175  //173    //172   //170
#define QTR_CELL_DISP   90    //91 //90   //89    

#define QTR_TURN_DISP   148//150   // 140
#define HALF_TURN_DISP  300   // 293      // <---- this is the real one
 //#define HALF_TURN_DISP  285              // <---- for speed runs


// min-max values

#define MIN_SUM      -1
#define MIN_DISP     -100000
#define MIN_VELOC    -2000
#define MIN_ACCEL    -500

#define MAX_DISP     100000
#define MAX_VELOC    2000   //2000   
#define MAX_ACCEL    100 //70//100//600 //250    

// freq values
#define FREQ_ACCEL   100            /* This value is determined experimentally based on how often we really need to adjust the acceleration */

// misc values

#define DEST_REACHED   0

#define NO_ACCEL       0
#define NO_VELOC       0

 //350 //50       /* set at something even lower to test feasibility of deceleration algo */
#define STOPPED_VELOC  0

#define PDLLFLAG_T    'T'
#define PDLLFLAG_F    'F'

#define DISP_DIR_POS  'P'
#define DISP_DIR_NEG  'N'

//---pin constants---
#define ALL_INPUT_PINS	0xFF
#define ALL_HIGH		0xFF

#define PIN_LOW         0
#define PIN_HIGH        1

// handle signed quantities
#define POS             1
#define NEG             0

// Motor Settings
#define MOTOR_PORT_L    PORTE
#define MOTOR_DDR_L     DDRE

#define MOTOR_PORT_R    PORTE
#define MOTOR_DDR_R     DDRE

#define MOTOR_PIN_STEP_L   PORTE4
#define MOTOR_PIN_DIR_L    PORTE5

#define MOTOR_PIN_STEP_R   PORTE6
#define MOTOR_PIN_DIR_R    PORTE7

#define PIN_DIR_BACKWARD_L   1
#define PIN_DIR_FORWARD_L    1

#define PIN_DIR_BACKWARD_R   1
#define PIN_DIR_FORWARD_R    1

//Directions for stepper motor
#define L_FWD_STEP      0xCF
#define R_FWD_STEP      0x08      
#define L_REV_STEP      0x20
#define R_REV_STEP      0xF7

//Motor Enable
#define DISABLE_LEFT_MOTOR     0x20
#define DISABLE_RIGHT_MOTOR    0x04 
#define ENABLE_LEFT_MOTOR      0xDF
#define ENABLE_RIGHT_MOTOR     0xFB
#define LEFT_MOTOR             1
#define RIGHT_MOTOR            0

//--------------------- SENSOR CALIBRATION CONSTANTS ----------------------------

// sensor result values
#define NO_WALL_FOUND 0
#define WALL_FOUND    1


  // SENSOR IDS
#define LEFT_SENSOR    0      
#define CENTER_SENSOR  1
#define RIGHT_SENSOR   2

//first sensor

#define START_SENSOR LEFT_SENSOR
  
  //SENSOR CHANNELS - ((IF REF VOLTAGE OR LEFT ADJUST CHANGE, CHANGE THESE TOO))
  
#define ADC_CHANNEL_0   0xC0 
#define ADC_CHANNEL_1   0xC1 
#define ADC_CHANNEL_2   0xC2 
#define ADC_CHANNEL_3   0xC3 
#define ADC_CHANNEL_4   0xC4 
#define ADC_CHANNEL_5   0xC5 
#define ADC_CHANNEL_6   0xC6 
#define ADC_CHANNEL_7   0xC7 

#define LEFT_CHANNEL    ADC_CHANNEL_0
#define CENTER_CHANNEL  ADC_CHANNEL_2
#define RIGHT_CHANNEL   ADC_CHANNEL_4

  //WALL CHECK THRESHOLDS (Values are in cm)

// (error about 2cm)  set about 3cm greater than what you want 
#define  LEFT_WALL_CHECK       350//390 //410   
#define  RIGHT_WALL_CHECK      350 //360 //410
#define  FRONT_WALL_CHECK      130 //100 //120

#define  LEFT_RIB_CHECK      310 //270//200//270 // 220  //250 //270  //360  //500 
#define  RIGHT_RIB_CHECK     240 //190 //130 //190//150 //170 //190 //220//240//280  //500

#define RIB_DIFF_CHECK         10 //30//70 //20 //40 //60
#define RIB_DIFF_CENTER        -122 //102 //-122 //-142

// error correction thresholds
#define CL_FORWARD_READING_MIN 1064 
#define CL_FORWARD_READING_MAX 1240

#define CL_BACK_READING_MIN    480 //480 //525 // 662
#define CL_BACK_READING_MAX    630 // 494

#define FORWARD_READING_MIN    192//195  //200 //180
#define FORWARD_READING_MAX    270 // 270

#define BACK_READING_MIN       82  //115
#define BACK_READING_MAX       145  //145   /* may need osc again */

#define SENSOR_DELAY_TIME      50

#define LEFT_WALL_READING_L_MIN  570  //630     //640 //660        //630
#define LEFT_WALL_READING_L_MAX  790        //790

#define LEFT_WALL_READING_R_MIN  0 //270 //330   //340  //360 //380 //400  //420//390   //540
#define LEFT_WALL_READING_R_MAX  540  //540   //390 

#define RIGHT_WALL_READING_L_MIN  0//310 //370 //390   //400  //420 //465     //499
#define RIGHT_WALL_READING_L_MAX 510 //499       //465

#define RIGHT_WALL_READING_R_MIN 540   //570       //580   //600  //671
#define RIGHT_WALL_READING_R_MAX 810    //755 

#define LEFT_SKEW_READING_L_MIN  570 
#define LEFT_SKEW_READING_L_MAX  790        

#define LEFT_SKEW_READING_R_MIN  0 
#define LEFT_SKEW_READING_R_MAX  530  

#define RIGHT_SKEW_READING_L_MIN  0 //370 //390   //400  //420 //465     //499
#define RIGHT_SKEW_READING_L_MAX 530 //499       //465

#define RIGHT_SKEW_READING_R_MIN 570   //570       //580   //600  //671
#define RIGHT_SKEW_READING_R_MAX 810    //755


// thesholds for ribs
#define LEFT_RIB_READING_L_MIN  580//590 //380
#define LEFT_RIB_READING_L_MAX  790 //430

#define RIGHT_RIB_READING_R_MIN 560//530
#define RIGHT_RIB_READING_R_MAX 810

// used for lining up to a front wall - sensor values
#define APPX_DIST_TO_FRONT_WALL 200
#define DIST_TO_FRONT_WALL 875 //1000

// used for lining up to a front wall - stepper values
#define APPX_DISP_TO_FRONT_WALL 180 //130
#define DISP_TO_FRONT_WALL      0

/*
#define LEFT_RIB_READING_R_MIN  280//290  
#define LEFT_RIB_READING_R_MAX  540     

#define RIGHT_RIB_READING_L_MIN 360 //330
#define RIGHT_RIB_READING_L_MAX 540
*/

//-----------------------SKEW CONTROL -------------------
#define CENTER_RANGE 60 //20 //120
#define CENTER_RANGE_OFFSET 60 //50 //50 //30

//-----------------------THRESHOLD AND BUFFER DEFINITIONS------------------------

    #define BUFFER_SIZE 40   //40
    #define SENSOR_THRESHOLD 120 //120

// LINEARIZATION DEFINITIONS

   //GPD12 
   
    //Linearization Slope
    #define M_LIN_L        13255      

    //Lin Offset
    #define B_LIN_L        -6         

    //Lin constant
    #define K_LIN_L         4         

   //GPD120 
   //Linearization Slope
    #define M_LIN_S        6218       
    
    //Lin Offset   
    #define B_LIN_S        19             
    
    //Lin constant
    #define K_LIN_S         2         
    
    #define MUL_SENSOR 1              //CHANGE TO 1 IF USING 3 SENSORS

//--------------------------Control Board Ports--------------------------------------
  #define LED_PORT  PORTA
  #define LED_DDR   DDRA

  #define LED_PIN0  PORTA0
  #define LED_PIN1  PORTA1
  #define LED_PIN2  PORTA2
  #define LED_PIN3  PORTA3
  #define LED_PIN4  PORTA4
  #define LED_PIN5  PORTA5

  #define PSH_BUTTON_PORT  PORTA
  #define PSH_BUTTON_DDR   DDRA

  #define PSH_BUTTON1_PIN  PORTA6  
  #define PSH_BUTTON2_PIN  PORTA7
  
//--------------------------Menu Constants--------------------------------------

  #define MENU_CHOICE_MAX 4
  #define MENU_WAIT_TIME 100000
  
//--------------------------Error Codes-----------------------------------------
  #define FF_STUCK_ERROR_1   0xFE

#endif
