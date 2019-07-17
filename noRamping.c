//
//  linefollowLab7.c
//
//
//  Created by Daniel Mohazab on 2019-07-14.
//
//

#include "mte220.ca" //Must be included, C functions needed for labs

void main(void) {

    uns8 analog_value_magnets; //Current Analog-Digital Converter value for Hall effect sensor
    uns8 analog_value_lines; //Current Analog-Digital Converter value for IR sensor

    Initialization(); //Sets up ADCs, timing, interrupts

    UseServos //Macro for using servo motors

    WaitForButton(); //Waits until the button is pressed before starting

    BothServosOn //Turns both Servo motors on at once

    while(1){

        analog_value_lines = AnalogConvert(ADC_IR_SENSOR);  // get analog value from IR sensor diff amp
        analog_value_magnets = AnalogConvert(ADC_HALL_EFFECT); // get analog value from the hall effect sensor

        if ( analog_value_lines < 0x66 )  // 0x66 is 2V for 10-bit ADC with 2 LSB dropped
        {
            // left servo only
            LeftServoOn
            RightServoOff
        }
        else if ( analog_value_lines > 0x99 )  // 0x99 is 3V for 10-bit ADC with 2 LSB dropped
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

        //At 0 Gauss, there is no magnetic field, and the output is 2.5v, meaning it has 255/2 (around 127.5) so to account for some error we will say
        //that if the reading is above 130 or below 125 then there is a magnet that is being detected (to also account for some noise)
        if (analog_value_magnets < 0x7D || analog_value_magnets > 0x82){
          //Magnet is detected, so robot needs to stop
          BothServosOff

          //If value of the sensor is below 0 Gauss, green LED blinked for 7 seconds

          if (analog_value_magnets < 0x7D){
            //Loops 7 times, 7*8000 is 56000 which is 7 seconds
            for (int i = 0; i < 7; i++) {
              OnLED
              LongDelay(4000);
              OffLED
              LongDelay(4000)
            }
            //We don't want to detect this magnet again so soon, so we must run the robot a bit before attempting to detect more magnets
            //We run until the condition of the magnet we just reached is gone, since magnets are 20cm apart we will reach 0 gauss before sensing another magnet again
            while (analog_value_magnets < 0x7D){

              analog_value_lines = AnalogConvert(ADC_IR_SENSOR);  // get analog value from IR sensor diff amp
              analog_value_magnets = AnalogConvert(ADC_HALL_EFFECT); // get analog value from the hall effect sensor

              if ( analog_value_lines < 0x66 )  // 0x66 is 2V for 10-bit ADC with 2 LSB dropped
              {
                  // left servo only
                  LeftServoOn
                  RightServoOff
              }
              else if ( analog_value_lines > 0x99 )  // 0x99 is 3V for 10-bit ADC with 2 LSB dropped
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

          //If value of the sensor is above 0 gauss, LED on for 7 seconds

          else if (analog_value_magnets > 0x82){
            OnLED
            LongDelay(56000);
            OffLED

            //We don't want to detect this magnet again so soon, so we must run the robot a bit before attempting to detect more magnets
            //We run until the condition of the magnet we just reached is gone, since magnets are 20cm apart we will reach 0 gauss before sensing another magnet again
            while (analog_value_magnets > 0x82){

              analog_value_lines = AnalogConvert(ADC_IR_SENSOR);  // get analog value from IR sensor diff amp
              analog_value_magnets = AnalogConvert(ADC_HALL_EFFECT); // get analog value from the hall effect sensor

              if ( analog_value_lines < 0x66 )  // 0x66 is 2V for 10-bit ADC with 2 LSB dropped
              {
                  // left servo only
                  LeftServoOn
                  RightServoOff
              }
              else if ( analog_value_lines > 0x99 )  // 0x99 is 3V for 10-bit ADC with 2 LSB dropped
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
          //Robot should then continue to move along the line until another magnet is detected or until reaching finish line
        }
    }
}


//Delay(count) Wait approximately count * 0.5 us
//LongDelay(count) Wait approximately count * 0.125 ms

/*

BothServosOn -- Turns on the servos

BothServosOff -- Turns off the servos

LongDelay(16); -- Waits for around 2 seconds

// get analog value from the Hall effect sensor
analog_value = AnalogConvert(ADC_HALL_EFFECT);

// Turn on LED for fraction of time corresponding to analog value
OnLED
Delay(analog_value);

// Turn off LED
OffLED
analog_value = 255 - analog_value;
Delay(analog_value);

The duration of the LED being on/off is set to correspond to the intensity of the magnetic field, at 0 Gauss the LED is blinking,
the time on and time off are equal.

Value of output is nominally 2.5V at 0 gauss, this means that 255/5 * 2.5 is effectively half of 255, which is why 255-(255*0.5) makes it blink at same time

In the presence of a magnetic field however, the ratio of OnLED and OffLED will change with the visual effect of either the LED being on continuously or LED being off

For detecting fields of different polarities, compare the hall sensor to a couple of constants, for example analog_value<0x66 and analog_value>0x99 is 2V and 3V respectively

To avoid microreset, we must gradually turn motors off/on
*/
