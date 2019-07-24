//
//  linefollowLab7.c
//
//
//  Created by Daniel Mohazab on 2019-07-14.
//
//

#include "mte220.c" //Must be included, C functions needed for labs
//#include <stdbool.h>

#define belowZeroGauss 0x7D
#define aboveZeroGauss 0x82

#define LEFT_STOP (uns16)3500
#define RIGHT_STOP (uns16)3400
#define LEFT_MAX (uns16)2875
#define RIGHT_MAX (uns16)3875

#define BothServosQuarterOn \
  SetLeft(3344); \
  SetRight(3519);

#define BothServosHalfOn \
  SetLeft(3188); \
  SetRight(3638);

void main(void)
{
	uns8 stopped = 0;
	uns8 magnet_value;  // current ADC value
  uns8 IR_value;
	int detect = 0; // If magnet has recently been detected
	Initialization();
  UseServos         // (syntax of "call" is correct without () or ;)

	WaitForButton();  // wait until the button is pushed
	/*
	LongDelay(1);
	BothServosQuarterOn
	LongDelay(2);
	BothServosHalfOn
	LongDelay(2);
	*/
	BothServosOn
	LongDelay(2);

	//LeftServoOn
	//WaitForButton();


//255/5 * x

  while (1)  // loop forever
  {
		// get analog value from the Hall effect sensor
    //Ours would be > 0x99
		magnet_value = AnalogConvert(ADC_HALL_EFFECT);
		if(magnet_value > 0xA0 && detect == 0) //160 decimal and not recently detected (3.14V)
		{
			magnet_value = AnalogConvert(ADC_HALL_EFFECT);
			if(magnet_value > 0xA0) //160 decimal (3.14V)
			{
				magnet_value = AnalogConvert(ADC_HALL_EFFECT);
				if(magnet_value > 0xA0) //160 decimal (3.14V)
				{
					magnet_value = AnalogConvert(ADC_HALL_EFFECT);
					if(magnet_value > 0xA0) //160 decimal (3.14V)
					{
						magnet_value = AnalogConvert(ADC_HALL_EFFECT);
						if(magnet_value > 0xA0) //160 decimal (3.14V)
						{
							// Turn on LED for fraction of time corresponding to analog value
							//need to check if BothServosOff comes to a full stop
							//need to verify on track if the blink/solid are in the correct
							//statements
							detect = 1; // Set recently detected
							BothServosHalfOn
							LongDelay(1);
							BothServosQuarterOn
							LongDelay(1);
							BothServosOff;
							OnLED;
							LongDelay(56);
							OffLED;
							BothServosQuarterOn
							LongDelay(1);
							BothServosHalfOn
							LongDelay(1);
							BothServosOn;
						}
					}
				}
			}
		}
    //Ours would be < 0x56
		if(magnet_value < 0x5F && detect == 0) //95 decimal and not recently detected
		{
			magnet_value = AnalogConvert(ADC_HALL_EFFECT);
			if(magnet_value < 0x5F) //95 decimal (1.86V)
			{
				magnet_value = AnalogConvert(ADC_HALL_EFFECT);
				if(magnet_value < 0x5F) //95 decimal (1.86V)
				{
					magnet_value = AnalogConvert(ADC_HALL_EFFECT);
					if(magnet_value < 0x5F) //95 decimal (1.86V)
					{
						magnet_value = AnalogConvert(ADC_HALL_EFFECT);
						if(magnet_value < 0x5F) //95 decimal (1.86V)
						{
							// Turn off LED
							detect = 1; // Set recently detected
							int i = 0;
							BothServosHalfOn
							LongDelay(1);
							BothServosQuarterOn
							LongDelay(1);
							BothServosOff;
							while(i<28)
							{
								OnLED;
								LongDelay(1);
								OffLED;
								LongDelay(1);
								i++;
							}
							BothServosQuarterOn
							LongDelay(1);
							BothServosHalfOn
							LongDelay(1);
							BothServosOn;
						}
					}
				}
			}
		}
    //110 Decimal and 140 Decimal ---- 2.16V and 2.75V
    //Ours would be around the same
		if(magnet_value > 0x6E && magnet_value < 0x8C)
		{
			magnet_value = AnalogConvert(ADC_HALL_EFFECT);
			if(magnet_value > 0x6E && magnet_value < 0x8C)
			{
				magnet_value = AnalogConvert(ADC_HALL_EFFECT);
				if(magnet_value > 0x6E && magnet_value < 0x8C)
				{
					magnet_value = AnalogConvert(ADC_HALL_EFFECT);
					if(magnet_value > 0x6E && magnet_value < 0x8C)
					{
						magnet_value = AnalogConvert(ADC_HALL_EFFECT);
						if(magnet_value > 0x6E && magnet_value < 0x8C)
							detect = 0; // Unset recently detected
					}
				}
			}
		}

    IR_value = AnalogConvert(ADC_IR_SENSOR);  // get analog value from IR sensor diff amp

    if ( IR_value < 0x66 )  // 0x66 is 2V for 10-bit ADC with 2 LSB dropped
    {
      // left servo only
      LeftServoOn
      RightServoOff
    }
    else if ( IR_value > 0x99 )  // 0x99 is 3V for 10-bit ADC with 2 LSB dropped
    {
      // right servo only
      RightServoOn
      LeftServoOff
    }
    else
    {
      // both servos on
      BothServosOn
    }
	}
}
