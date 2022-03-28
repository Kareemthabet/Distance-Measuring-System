 /******************************************************************************
 *
 * Module: ICU
 *
 * File Name: icu.c
 *
 * Description: source file for the AVR ICU driver
 *
 * Author: Kareem Ahmed Thabet
 *
 *******************************************************************************/
#include"icu.h"
#include"gpio.h"
#include"common_macros.h" /* To use the macros like SET_BIT */
#include<avr/io.h> /* To use ICU/Timer1 Registers */
#include<avr/interrupt.h> /* For ICU ISR */

/*******************************************************************************
 *                         Global Variables                                    *
 *******************************************************************************/


/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;


/*******************************************************************************
 *                         Interrupt Service Routines                          *
 *******************************************************************************/
ISR(TIMER1_CAPT_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}


/*
 * Description : Function to initialize the ICU driver
 * 	1. Set the required clock.
 * 	2. Set the required edge detection.
 * 	3. Enable the Input Capture Interrupt.
 * 	4. Initialize Timer1 Registers
 */
void ICU_init( const ICU_ConfigType *ConfigPtr )
{
	/* set IC1 (PD6) as an output pin */
	GPIO_setupPinDirection(PORTD_ID,PIN6_ID,PIN_OUTPUT);

	/* Timer1 always operates in non-PWM Mode */
	TCCR1A |= (1<<FOC1A) | (1<<FOC1B);

	/* Insert ICES1 bit in TCCR1B register */
	TCCR1B = ( TCCR1B & 0XBF) | ( ( ConfigPtr->edge ) << 6 );

	/* Insert ICU clock bits ( CS12, CS11, CS10) in TCCR1B register */
	TCCR1B = ( TCCR1B & 0XF8) | ( ConfigPtr->clock );

	/* Reset Timer 1 counter register value */
	TCNT1 = 0;

	/* Reset ICR1 register value */
	ICR1 = 0;

	/* Enable Input capture interrupt */
	TIMSK |= (1<<TICIE1);
}

/*
 * Description: Function to set the required edge detection.
 */
void ICU_setEdge( ICU_EdgeType edge )
{
	/* Insert ICES1 bit in TCCR1B register */
	TCCR1B = ( TCCR1B & 0XBF) | ( edge  << 6 );
}

/*
 * Description: Function to set the Call Back function address.
 */
void ICU_setCallBackFunction( void(*a_ptr)(void) )
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}

/*
 * Description: Function to get the Timer1 Value when the input is captured
 *              The value stored at Input Capture Register ICR1
 */
uint16 ICU_getInputCaptureValue(void)
{
	return ICR1;
}

/*
 * Description: Function to clear the Timer1 Value to start count from ZERO
 */
void ICU_clearTimer( void )
{
	TCNT1 = 0;
	ICR1 = 0;
}

/*
 * Description: Function to disable the Timer1 to stop the ICU Driver
 */
void ICU_Deinit( void )
{
	/* Clear All Timer1 Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	ICR1 = 0;
	/* Disable Input capture interrupt */
	TIMSK &= ~(1<<TICIE1);
}

