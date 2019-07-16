//
//  linefollowLab7.c
//
//
//  Created by Daniel Mohazab on 2019-07-14.
//
//

#include "mte220.c" //Must be included, C functions needed for labs
#include <stdbool.h>

#define belowZeroGauss 0x7D
#define aboveZeroGauss 0x82
#define LEFT_STOP (uns16)3512
#define RIGHT_STOP (uns16)3395
#define LEFT_MAX (uns16)2875
#define RIGHT_MAX (uns16)3875

//3395 (stop) to 3875 max speed right
void rightOn() {
  uns16 rightMotorIncrement = (RIGHT_MAX - RIGHT_STOP)/25; //This gives 19.2, truncated to 19
  uns16 stopToStart = RIGHT_STOP;
  for(int i = 0; i<25; i++) {
    stopToStart += rightMotorIncrement;
    SetRight(stopToStart);
    LongDelay(160);
  }
  //Value is at 3870 ticks, set to max
  SetRight(RIGHT_MAX);
}

//From 3512 to 2875 start left
void leftOn() {
  uns16 leftMotorIncrement = (LEFT_STOP - LEFT_MAX)/25; //This gives 25.48, truncated to 25
  uns16 stopToStart = LEFT_STOP;
  for(int i = 0; i < 25; i++) {
    stopToStart -= leftMotorIncrement;
    SetLeft(stopToStart);
    LongDelay(160);
  }
  //Value is now at 2887 ticks, set to max
  SetLeft(LEFT_MAX);
}

//From 3875 max to 3395 stop right
void rightOff() {
  uns16 rightMotorIncrement = (RIGHT_MAX - RIGHT_STOP)/25; //19.2 truncated to 19
  uns16 maxToStop = RIGHT_MAX;
  for(int i = 0; i < 25; i++) {
    maxToStop -= rightMotorIncrement;
    SetRight(maxToStop);
    LongDelay(160);
  }
  //Value is at 3400, set to stop
  SetRight(RIGHT_STOP);
}

//From 2875 max speed to 3512 stop left
void leftOff() {
  uns16 leftMotorIncrement = (LEFT_STOP - LEFT_MAX)/25; //25.48, truncated to 25
  uns16 maxToStop = LEFT_MAX;

  for(int i = 0; i<25; i++) {
    maxToStop += leftMotorIncrement;
    SetLeft(maxToStop);
    LongDelay(160);
  }
  //Value is at 3500 at this point, set to stop
  SetLeft(LEFT_STOP);
}

//3395 (stop) to 3875 max speed right
//From 2875 max speed to 3512 stop left
void leftOffRightOn() {
  uns16 rightMotorIncrement = (RIGHT_MAX - RIGHT_STOP)/25; //This gives 19.2, truncated to 19
  uns16 stopToStart = RIGHT_STOP;
  uns16 leftMotorIncrement = (LEFT_STOP - LEFT_MAX)/25; //25.48, truncated to 25
  uns16 maxToStop = LEFT_MAX;

  for(int i = 0; i<25; i++) {
    stopToStart += rightMotorIncrement;
    maxToStop += leftMotorIncrement;
    SetRight(stopToStart);
    SetLeft(maxToStop);
    LongDelay(160);
  }
  //Value is at 3870 ticks, set to max
  SetRight(RIGHT_MAX);
  //Value is at 3500 at this point, set to stop
  SetLeft(LEFT_STOP);
}

//From 3875 max to 3395 stop right
//From 3512 to 2875 start left
void leftOnRightOff() {
  uns16 rightMotorIncrement = (RIGHT_MAX - RIGHT_STOP)/25; //19.2 truncated to 19
  uns16 maxToStop = RIGHT_MAX;
  uns16 leftMotorIncrement = (LEFT_STOP - LEFT_MAX)/25; //This gives 25.48, truncated to 25
  uns16 stopToStart = LEFT_STOP;
  for(int i = 0; i < 25; i++) {
    maxToStop -= rightMotorIncrement;
    stopToStart -= leftMotorIncrement;
    SetRight(maxToStop);
    SetLeft(stopToStart);
    LongDelay(160);
  }
  //Value is at 3400, set to stop
  SetRight(RIGHT_STOP);
  //Value is now at 2887 ticks, set to max
  SetLeft(LEFT_MAX);
}

//From 2875 max speed to 3512 stop left
//From 3875 max to 3395 stop right
void leftOffRightOff() {
  uns16 rightMotorIncrement = (RIGHT_MAX - RIGHT_STOP)/25; //19.2 truncated to 19
  uns16 maxToStopRight = RIGHT_MAX;
  uns16 leftMotorIncrement = (LEFT_STOP - LEFT_MAX)/25; //25.48, truncated to 25
  uns16 maxToStopLeft = LEFT_MAX;
  for(int i = 0; i < 25; i++) {
    maxToStopLeft += leftMotorIncrement;
    maxToStopRight -= rightMotorIncrement;
    SetRight(maxToStopRight);
    SetLeft(maxToStopLeft);
    LongDelay(160);
  }
  //Value is at 3400, set to stop
  SetRight(RIGHT_STOP);
  //Value is at 3500 at this point, set to stop
  SetLeft(LEFT_STOP);
}

