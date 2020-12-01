/** @file   ir_array.h
 *  @brief  This file contains the implementation of an ir_array that has
 *          8 IR LEDs and 8 photodiodes capable of detecting if a specific
 *          section of the array is over a strip of black tape. The ir_array
 *          used in the term project is a Pololu QTR-8RC Reflectance Sensor
 *          Array.
 * 
 *  @author Weston Montgomery 
 *  @author Alex Haduong 
 *  @author Aris Recidoro
 *  
 *  @date    12 Nov 2020 File Created
 *  @date    13 Nov 2020 Class Structure, variables, and methods created
 */


#ifndef IR_ARRAY_H
#define IR_ARRAY_H

#include <Arduino.h>

/** @brief   Class which implements a 8 sensor IR reflectance array 
 *  @details This class creates objects for 8 sensor IR reflectance arrays,
 *           primarily of the Pololu QTR-8RC type. The class has 8 bit integers 
 *           to hold the pins on the nucleo that are used for the IR array
 */
class IR_Array
{
private:

// 8 bit integers to hold which pin of the nucleo each sensor is connected to
uint8_t sensorPin_1;
uint8_t sensorPin_2;
uint8_t sensorPin_3;
uint8_t sensorPin_4;
uint8_t sensorPin_5;
uint8_t sensorPin_6;
uint8_t sensorPin_7;
uint8_t sensorPin_8;


public:

/** @brief      Constructor to make an IR_Array and initializes the 
 *              pins on the nucleo microcontroller
 * 
 *  @param      sensorPins  Array of 8 bit integers that describe which pins
 *                          of the microcontroller are assigned for each sensor
 */
IR_Array(uint8_t sensorPins[]);

/** @brief      Getter method that returns the status of sensor 1
 */
bool getSensor_1();

/** @brief      Getter method that returns the status of sensor 2
 */
bool getSensor_2();

/** @brief      Getter method that returns the status of sensor 3
 */
bool getSensor_3();

/** @brief      Getter method that returns the status of sensor 4
 */
bool getSensor_4();

/** @brief      Getter method that returns the status of sensor 5
 */
bool getSensor_5();

/** @brief      Getter method that returns the status of sensor 6
 */
bool getSensor_6();

/** @brief      Getter method that returns the status of sensor 7
 */
bool getSensor_7();

/** @brief      Getter method that returns the status of sensor 8
 */
bool getSensor_8();


}; //end class decleration

#endif //end if: define ir_array class declaration