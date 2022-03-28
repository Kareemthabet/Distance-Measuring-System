 /******************************************************************************
 *
 * Module: ICU
 *
 * File Name: icu.h
 *
 * Description: Header file for the AVR ICU driver
 *
 * Author: Kareem Ahmed Thabet
 *
 *******************************************************************************/
#ifndef ICU_H_
#define ICU_H_


/*******************************************************************************
 *                         Includes                                   *
 *******************************************************************************/

#include"std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum {
	FALLING,RISING
}ICU_EdgeType;

typedef enum{
	NO_CLOCK ,F_CPU_CLOCK, F_CPU_8, F_CPU_64, F_CPU_256, F_CPU_1024
}ICU_Clock;

typedef struct{
	ICU_EdgeType edge;
	ICU_Clock clock;
}ICU_ConfigType;


/*******************************************************************************
 *                         Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description : Function to initialize the ICU driver
 * 	1. Set the required clock.
 * 	2. Set the required edge detection.
 * 	3. Enable the Input Capture Interrupt.
 * 	4. Initialize Timer1 Registers
 */
void ICU_init( const ICU_ConfigType *ConfigPtr );


/*
 * Description: Function to set the required edge detection.
 */
void ICU_setEdge( ICU_EdgeType edge );

/*
 * Description: Function to set the Call Back function address.
 */
void ICU_setCallBackFunction( void(*a_ptr)(void) );

/*
 * Description: Function to get the Timer1 Value when the input is captured
 *              The value stored at Input Capture Register ICR1
 */
uint16 ICU_getInputCaptureValue(void);

/*
 * Description: Function to clear the Timer1 Value to start count from ZERO
 */
void ICU_clearTimer( void );


/*
 * Description: Function to disable the Timer1 to stop the ICU Driver
 */
void ICU_Deinit( void );




#endif /* ICU_H_ */
