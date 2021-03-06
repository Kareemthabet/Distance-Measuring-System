/******************************************************************************
 *
 * Module: ULTRASONIC
 *
 * File Name: ultrasonic.c
 *
 * Description: Header file for the AVR ULTRASONIC driver
 *
 * Author: Kareem Ahmed Thabet
 *
 *******************************************************************************/
#include"icu.h"
#include"ultrasonic.h"
#include"gpio.h"
#include"common_macros.h"
#include<util/delay.h>

/*******************************************************************************
 *                         Global Variables                                    *
 *******************************************************************************/
uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;

/*******************************************************************************
 *                         Functions Definitions                               *
 *******************************************************************************/
/*
 * Description:
 * Initialize the ICU driver as required.
 * Setup the ICU call back function.
 * Setup the direction for the trigger pin as output pin through the GPIO driver
 */
void ULTRASONIC_init( void )
{
	/* Determine ICU Configuration parameters */
	ICU_ConfigType Config = {RISING,F_CPU_CLOCK};

	/* Intialize ICU driver */
	ICU_init( &Config );

	/* Set the Call back function pointer in the ICU driver */
	ICU_setCallBackFunction( ULTRASONIC_edgeProcessing );

	/* Setup the direction for the trigger pin as output pin through the GPIO driver */
	GPIO_setupPinDirection(PORTD_ID,PIN4_ID,PIN_OUTPUT);
}

/*
 * Description: Send the Trigger pulse to the ultrasonic
 */
 void ULTRASONIC_Trigger( void )
{
	 /* write 1 to TRIGGER pin */
	 GPIO_writePin(TRIGGER_PORT_ID,TRIGGER_PIN_ID,LOGIC_HIGH);

	/* delay 10 us */
	_delay_us(10);

	/* write 0 to TRIGGER pin */
	GPIO_writePin(TRIGGER_PORT_ID,TRIGGER_PIN_ID,LOGIC_LOW);
}

 /*
  * Description
  * Send the trigger pulse by using Ultrasonic_Trigger function.
  * Start the measurements by the ICU from this moment.
  */
uint16 ULTRASONIC_readDistance(void)
{
	 ULTRASONIC_Trigger();
	 uint16 distance = ( g_timeHigh / 58.8 );
	 return distance;
}

/*
 * Description:
 * This is the call back function called by the ICU driver.
 *  This is used to calculate the high time (pulse time) generated by the ultrasonic sensor.
 */
void ULTRASONIC_edgeProcessing(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		ICU_setEdge(FALLING);
		ICU_clearTimer();
	}
	else if(g_edgeCount == 2)
	{
		g_timeHigh = ICU_getInputCaptureValue();
		ICU_clearTimer();
		ICU_setEdge(RISING);
		g_edgeCount = 0;
	}
}

