/*
 * main.c
 *
 *  Created on: Oct 24, 2021
 *  Author: Kareem Ahmed Thabet
 */
#include"lcd.h"
#include"ultrasonic.h"
int main()
{
	ULTRASONIC_init();
	LCD_init();
	LCD_displayString("Distance =");
	LCD_moveCursor(0,11);
	uint32 distance = 0;
	while(1)
	{
		distance = ULTRASONIC_readDistance();
		LCD_intgerToString(distance);
	}
}

