/** @file   ir_array.cpp
 *  @brief  This file contains the definition of ir_array methods that has
 *          8 IR LEDs and 8 photodiodes capable of detecting if a specific
 *          section of the array is over a strip of black tape. The ir_array
 *          used in the term project is a Pololu QTR-8RC Reflectance Sensor
 *          Array.
 * 
 *  @author Weston Montgomery 
 *  @author Alex Haduong 
 *  @author Aris Recidoro
 * 
 *  @date   12 Nov 2020     File Created
 *  @date   13 Nov 2020     Constructor defined
 */

#include "ir_array.h"

/** @brief      Constructor to make an IR_Array and initializes the 
 *              pins on the nucleo microcontroller.
 * 
 *  @param      sensorPins  Array of 8 bit integers that describe which pins
 *                          of the microcontroller are assigned for each sensor
 */
IR_Array::IR_Array(uint8_t sensorPins[]){

    // assign class variable sensorPin values from the array passed in
    // to the parameter
    sensorPin_1 = sensorPins[0];
    sensorPin_2 = sensorPins[1];
    sensorPin_3 = sensorPins[2];
    sensorPin_4 = sensorPins[3];
    sensorPin_5 = sensorPins[4];
    sensorPin_6 = sensorPins[5];
    sensorPin_7 = sensorPins[6];
    sensorPin_8 = sensorPins[7];


    // Initializes the pins as input pins using the pinMode Arduino function
    pinMode(sensorPin_1, INPUT);
    pinMode(sensorPin_2, INPUT);
    pinMode(sensorPin_3, INPUT);
    pinMode(sensorPin_4, INPUT);
    pinMode(sensorPin_5, INPUT);
    pinMode(sensorPin_6, INPUT);
    pinMode(sensorPin_7, INPUT);
    pinMode(sensorPin_8, INPUT);

}


/** @brief      Getter method that returns the status of sensor 1
 */
bool IR_Array::getSensor_1(){
    return digitalRead(sensorPin_1);
}

/** @brief      Getter method that returns the status of sensor 2
 */
bool IR_Array::getSensor_2(){
    return digitalRead(sensorPin_2);
}

/** @brief      Getter method that returns the status of sensor 3
 */
bool IR_Array::getSensor_3(){
    return digitalRead(sensorPin_3);
}

/** @brief      Getter method that returns the status of sensor 4
 */
bool IR_Array::getSensor_4(){
    return digitalRead(sensorPin_4);
}

/** @brief      Getter method that returns the status of sensor 5
 */
bool IR_Array::getSensor_5(){
    return digitalRead(sensorPin_5);
}

/** @brief      Getter method that returns the status of sensor 6
 */
bool IR_Array::getSensor_6(){
    return digitalRead(sensorPin_6);
}

/** @brief      Getter method that returns the status of sensor 7
 */
bool IR_Array::getSensor_7(){
    return digitalRead(sensorPin_7);
}

/** @brief      Getter method that returns the status of sensor 8
 */
bool IR_Array::getSensor_8(){
    return digitalRead(sensorPin_8);
}