//From 3512 to 2875 start left
//3395 (stop) to 3875 max speed right
void leftOnRightOn() {
  uns16 leftMotorIncrement = (LEFT_STOP - LEFT_MAX)/25; //This gives 25.48, truncated to 25
  uns16 stopToStartLeft = LEFT_STOP;
  uns16 rightMotorIncrement = (RIGHT_MAX - RIGHT_STOP)/25; //This gives 19.2, truncated to 19
  uns16 stopToStartRight = RIGHT_STOP;
  for(int i = 0; i < 25; i++) {
    stopToStartLeft -= leftMotorIncrement;
    stopToStartRight += rightMotorIncrement;
    SetLeft(stopToStartLeft);
    SetRight(stopToStartRight);
    LongDelay(160);
  }
  SetRight(RIGHT_MAX);
  SetLeft(LEFT_MAX);
}

void main(void) {

    uns8 analog_value_magnets; //Current Analog-Digital Converter value for Hall effect sensor
    uns8 analog_value_lines; //Current Analog-Digital Converter value for IR sensor

    Initialization(); //Sets up ADCs, timing, interrupts

    UseServos //Macro for using servo motors

    WaitForButton(); //Waits until the button is pressed before starting

    leftOnRightOn();
    bool rightOff = false;
    bool leftOff = false;

    while(1) {

        analog_value_magnets = AnalogConvert(ADC_HALL_EFFECT); // get analog value from the hall effect sensor

        analog_value_lines = AnalogConvert(ADC_IR_SENSOR);  // get analog value from IR sensor diff amp

        if ( analog_value_lines < 0x66 )  // 0x66 is 2V for 10-bit ADC with 2 LSB dropped
        {
          //if left is already off and right is on
          if(leftOff == true && rightOff == false){
            SetLeft(LEFT_STOP);
            SetRight(RIGHT_MAX);
          }
          //if left is on and right is off
          else if(leftOff == false && rightOff == true){
            leftOffRightOn();
          }
          //if left is off and right is off
          else if(leftOff == true && rightOff == true){
            SetLeft(LEFT_STOP);
            rightOn();
          }
          //If left is on and right is on
          else if(leftOff == false && rightOff == false) {
            SetRight(RIGHT_MAX);
            leftOff();
          }
          leftOff = true;
          rightOff = false;
        }
        else if ( analog_value_lines > 0x99 )  // 0x99 is 3V for 10-bit ADC with 2 LSB dropped
        {
          //if left is already off and right is on
          if(leftOff == true && rightOff == false){
            leftOnRightOff();
          }
          //if left is on and right is off
          else if(leftOff == false && rightOff == true){
            SetLeft(LEFT_MAX);
            SetRight(RIGHT_STOP);
          }
          //if left is off and right is off
          else if(leftOff == true && rightOff == true){
            leftOn();
            SetRight(RIGHT_STOP);
          }
          //If left is on and right is on
          else if(leftOff == false && rightOff == false) {
            SetLeft(LEFT_MAX);
            rightOff();
          }
          leftOff = false;
          rightOff = true;
        }
        else
        {
          //if left is already off and right is on
          if(leftOff == true && rightOff == false){
            leftOn();
            SetRight(RIGHT_MAX);
          }
          //if left is on and right is off
          else if(leftOff == false && rightOff == true){
            SetLeft(LEFT_MAX);
            rightOn();
          }
          //if left is off and right is off
          else if(leftOff == true && rightOff == true){
            leftOnRightOn();
          }
          //If left is on and right is on
          else if(leftOff == false && rightOff == false) {
            leftOffRightOff();
          }
          leftOnRightOn();
          leftOff = false;
          rightOff = false;
        }

        //At 0 Gauss, there is no magnetic field, and the output is 2.5v, meaning it has 255/2 (around 127.5) so to account for some error we will say
        //that if the reading is above 130 or below 125 then there is a magnet that is being detected (to also account for some noise)
        if (analog_value_magnets < belowZeroGauss || analog_value_magnets > aboveZeroGauss){
          //Magnet is detected, so robot needs to stop
          leftOffRightOff();
          rightOff = true;
          leftOff = true;

          //If value of the sensor is below 0 Gauss, green LED blinked for 7 seconds

          if (analog_value_magnets < belowZeroGauss){
            //Loops 7 times, 7*8000 is 56000 which is 7 seconds
            for (int i = 0; i < 7; i++) {
              OnLED
              LongDelay(4000);
              OffLED
              LongDelay(4000)
            }
            //We don't want to detect this magnet again so soon, so we must run the robot a bit before attempting to detect more magnets
            //We run until the condition of the magnet we just reached is gone, since magnets are 20cm apart we will reach 0 gauss before sensing another magnet again
            while (analog_value_magnets < belowZeroGauss) {

              analog_value_magnets = AnalogConvert(ADC_HALL_EFFECT); // get analog value from the hall effect sensor

              analog_value_lines = AnalogConvert(ADC_IR_SENSOR);  // get analog value from IR sensor diff amp

              if ( analog_value_lines < 0x66 )  // 0x66 is 2V for 10-bit ADC with 2 LSB dropped
              {
                //if left is already off and right is on
                if(leftOff == true && rightOff == false){
                  SetLeft(LEFT_STOP);
                  SetRight(RIGHT_MAX);
                }
                //if left is on and right is off
                else if(leftOff == false && rightOff == true){
                  leftOffRightOn();
                }
                //if left is off and right is off
                else if(leftOff == true && rightOff == true){
                  SetLeft(LEFT_STOP);
                  rightOn();
                }
                //If left is on and right is on
                else if(leftOff == false && rightOff == false) {
                  SetRight(RIGHT_MAX);
                  leftOff();
                }
                leftOff = true;
                rightOff = false;
              }
              else if ( analog_value_lines > 0x99 )  // 0x99 is 3V for 10-bit ADC with 2 LSB dropped
              {
                //if left is already off and right is on
                if(leftOff == true && rightOff == false){
                  leftOnRightOff();
                }
                //if left is on and right is off
                else if(leftOff == false && rightOff == true){
                  SetLeft(LEFT_MAX);
                  SetRight(RIGHT_STOP);
                }
                //if left is off and right is off
                else if(leftOff == true && rightOff == true){
                  leftOn();
                  SetRight(RIGHT_STOP);
                }
                //If left is on and right is on
                else if(leftOff == false && rightOff == false) {
                  SetLeft(LEFT_MAX);
                  rightOff();
                }
                leftOff = false;
                rightOff = true;
              }
              else
              {
                //if left is already off and right is on
                if(leftOff == true && rightOff == false){
                  leftOn();
                  SetRight(RIGHT_MAX);
                }
                //if left is on and right is off
                else if(leftOff == false && rightOff == true){
                  SetLeft(LEFT_MAX);
                  rightOn();
                }
                //if left is off and right is off
                else if(leftOff == true && rightOff == true){
                  leftOnRightOn();
                }
                //If left is on and right is on
                else if(leftOff == false && rightOff == false) {
                  leftOffRightOff();
                }
                leftOnRightOn();
                leftOff = false;
                rightOff = false;
              }
            }
          }

          //If value of the sensor is above 0 gauss, LED on for 7 seconds

          else if (analog_value_magnets > aboveZeroGauss){
            OnLED
            LongDelay(56000);
            OffLED

            //We don't want to detect this magnet again so soon, so we must run the robot a bit before attempting to detect more magnets
            //We run until the condition of the magnet we just reached is gone, since magnets are 20cm apart we will reach 0 gauss before sensing another magnet again
            while (analog_value_magnets > aboveZeroGauss){

              analog_value_magnets = AnalogConvert(ADC_HALL_EFFECT); // get analog value from the hall effect sensor

              analog_value_lines = AnalogConvert(ADC_IR_SENSOR);  // get analog value from IR sensor diff amp

              if ( analog_value_lines < 0x66 )  // 0x66 is 2V for 10-bit ADC with 2 LSB dropped
              {
                //if left is already off and right is on
                if(leftOff == true && rightOff == false){
                  SetLeft(LEFT_STOP);
                  SetRight(RIGHT_MAX);
                }
                //if left is on and right is off
                else if(leftOff == false && rightOff == true){
                  leftOffRightOn();
                }
                //if left is off and right is off
                else if(leftOff == true && rightOff == true){
                  SetLeft(LEFT_STOP);
                  rightOn();
                }
                //If left is on and right is on
                else if(leftOff == false && rightOff == false) {
                  SetRight(RIGHT_MAX);
                  leftOff();
                }
                leftOff = true;
                rightOff = false;
              }
              else if ( analog_value_lines > 0x99 )  // 0x99 is 3V for 10-bit ADC with 2 LSB dropped
              {
                //if left is already off and right is on
                if(leftOff == true && rightOff == false){
                  leftOnRightOff();
                }
                //if left is on and right is off
                else if(leftOff == false && rightOff == true){
                  SetLeft(LEFT_MAX);
                  SetRight(RIGHT_STOP);
                }
                //if left is off and right is off
                else if(leftOff == true && rightOff == true){
                  leftOn();
                  SetRight(RIGHT_STOP);
                }
                //If left is on and right is on
                else if(leftOff == false && rightOff == false) {
                  SetLeft(LEFT_MAX);
                  rightOff();
                }
                leftOff = false;
                rightOff = true;
              }
              else
              {
                //if left is already off and right is on
                if(leftOff == true && rightOff == false){
                  leftOn();
                  SetRight(RIGHT_MAX);
                }
                //if left is on and right is off
                else if(leftOff == false && rightOff == true){
                  SetLeft(LEFT_MAX);
                  rightOn();
                }
                //if left is off and right is off
                else if(leftOff == true && rightOff == true){
                  leftOnRightOn();
                }
                //If left is on and right is on
                else if(leftOff == false && rightOff == false) {
                  SetLeft(LEFT_MAX);
                  SetRight(RIGHT_MAX);
                }
                leftOff = false;
                rightOff = false;
              }
            }
          }
        }
    }
}
