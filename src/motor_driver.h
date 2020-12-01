/** @file   motor_driver.h
 *  @brief  This file contains the implementation of motor driver that 
 *          can set the pins of the motor and then subsequently set the speed
 *          of a motor.
 * 
 *  @author Weston Montgomery 
 *  @author Alex Haduong 
 *  @author Aris Recidoro
 *  
 *  @date    12 Nov 2020 File Created
 *  @date    13 Nov 2020 Class Structure, variables, and methods created
 */


#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

/** @brief   Class which implements a 8 sensor IR reflectance array 
 *  @details This class creates objects for 8 sensor IR reflectance arrays,
 *           primarily of the Pololu QTR-8RC type. The class has 8 bit integers 
 *           to hold the pins on the nucleo that are used for the IR array
 */
class Motor_Driver
{
private:

// 8 bit integers to hold which pins of the nucleo the motor is attatched to
uint8_t PIN_MD1_IN1;
uint8_t PIN_MD1_IN2;


public:

/** @brief      Constructor to make motordriver object and set the correct pins
 * 
 *  @param      PHASE_PIN  set the pin number for the phase pin 
 *  @param      ENABLE_PIN   set the pin number for the phase pin         
 */
void SetPins(uint8_t PHASE_PIN, uint8_t ENABLE_PIN);

/** @brief      method to set motor speed
 * 
 * @param       duty_cycle_var  set the speed for the motor to turn(max of 255)
 */
void ChangeSpeed(uint8_t duty_cycle_var);

}; //end class decleration

#endif //end if: define ir_array class declaration