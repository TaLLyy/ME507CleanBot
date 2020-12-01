/** @file main.cpp
 *      This file contains a program that runs our term project cleanbot. The
 *      microcontroller is being used to communicate with mutiple sensors including 
 *      an IR sensor array, two motors, and a WiFi reciever, all running
 *      the arduinio framework.
 *
 *  @author  WC Montgomery, A Recidoro, A Haduong
 * 
 *  @date    05 Nov 2020    Original file
 */

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <ir_array.h>
#include <motor_driver.h>
#include <wifi_system.h>
#include <taskshare.h>


extern Share<bool> wifi_to_motor_share;
extern Share<uint8_t> driveState_share;

/** @brief   Task which controls CleanBot's motors.
 *  @details This task controls the drivetrain system. This includes
 *           controling the motor driver and the motor encoders. This tasks uses
 *           those components to get the cleanbot to go given the information from the 
 *           vision subsytem. Can go into 5 different drive states:
 *           0: drive in straight line
 *           1: rotate CCW
 *           2: turn left
 *           3: turn right
 *           4: rotate CW
 *           
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_drive_train(void* p_params)
{   
    (void) p_params;
    bool wifi_flag;
    uint8_t IR_flag;
    const uint8_t max_speed= 255;
    Motor_Driver leftMotor;
    Motor_Driver rightMotor;

    leftMotor.SetPins (5, 3);
    rightMotor.SetPins(6, 9); // check and make sure the pins are correct

    for (;;)
    {   
        wifi_to_motor_share.get (wifi_flag);
        driveState_share.get (IR_flag);
        if(wifi_flag == true)     // checks wifi reciever to see if signal has been sent to turn on
        {
          if(IR_flag == 0)    // check what IR array flag gives and point to right order
          {
            // go straight

            leftMotor.ChangeSpeed(.5*max_speed);  // call class of motor driver and set left wheel forward  .5 max speed
            rightMotor.ChangeSpeed(.5*max_speed); // call class of motor driver and set right wheel forward .5 max speed
          }                
          else if(IR_flag == 1)
          {
            // rotate counter clockwise

            leftMotor.ChangeSpeed(.5*max_speed);  // call class of motor driver and set left wheel forward .5 max speed
            rightMotor.ChangeSpeed(-.5*max_speed); // call class of motor driver and set right wheel backward .5 max speed
          }
          else if(IR_flag == 2)
          {
            // turn left, right wheel spins twice as fast as left

            leftMotor.ChangeSpeed(.25*max_speed); // call class of motor driver and set left wheel to .25 max speed
            rightMotor.ChangeSpeed(.5*max_speed); // call class of motor driver and set right wheel to .5 max speed
          }
          else if (IR_flag == 3)
          {
            // turn right, left wheel spins twice as fast as left

            leftMotor.ChangeSpeed(.5*max_speed);  // call class of motor driver and set left wheel to .5 max speed
            rightMotor.ChangeSpeed(.25*max_speed); // call class of motor driver and set right wheel to .25 max speed
          }
          else if (IR_flag == 4)
          {
            // rotate clockwise

            leftMotor.ChangeSpeed(-.5*max_speed);  // call class of motor driver and set left wheel backward .5 max speed
            rightMotor.ChangeSpeed(.5*max_speed); // call class of motor driver and set right wheel forward .5 max speed
          }
        }

    }
}

/** @brief   Task which gets information from IR array
 *  @details lets IR array see whether it senses a black line on the ground
 *           and gives the proper instructions to the drive train subsystem
 *           using a uint8_t share. Runs every 5 ms and sets a drive state:
 *           0: drive in straight line
 *           1: rotate CCW
 *           2: turn left
 *           3: turn right
 *           4: rotate CW
 * 
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_IR_array (void* p_params)
{   
  (void) p_params;
    //Timing of the task: 5 ms between sensor reads
    const TickType_t TASK_DELAY = 5;

    //create int values to hold which pins of the microcontroller the IR Array is connected to
    uint8_t IR_Pin_1 = 2;
    uint8_t IR_Pin_2 = 4;
    uint8_t IR_Pin_3 = 7;
    uint8_t IR_Pin_4 = 8;
    uint8_t IR_Pin_5 = 10;
    uint8_t IR_Pin_6 = 11;
    uint8_t IR_Pin_7 = 12;
    uint8_t IR_Pin_8 = 13;
    uint8_t sensorPins [8] = {IR_Pin_1, IR_Pin_2, IR_Pin_3, IR_Pin_4, IR_Pin_5, IR_Pin_6, IR_Pin_7, IR_Pin_8};

    //create IR_Array object with pin values connected to the microcontroller
    IR_Array* lineArray = new IR_Array(sensorPins);

    for (;;)
    {
        // call the ir_array task and make variables true depending on which
        // io any IR sensors were tripped
        // Sensor 8 is on bot's driver side, sensor 1 is on bot's passenger side

        //drive state 0: drive straight when only the middle sensors 4 and 5 see the line 
        if (lineArray->getSensor_8() == false && 
            lineArray->getSensor_7() == false &&
            lineArray->getSensor_6() == false && 
            lineArray->getSensor_5() == true && 
            lineArray->getSensor_4() == true && 
            lineArray->getSensor_3() == false && 
            lineArray->getSensor_2() == false &&
            lineArray->getSensor_1() == false)
            {
                driveState_share.put(0);
            }
        //drive state 1: rotate CCW when sensors 8-5 (driver side) see the line
        //and sensors 4-1 (passengers side) do NOT see the line 
        else if(lineArray->getSensor_8() == true && 
                lineArray->getSensor_7() == true &&
                lineArray->getSensor_6() == true && 
                lineArray->getSensor_5() == true && 
                lineArray->getSensor_4() == false && 
                lineArray->getSensor_3() == false && 
                lineArray->getSensor_2() == false &&
                lineArray->getSensor_1() == false)
                {
                    driveState_share.put(1);
                }
        //drive state 2: turn left (right wheel drives at 2x the speed of left wheel)
        //when (sensors 5 and 6) OR (sensors 6 and 7) see the line and all other sensors
        //do NOT see the line
        else if( (lineArray->getSensor_8() == false && 
                lineArray->getSensor_7() == true &&
                lineArray->getSensor_6() == true && 
                lineArray->getSensor_5() == false && 
                lineArray->getSensor_4() == false && 
                lineArray->getSensor_3() == false && 
                lineArray->getSensor_2() == false &&
                lineArray->getSensor_1() == false) 
                ||
                (lineArray->getSensor_8() == false && 
                lineArray->getSensor_7() == false &&
                lineArray->getSensor_6() == true && 
                lineArray->getSensor_5() == true && 
                lineArray->getSensor_4() == false && 
                lineArray->getSensor_3() == false && 
                lineArray->getSensor_2() == false &&
                lineArray->getSensor_1() == false) )
                {
                    driveState_share.put(2);
                }
        //drive state 3: turn right (left wheel drives at 2x the speed of right wheel)
        //when (sensors 4 and 3) OR (sensors 3 and 2) see the line and all other sensors
        //do NOT see the line
        else if( (lineArray->getSensor_8() == false && 
                lineArray->getSensor_7() == false &&
                lineArray->getSensor_6() == false && 
                lineArray->getSensor_5() == false && 
                lineArray->getSensor_4() == true && 
                lineArray->getSensor_3() == true && 
                lineArray->getSensor_2() == false &&
                lineArray->getSensor_1() == false) 
                ||
                (lineArray->getSensor_8() == false && 
                lineArray->getSensor_7() == false &&
                lineArray->getSensor_6() == false && 
                lineArray->getSensor_5() == false && 
                lineArray->getSensor_4() == false && 
                lineArray->getSensor_3() == true && 
                lineArray->getSensor_2() == true &&
                lineArray->getSensor_1() == false) )
                {
                    driveState_share.put(3);
                }
        //drive state 4: rotate CW when sensors 4-1 (passenger side) see the line
        //and sensors 8-5 (drivers side) do NOT see the line 
        else if(lineArray->getSensor_8() == false && 
                lineArray->getSensor_7() == false &&
                lineArray->getSensor_6() == false && 
                lineArray->getSensor_5() == false && 
                lineArray->getSensor_4() == true && 
                lineArray->getSensor_3() == true && 
                lineArray->getSensor_2() == true &&
                lineArray->getSensor_1() == true)
                {
                    driveState_share.put(4);
                }        

        vTaskDelay(TASK_DELAY);

    }//end for: infinite loop to run during the task

}//end task: task_IR_array

/** @brief   Task which gets turns CleanBot on or off from Wifi Reciever
 *  @details 
 *           
 *           Example code demonstrates how to run a task on an approximate        //taken from ridgley lab
 *           schedule using @c vTaskDelay().  If we need to run a task on a
 *           more accurate schedule that doesn't accumulate delays, we should
 *           use @c vTaskDelayUntil() to do the scheduling.
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_wifi_reciever (void)
{

    for (;;)
    {
        // call wifi reciever function and put result into share

        // code that turns CleanBot On/off depending on if 
        // signal has been sent. Send a share to drive train task 
        // needs to be true for rest of the task to run 
        // and LED to be on
        
        // will put a boolean into wifi_flag
        

    }
}

/** @brief   turns LED on or off depending on the signal from the wifi reciever
 *  @details 
 *           
 *           Example code demonstrates how to run a task on an approximate        //taken from ridgley lab
 *           schedule using @c vTaskDelay().  If we need to run a task on a
 *           more accurate schedule that doesn't accumulate delays, we should
 *           use @c vTaskDelayUntil() to do the scheduling.
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void led_task (void)
{

    for (;;)
    {
        // put boolean share into a variable
        // see if bool is true or false
        // if true, turn LED ON
        // if false, turn LED OFF

    }
}

/** @brief   reads the encoders position and puts it into queue to be sent to
 *           the motor driver task
 *  @details 
 *           
 *           Example code demonstrates how to run a task on an approximate        //taken from ridgley lab
 *           schedule using @c vTaskDelay().  If we need to run a task on a
 *           more accurate schedule that doesn't accumulate delays, we should
 *           use @c vTaskDelayUntil() to do the scheduling.
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void encoder_task (void)
{

    for (;;)
    {
        // read encoder from left motor
        // put position into left_motor_position queue
        // read encoder from right motor
        // put position into left_motor_position queue

        // clear encoders if told to by motor driver task
    }
}


void setup() {
  // put your setup code here, to run once:
}

/** @brief   Arduino's low-priority loop function, which we don't use.
 *  @details A non-RTOS Arduino program runs all of its continuously running
 *           code in this function after @c setup() has finished. When using
 *           FreeRTOS, @c loop() implements a low priority task on most
 *           microcontrollers, and crashes on some others, so we'll not use it.
 */
void loop () 
{
}